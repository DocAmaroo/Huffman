#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "arbre.h"
#include "compress.h"

void decompress(char *argv[]){

	// "Ouvre" le fichier en mode lecture
	FILE * file_read;
	file_read = fopen( argv[1], "rb");

	if( file_read == NULL ) /*Si fp n'existe pas => erreur*/
	{ 
		perror("\033[1;41m[Error]\033[0m : Vérifier si vous avez bien écrit le nom de votre fichier\n");
		exit(0);
	} 

	printf("Patientez decompression en cours...\n\n");
	// "Ouvre" le fichier en mode écriture
	FILE * file_wrote;
	file_wrote = fopen( argv[2] , "w" );

	// Dirige le pointeur au début du fichier
	fseek(file_read, 0L, SEEK_SET); 

	// Taille de l'entête
	int tailleEntete = 4;

	// 1er octet
	int nb_char_diff = 0;

	// 2eme octet
	int nb_octet_for_nb_char = 0;

	// 3eme octet
	size_t nb_char = 0;

	// On récupère le caractere lu dans le fichier en lecture
	int caractere = 0;

	// Permet de savoir a quel octect on est dans le code binaire
	size_t position_bit = 0;
	int copyCaractere = 0;
	int copy_nboctet = 0;
	char* buffer = calloc(0, 8*sizeof(char));
	int position_buffer = 0;

	// lire l'entete
	while( position_bit < tailleEntete ){
		
		caractere = fgetc(file_read);

		// On récupère le 1er octet : nombre de caractere différents
		if ( position_bit == 0 ){ nb_char_diff = caractere; }

		// On récupère le 2eme octet : nombre d'octet necessaire pour coder le nombre de caractere(s) total(aux)
		if ( position_bit == 1 ){ 
		
			nb_octet_for_nb_char = caractere;
			copy_nboctet = nb_octet_for_nb_char;
			while( (copy_nboctet)%8 != 0 ) { copy_nboctet++;}

		}

		// On récupère le 3eme (ou +) octect : nombre de caractere(s) total(aux)
		// si nb_octet_for_nb_char > 8, alors on va devoir allez chercher quelques octect plus loins
		
		copyCaractere = caractere;

		if ( position_bit >= 2 ){ 
			
			if (copy_nboctet > 8){
				tailleEntete++;

				while ( copyCaractere != 0 ){

					if ( copyCaractere%2 == 0){ buffer[position_buffer] = '0'; }

					else {buffer[position_buffer] = '1';}

					position_buffer++;
					copyCaractere = copyCaractere/2;
				}

				for(int i=position_buffer; i < 8; i++){ buffer[i] = '0';}

				for (int i=8; i > 0; i--)
				{

					if ( buffer[i-1] == '1' ){ nb_char += puissance(2,copy_nboctet-1); }
					copy_nboctet--; 
				}

				position_buffer=0;


			}

			// Plus qu'a ajouter le dernier caractere 
			else{ 
				nb_char += caractere;
				position_bit++;
			}
		}

		position_bit++;

	}

	// Initialisation du dictionnaire
	dictionnaire* dico = malloc( nb_char_diff*sizeof(dictionnaire) );

	for(int i=0; i < nb_char_diff; i++){
		dico[i].caractere = '\0';
		dico[i].taille = 0;
		dico[i].code = calloc( 0, ( (nb_char_diff/2)-1 )*sizeof(dictionnaire) );
	}


	copyCaractere = 0;
	int i=0;
	int copyTaille = 0;

	while( i < nb_char_diff && (caractere = fgetc(file_read)) ){

		dico[i].caractere = caractere;

		caractere = fgetc(file_read);
		dico[i].taille = caractere;

		caractere = fgetc(file_read);

		copyCaractere = caractere;
		copyTaille = dico[i].taille;

		for(int j=0; j < copyTaille ; j++){

			if( copyCaractere%2 == 0 ){ dico[i].code[copyTaille - j - 1] = '0'; }

			else{ dico[i].code[copyTaille - j - 1] = '1'; }
			
			copyCaractere = copyCaractere/2;
		}
		i++;
	}


	// On traduit notre code 
	buffer = calloc( 0, 8*sizeof(char) );
	position_buffer = 8;
	
	char* temp_buffer = calloc(0, 8*sizeof(char));
	int position_temp_buffer = 0;

	size_t nb_char_trouve = 0;

	while( (caractere = fgetc(file_read)) != EOF ){

		copyCaractere = caractere;

		for(int i=0; i < 8 ; i++){

			if( copyCaractere%2 == 0 ){ buffer[position_buffer - i - 1] = '0'; }

			else{ buffer[position_buffer - i - 1] = '1'; }

			copyCaractere = copyCaractere/2;
		}


		for( int i=0; (i < 8) && (nb_char_trouve != nb_char) ; i++){

			temp_buffer[position_temp_buffer] = buffer[i];

			position_temp_buffer++;

			int cpt = 0;
			while( cpt < nb_char_diff ){


				if ( strcmp(dico[cpt].code, temp_buffer) == 0) {

					fputc(dico[cpt].caractere, file_wrote);
					temp_buffer = calloc(0, 8*sizeof(char));
					position_temp_buffer = 0;
					nb_char_trouve++;
				}

				cpt++;
			}
		}

	}

	// Petit message de succès qui fait jolie
	printf("\033[1;42m[Success]\033[0m : Le fichier a été décompréssé avec succès\n");
	printf("\nAffichage du fichier...\n\n");
	printf("Utiliser la commande suivante :\n   Petit fichier : cat %s\n   Gros fichier  : cat %s | more\n",argv[2], argv[2] );
	/* On ferme les fichiers */
	fclose(file_read); 
	fclose(file_wrote);

	/* On peut libérer la mémoire */
	free(temp_buffer);
	free(buffer);
	free(dico);
}
