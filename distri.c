#include "distri.h"


size_t readFile(char* argv[]){

	size_t size;

	FILE * file; /*Crée la variable file comme fichier*/
	file = fopen( argv[1] , "r" ); /*"Ouvre" le fichier en mode lecture*/
	
	if( file == NULL ) /*Si fp n'existe pas => erreur*/
	{ 
		perror("\033[1;41m[Error]\033[0m : Vérifier si vous avez bien écrit le nom de votre fichier\n");
		exit(0);
	} 

	fseek(file, 0L, SEEK_END); /* Dirige le pointeur a la fin du fichier*/
	
	size = ftell(file); /*Trouve la taille du fichier en prenant tous les 
						 *caractère avant le pointeur */

	fclose(file); /* on ferme le fichier quand tout est fini */
	
	return size;
}

tab_dist* initTab(FILE* file, const int tailleFichier){

	int temp[NMAX] = {0}; // création d'un tableau de la taille du nombre de charactère ascii possible et initialisé à 0;

	fseek(file, 0L, SEEK_SET); // On met le curseur au début du fichier;

	for(int i=0; i<tailleFichier; i++) { //Pour i de 0 a taille du fichier - 1 faire;
		int caractere = (int) fgetc(file); // on prend le code ascii du caractere devant le curseur;
		temp[caractere]++; // On incrémente de 1 si on trouve cette lettre;
	}

	/* On supprime chaque lettre dont l'occurence est nul (=0) et on renvoie la taille final du tableau; */
	size_t sizeTab=0,k=0;
	while (k < NMAX) {
		if( temp[k] != 0 ){sizeTab++;}
		k++;
	}

	/* On crée une table de probabilité dynamique et son allocation mémoire */
	tab_dist* probTab;
	probTab = (tab_dist*) malloc(sizeTab*sizeof(tab_dist));

	/* initialise chaque élément dans le tableau */
	int x=0;
	for (int i=0; i < NMAX; i++){
		if( temp[i] != 0 ){
			probTab[x].caractere = i; /* ligne 1 --> lettre */
			probTab[x].nboccurence = temp[i];/* ligne 2 --> nboccurence */
			probTab[x].probabilite = ( (float) temp[i] / (float) tailleFichier )*100;/* ligne 3 --> proba */
			x++;
		}
	}

	return(probTab);
}

int getSizeTab(const tab_dist* tab){
	int sizeTab = 0; 
	int i = 0;
	while( tab[i].caractere != '\0'){
		sizeTab++;
		i++;
	}
	return sizeTab;
}

float getMinProb(tab_dist* tab){
	tab_dist* temp = triTab(tab);	
	float res = temp[0].probabilite;

	//free(temp);
	return res;
}


tab_dist* triTab(tab_dist* tab){

	tab_dist* tempTab = (tab_dist*) malloc(getSizeTab(tab)*sizeof(tab_dist));
	int tailleTab = getSizeTab(tab);
	for(int i=0; i < tailleTab; i++){
		memset( tempTab, 0, sizeof(tab_dist)*tailleTab);
		for (int j=0; j < tailleTab-1; j++){
			if(tab[i].nboccurence < tab[j].nboccurence ){ // si T[i] > T[i+1]
				tempTab[i] = tab[i];
				tab[i] = tab[j]; // on copie la case précédente
				tab[j] = tempTab[i]; // on copie la case suivante
			}
		}
	}

	//free(tempTab);
	return tab;
}


void printTab(const tab_dist* tab){
	printf("\n===============================================================\n");
	printf("=                   distribution des probabilité              =");
	printf("\n===============================================================\n\n");
	size_t sizeTab = getSizeTab(tab);
	for(int i=0; i < sizeTab; i++)
	{
		if ( tab[i].caractere == 9 ){ printf("[ TAB ] --> Occurence : %d | probabilité = [ %.2f%% ]\n", tab[i].nboccurence, tab[i].probabilite); }
		else if ( tab[i].caractere == 10 ){ printf("[ LF ] --> Occurence : %d | probabilité = [ %.2f%% ]\n", tab[i].nboccurence, tab[i].probabilite); }
		else if ( tab[i].caractere == 13 ){ printf("[ CR ] --> Occurence : %d | probabilité = [ %.2f%% ]\n", tab[i].nboccurence, tab[i].probabilite); }
		else if ( tab[i].caractere == 32 ){ printf("[ SP ] --> Occurence : %d | probabilité = [ %.2f%% ]\n", tab[i].nboccurence, tab[i].probabilite); }
		else { printf("[ %c ] --> Occurence : %d | probabilité = [ %.2f%% ]\n", tab[i].caractere, tab[i].nboccurence, tab[i].probabilite); }
	}
	printf("\n");
}