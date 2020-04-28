#include <stdlib.h>
#include <stdio.h>

#include "distri.h"
#include "arbre.h"


int puissance(int x, int n){
	int puis=1;
	while( n > 0 ){
		puis *= x;
		n--;
	}
	
	return puis;
}

void compress(char* argv[], dictionnaire* dico, size_t tailleFichierSource){

	// "Ouvre" le fichier en mode lecture
	FILE * file_read;
	file_read = fopen( argv[1], "r");

	// "Ouvre" le fichier en mode écriture
	FILE * file_wrote;
	file_wrote = fopen( argv[2] , "wb" );

	// Dirige le pointeur au début du fichier
	fseek(file_read, 0L, SEEK_SET); 

	// On récupère la taille de notre dictionnaire
	size_t tailleDico = getSizeDico(dico);

	char buffer = 0;
	int position_buffer = 0;
	size_t copyTailleDico = tailleDico;

	char* temp_buffer = calloc ( 0, 8*sizeof(char) );

	// On ajoute le code de l'arbre de huffman en entete
	size_t i = 0;

	// 1er octect on met le nombre de caractere différent
	while ( copyTailleDico != 0 ){

		if ( copyTailleDico%2 == 0 ){
			temp_buffer[position_buffer] = '0';
			position_buffer++;
		}

		else{
			temp_buffer[position_buffer] = '1';
			position_buffer++;
		}

		copyTailleDico = copyTailleDico/2;

		i++;
	}

	for (int i=position_buffer; i > 0 ; i--) { buffer = (buffer<<1) + ( (temp_buffer[i-1] == '0') ? 0 : 1 ); }

	fputc(buffer, file_wrote);

	// On réinitialise le buffer
	buffer = 0;
	position_buffer = 0;

	// 2eme octect : le nombre bits sur lequel est codé le nombre de caractere total du fichier source
	int n = 1;
	while( puissance(2,n) < tailleFichierSource ) { n++; }
	
	int copy_n = n;

	while ( copy_n != 0 ){

		if ( copy_n%2 == 0 ){
			temp_buffer[position_buffer] = '0';
			position_buffer++;
		}

		else{
			temp_buffer[position_buffer] = '1';
			position_buffer++;
		}

		copy_n = copy_n/2;
	}

	for (int i=position_buffer; i > 0; i--) { buffer = (buffer << 1) + ( (temp_buffer[i-1] == '0') ? 0 : 1 ); }

	fputc(buffer, file_wrote);

	// On réinitialise le buffer et le buffer temporaire
	buffer = 0;
	position_buffer = 0;
	temp_buffer = calloc( 0 , 8*sizeof(char));

	// 3eme octect : le nombre de caractere total du fichier source
	copy_n = n;
	size_t copyTailleFichier = tailleFichierSource;
	long long int estPuissanceDe2 = copyTailleFichier;
	int plusProcheDiviseur = 0;
	int position_temp_buffer = 0;

	while( (n-plusProcheDiviseur)%8 != 0 ) { plusProcheDiviseur++; }

	if (copy_n > 7){
		
		while ( copy_n > 7 ){

			if ( position_buffer == 8 || (n-plusProcheDiviseur-1) == copy_n ){

				// On met les zéros devant si besoin
				for(int i=0 ; i < 8 ; i++){

					if ( i >= (8-position_buffer) ){
						buffer = (buffer<<1) + ( (temp_buffer[position_temp_buffer] == '0') ? 0 : 1 );
						position_temp_buffer++;
					}

					else{
						buffer = (buffer<<1);
					}
				}

				// On écrit le buffer
				fputc(buffer, file_wrote);
				position_buffer = 0;
				position_temp_buffer = 0;
			}
	

			estPuissanceDe2 = copyTailleFichier - puissance(2,(copy_n));

			if ( estPuissanceDe2 > 0 ){
				temp_buffer[position_buffer] = '1';
				copyTailleFichier = estPuissanceDe2;
			}


			else{
				temp_buffer[position_buffer] = '0';
			}
			position_buffer++;
			copy_n--;

		}

		// On rajoute des 0 si necessaires
		if ( position_buffer < 8){ for (int i=position_buffer; i < 8 ; i++) { temp_buffer[i] = '0';} }

		// On écrit le buffer
		for (int i=0; i < 8; i++) { buffer = (buffer << 1) + ( (temp_buffer[i] == '0') ? 0 : 1 ); }

		// On l'écrit dans le fichier compresser 
		fputc(buffer, file_wrote);
	}

	// on réinitialise
	buffer = 0;
	position_buffer = 0;

	while ( copyTailleFichier != 0 ){

		if ( copyTailleFichier%2 == 0 ){
			temp_buffer[position_buffer] = '0';
		}

		else{
			temp_buffer[position_buffer] = '1';
		}

		position_buffer++;
		copyTailleFichier = copyTailleFichier/2;

	}

	// On rajoute les 0 necessaires
	for (int i = position_buffer; i < 8; i++) { temp_buffer[i] = '0'; }

	// On écrit le buffer
	for (int i=position_buffer; i > 0; i--) { buffer = (buffer << 1) + ( (temp_buffer[i-1] == '0') ? 0 : 1 ); }

	// On l'écrit dans le fichier compresser 
	fputc(buffer, file_wrote);


	// On réinitialise le buffer
	buffer = 0;
	position_buffer = 0;

	// On écrit l'arbre
	size_t j = 0;
	while (  j != tailleDico )
	{

		int caractere = dico[j].caractere;
		int taille = dico[j].taille;

		// Le caractere
		while ( caractere != 0 ){

			if ( caractere%2 == 0 ){
				temp_buffer[position_buffer] = '0';
				position_buffer++;
			}

			else{
				temp_buffer[position_buffer] = '1';
				position_buffer++;
			}

			caractere = caractere/2;
		}

		for (int i=position_buffer; i > 0; i--)
		{
			buffer = (buffer << 1) + ( (temp_buffer[i-1] == '0') ? 0 : 1 );
			position_buffer--;
		}

		fputc(buffer, file_wrote);
		buffer = 0;
		position_buffer = 0;

		int copyTaille = taille;

		// La taille du code du caractere
		while ( copyTaille != 0 ){

			if ( copyTaille%2 == 0 ){
				temp_buffer[position_buffer] = '0';
				position_buffer++;
			}

			else{
				temp_buffer[position_buffer] = '1';
				position_buffer++;
			}

			copyTaille = copyTaille/2;
		}

		for (int i=position_buffer; i > 0; i--)
		{
			buffer = (buffer << 1) + ( (temp_buffer[i-1] == '0') ? 0 : 1 );
			position_buffer--;
		}

		fputc(buffer, file_wrote);
		buffer = 0;
		position_buffer = 8;

		int temp=0;

		// Le code du caractere
		while ( temp < taille ){

			if ( position_buffer == 0 ){
				fputc(buffer, file_wrote);
				position_buffer = 8;
				temp = 0;
			}

			buffer = (buffer<<1) + ( (dico[j].code[temp] == '0') ? 0 : 1 );
			position_buffer--;

			temp++;
		}

		fputc(buffer, file_wrote);
		buffer = 0;
		position_buffer = 0;

		j++;

	}

	buffer = 0;
	position_buffer = 8;

	// On récupère le caractere lu dans le fichier en lecture
	int carac = 0;

	// Tant qu'on est pas à la fin du fichier
	while( (carac = fgetc(file_read)) != EOF ){

		// On le cherche dans notre dictionnaire
		int j=0;
		while( carac != dico[j].caractere ){ j++; }

		// Tant que qu'on a pas fini de copier le code du caractere et qu'on est pas à la fin du fichier source
		int k=0;
		while( k < dico[j].taille ){

			// Quand on arrive a la fin du buffer on le réinitialise
			if ( position_buffer == 0 ){
				fputc(buffer, file_wrote);
				position_buffer = 8;
			}

			// On met un nouveau bit a la fin de notre buffer
			buffer = (buffer << 1) + ( (dico[j].code[k] == '0') ? 0 : 1 );
			position_buffer--;

			k++;
		}
	}

	// Tous les caractères ont été encodés, on ajoute des 0 a la fin de notre buffer pour finalisé
	while ( position_buffer != 0 ){
		buffer = (buffer << 1);
		position_buffer--;
	}

	fputc(buffer, file_wrote);

	// On récupère la taille du fichier créer
	size_t taillFichierOutput = ftell(file_wrote);

	// Petit message de succès qui fait jolie
	printf("\033[1;42m[Success]\033[0m : Le fichier a été compréssé avec succès\n");

	// On calcul la perte ou le gain
	if ( tailleFichierSource < taillFichierOutput ){ printf("\n\033[1;41m[Perte]\033[0m : Taille du fichier source = %ld octet(s) | Taille du fichier compréssé = %ld octet(s) | perte = %.2f%%\n",tailleFichierSource,taillFichierOutput, ( (float) tailleFichierSource / (float) taillFichierOutput )*100);}
	else { printf("\n\033[1;42m[Gain]\033[0m : Taille du fichier source = %ld octet(s) | Taille du fichier compréssé = %ld octet(s) | gain = %.2f%%\n",tailleFichierSource,taillFichierOutput, 100 - ( (float) taillFichierOutput / (float) tailleFichierSource )*100);}

	// On ferme les fichiers
	fclose(file_read); 
	fclose(file_wrote);
}