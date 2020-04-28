#include <string.h>

#include "distri.h"
#include "arbre.h"
#include "compress.h"
#include "decompress.h"

/*
 * Fonction : estDansLesOptions
 *
 * @param:
 *	nb_options: entier, nombre  d'options qui ont été rentrés dans la sortie standard;
 *	options : chaine de caractère, options dont la présence est à vérifier;
 *	argv[] : tableau de chaine de caracteres; 
 *
 * @description: Permet de vérifié si une option donnée a été donnée dans la sortie standard;
 *
*/
int estDansLesOptions(int nb_options, char* options, char* argv[]){

	int i=0;
	while( i < nb_options ){
		if ( strcmp(argv[i+1], options) == 0 ){
			return 0;
		}
		i++;
	}

	return 1;

}

int main(int argc, char* argv[]) {

	system("clear");
	/***************************************************/
	/****************** VERIFICATION *******************/
	/***************************************************/

	int nb_options = 0;
	while( argc-1 > nb_options ){ nb_options++; }

	// On cherche les options

	if ( ( estDansLesOptions(nb_options,"-c",argv) == 0 || estDansLesOptions(nb_options,"-compress",argv) == 0 ) && ( estDansLesOptions(nb_options,"-d",argv) == 0 || estDansLesOptions(nb_options,"-decompress",argv) == 0 ) ){
		for (int i = 0; i < 10; i++) { printf("\033[48;5;160m                                                                                                                                    \033[0m\n"); }
		printf("\033[48;5;160m                                                     MAIS VOUS ETES FOU MESSIRES !                                                  \033[0m\n");
		for (int i = 0; i < 10; i++) { printf("\033[48;5;160m                                                                                                                                    \033[0m\n");}
		printf("//                     Cette erreur est dû à la tentative de compression et de décompression en une seule fois                    //\n");
		printf("//               Pour vous évitez de sombrer dans le Segmentation Fault (core dump), je vous laisse la vie sauve !                //\n");
		return 1;
	}
	// Chaque option commence par un '-'
	if ( estDansLesOptions(nb_options,"-h",argv) == 0 || estDansLesOptions(nb_options,"-help",argv) == 0 || (argc < 4)){ 
		printf("\033[48;5;160m                                 \n");
		printf("           \033[48;5;232m[   HELP   ]\033[48;5;160m          \n");
		printf("                                 \033[0m\n\n");
		printf("\033[48;5;026m  \033[0m Aide : ./huf < -h | -help >\n\n");
		printf("\033[48;5;161m                                 \n");
		printf("       \033[0;39m[   COMPILATION   ]\033[48;5;161m       \n");
		printf("                                 \033[0m\n\n");
		printf("\033[48;5;205m  \033[0m Compilation : make\n");
		printf("\033[48;5;231m  \033[0m Clean       : make clean\n\n");
		printf("\033[48;5;202m                                 \n");
		printf("          \033[0;39m[   USAGE   ]\033[48;5;202m          \n");
		printf("                                 \033[0m\n\n");
		printf("\033[48;5;127m  \033[0m Tableau       : ./huf fichier_source fichier_sortie < -tab | -table >\n");
		printf("\033[48;5;033m  \033[0m Arbre         : ./huf fichier_source fichier_sortie < -tr | -tree >\n");
		printf("\033[48;5;010m  \033[0m Code          : ./huf fichier_source fichier_sortie < -co | -code >\n");
		printf("\033[48;5;041m  \033[0m Compression   : ./huf fichier_source fichier_sortie < -c | -compress >\n");
		printf("\033[48;5;049m  \033[0m Décompression : ./huf fichier_source fichier_sortie < -d | -decompress >\n\n");

		return 1;
	}


	/***************************************************/
	/************* TABLEAU DE PROBABILITE **************/
	/***************************************************/

	// On vérifie si le fichier est vide, si oui on renvoie une erreur et on arrête le programme
	size_t tailleFichier = readFile(argv);
	if( tailleFichier == 0 ) { fprintf(stderr,"\033[1;41m[Error]\033[0m Votre fichier est vide...\n"); return 1;}

	// Sinon on initialise un tableau de probabilité
	FILE* file = fopen(argv[1], "r");
	tab_dist* tableau_de_probabilite = initTab(file,tailleFichier);

	// On tri le tableau
	tab_dist* tabTriee = triTab(tableau_de_probabilite);

	if ( estDansLesOptions(nb_options,"-tab",argv) == 0 || estDansLesOptions(nb_options,"-table",argv) == 0 ){
		printTab(tabTriee);
	}
	// On crée l'arbre
	noeud* arbre = creeArbre(tabTriee);

	if ( estDansLesOptions(nb_options,"-tr",argv) == 0 || estDansLesOptions(nb_options,"-tree",argv) == 0 ){
		printArbre(arbre);
	}

	if ( estDansLesOptions(nb_options,"-co",argv) == 0 || estDansLesOptions(nb_options,"-code",argv) == 0 || estDansLesOptions(nb_options,"-c",argv) == 0 || estDansLesOptions(nb_options,"-compression",argv) == 0 ){

		/***************************************************/
		/********************* CODAGE **********************/
		/***************************************************/

		// On récupère les tailles respectives du tableau et de l'arbre dans une variable pour éviter les rappels de fonctions
		int tailleTab = getSizeTab(tabTriee);
		int tailleArbre = getSizeArbre(arbre);

		// Initialisation du dictionnaire
		dictionnaire* dico = malloc( tailleTab*sizeof(dictionnaire) );
		for(int i=0; i < tailleTab; i++){
			dico[i].caractere = '\0';
			dico[i].taille = 0;
			dico[i].code = calloc( 0, ( (tailleArbre/2)-1 )*sizeof(dictionnaire) );
		}

		// On définit les variables pour l'appel de la fonction parcoursArbre
		int positionDico = 0;
		char* code = calloc( 0, ( (tailleArbre/2)-1 )*sizeof(char) );
		int positionCode = 0;
		int racine = tailleArbre-1;

		// écriture du code binaire
		parcoursArbre(arbre, dico, positionDico, code, positionCode, racine, tailleTab);

		if ( estDansLesOptions(nb_options,"-co",argv) == 0 || estDansLesOptions(nb_options,"-code",argv) == 0 ){
			printDico(dico); 
		}

		if ( estDansLesOptions(nb_options,"-c",argv) == 0 || estDansLesOptions(nb_options,"-compress",argv) == 0 ){

			/***************************************************/
			/******************* COMPRESSION *******************/
			/***************************************************/

			// On compresse le fichier
			compress(argv, dico, tailleFichier);
		}

		//On libère l'espace mémoire
		free(dico);
		free(arbre);
		free(tabTriee);
		free(tableau_de_probabilite); 
	}


	if ( estDansLesOptions(nb_options,"-d",argv) == 0 || estDansLesOptions(nb_options,"-decompress",argv) == 0 ){

		/***************************************************/
		/****************** DECOMPRESSION ******************/
		/***************************************************/

		// On decompresse le fichier
		decompress(argv);
	}

	return 0;
}
