#include "arbre.h"
#include "distri.h"

noeud* creeArbre(const tab_dist* tab){

	/* On garde la taille du tableau dans un coin */
	int tailleTab = getSizeTab(tab);

	/*
	 * Si un seul caractère dans le fichier
	 * 	Plus petite allocation mémoire possible pour cette unique caractère
	 * 	On met ce caractère à gauche
	 * 	On définit le père et le fils droit a -1
	 *	La probabilité du noeud est de 100%
	 * 	On arrête la fonciton en renvoyant l'arbre
	*/
	if ( tailleTab == 1 )
	{
		noeud* arbre = (noeud*) malloc(sizeof(arbre));
		arbre[0].pere = -1;
		arbre[0].fils_gauche = tab[0].caractere;
		arbre[0].fils_droit = -1;
		arbre[0].probabilite = 100.00;
		return arbre;
	}

	noeud* arbre = (noeud*) malloc( ( (tailleTab*2)-1)*sizeof(arbre));

	/* initialise le tableau proprement :
	* pere        = -1 => pas de père
	* fils_gauche = -1 => pas de fils gauche
	* fils_droit  = -1 => pas de fils droit
	* probabilite = 0.0
	*/
	for (int i=0; i < tailleTab-1; i++){
		arbre[i].pere = -1; // n'a pas de père
		arbre[i].fils_gauche = -1; // n'a pas de fils gauche
		arbre[i].fils_droit = -1; // n'a pas de fils droit
		arbre[i].probabilite = 0.0; // probabilité respective
	}
	
	/*
	 * j		   : entier, compteur permettant l'acces 1 a 1 des arbres, initialiser a 1;
	 * numMinArbre : entier, renvoie l'arbre possédant la plus petit probabilité, initialiser a 0;
	 * pbMinArbre  : flottant, renvoie l'arbre possédant la plus petite probabilité, initialiser a 0.0;
	*/
	int j=1, numMinArbre=0; float pbMinArbre = 0.0;

	/* on parcours le tableau de probabilité */
	for (int i=0; i < tailleTab; i++){

		/* 
		 * Le tableau étant triée on peut définir le premier élément
		 * A la fin du if, on incrémente "i" afin d'éviter de réévaluer la case 1 du tableau
		 * vu qu'elle vient d'être définit dans un noeud.
		 *					 @
		 * |0|1|2|...| --> /   \ 
		 *				tab[0] tab[1]
		*/
		if( i == 0 ){
			arbre[i].fils_gauche = tab[i].caractere;
			arbre[i].fils_droit = tab[i+1].caractere;
			arbre[i].probabilite = tab[i].probabilite+tab[i+1].probabilite;
			pbMinArbre = arbre[0].probabilite;
			i++;
		}

		/* Sinon */
		else{

			/*
			 * numNoeudPointe: entier, renvoie le numéro du noeud correspondant au noeud possédant 
			 * 				   la plus petite moyenne, on y ajoute 257 afin d'être plus grand que
			 *				   la table ascii étendue et de pouvoir différencier le noeud d'un
			 *				   caractere
			 * proba1: flottant, somme entre la probabilité minimum de l'arbre et celle du caractere
			 * proba2: flottant, initialiser a 0.0;
			 * @condition :
			 *	si i != taille du tableau - 1:
			 *	 |	proba 2 = somme entre probabilite du caractere et celle du caractere suivant
			 *	sinon:
			 *	 |	proba2 = proba1
			 *  fin si
			*/
			int numNoeudPointe = numMinArbre + 257;
			float proba1 = pbMinArbre + tab[i].probabilite;				
			float proba2 = 0.0;
			if(i != tailleTab-1){ proba2 = tab[i].probabilite + tab[i+1].probabilite; }
			else{ proba2 = proba1; }

			/* 
			 * Si pas de père et proba1 > proba2 alors :
			 * 	On crée un nouveau noeud composé de deux caractères 
			 * 	On incrémente i pour sauter l'élement suivant du tableau
			*/

			if ( arbre[j].pere == -1 && proba1 > proba2 ){

				arbre[j].fils_gauche = tab[i].caractere;
				arbre[j].fils_droit	= tab[i+1].caractere;
				arbre[j].probabilite = proba2;
				i++;


				/* 
				 * Si il reste trois éléments dans le tableau alors il reste un noeud et la racine
				 *  | On crée donc un nouveau noeud entre l'unique noeud droit et le noeud gauche ayant comme père la racine
				 * fin si	
				 * ex : 
				 *			  					      r 		
				 *		  	 	  			 	    /   \
				 *		 @		  				   @	 @	
				 *	    / \		 		 ====>    / \	/ \
				 * 	   @   c    d   e 		     @	 c d   e
				 *	  / \					    / \
				 *	 a	 b					   a   b
				*/
				if ( j >= tailleTab-2 )
				{
					
					j++;
					numNoeudPointe = numMinArbre + 257 ;
					arbre[numMinArbre].pere = j;
					arbre[numMinArbre+1].pere = j;
					arbre[j].fils_droit = numNoeudPointe+1;
					arbre[j].fils_gauche = numNoeudPointe;
					arbre[j].probabilite = arbre[numMinArbre].probabilite + arbre[numMinArbre+1].probabilite;
				}

			}

			/* 
			 * Si i != (taille du tableau - 1) :
			 *  | Le noeud possédant la plus petite probabilité a donc un père
			 * 	| Le fils gauche est le noeud de plus petite probabilité
			 * 	| Le fils droit est le caractere
			 * 	| On incrémente le numMinArbre car la probabilité de l'ancien arbre est 
			 * 	| dorénavant plus grand ou égal
			 *	| La probabilité minimum de l'arbre est maintenant celle du nouvelle arbre minimum
			 * fin si
			 * Exemple :
			 * Avant : numMinArbre = 10%				Après : numMinArbre = 14%
			 * Caractere étudier -> e = 12%				Caractere étudier -> ...
			 *	 
			 *  						   	   						22%
			 * 									====>     		   /   \
			 *   	   10%      14%  		     				 10%    e     14%
			 *  	  /   \    /   \			   				/   \	  	 /   \
			 * 	  	 a     b  c     d    e  		  		   a 	 b      c 	  d
			 *
			*/
			else if ( i != tailleTab-1 ){
				arbre[numMinArbre].pere = j;
				arbre[j].fils_gauche = numNoeudPointe;
				arbre[j].fils_droit = tab[i].caractere;
				arbre[j].probabilite = proba1;
				numMinArbre++;
				pbMinArbre = arbre[numMinArbre].probabilite;
			}


			/*
			 * Sinon on a atteind le dernier élément du tableau on peut construire la racine, on
			 * est donc a la fin de l'itération de notre boucle :
			 *	Dans un premier temps on crée le noeud entre le dernier élément du tableau de
			 *	probabilité et du plus petit noeud;
			 *	On incrémente le numero minimum de l'arbre;
			 * 	On redéfinis le numero du noeud pointe comme étant le numero minimum de l'arbre;
			 *	 Si il y a au moins un élement qui n'a pas de père alors il reste des noeuds a attachés:
			 *	 |	on incrémente j pour acceder a la case du tableau suivante;
			 *	 |	tant qu'on est pas la fin du tableau:
			 *	 |	 |	Le noeud minimum de l'arbre et le noeud minimum suivant on le même père
			 *	 |	 |	Le fils gauche est le numero du noeud pointe;
			 *	 |	 |	Le fils droit est le numero suivant du noeud pointe;
			 *	 |	 |	La probabilité est l'addition de la probabilité de ses deux noeuds;
			 *	 |	 |	On incrémente de deux le numero minimum de l'arbre;
			 *	 |	 |	On redéfinit le prochain numero du noeud;
			 *	 |	 |	on incrémente j;
			 *	 |	fin tant que
			 *	fin si
			*/

			else{
				arbre[numMinArbre].pere = j;
				arbre[j].fils_gauche = numNoeudPointe;
				arbre[j].fils_droit	= tab[i].caractere;
				arbre[j].probabilite = proba1;

				numMinArbre++;
				numNoeudPointe = numMinArbre + 257 ;

				if( (j - numMinArbre) >= 1 ){
					j++;
					while( j < tailleTab-1 ){
						arbre[numMinArbre].pere = j;
						arbre[numMinArbre+1].pere = j;
						arbre[j].fils_gauche = numNoeudPointe;
						arbre[j].fils_droit = numNoeudPointe+1;
						arbre[j].probabilite = arbre[numMinArbre].probabilite + arbre[numMinArbre+1].probabilite;
						numMinArbre+=2;
						numNoeudPointe = numMinArbre + 257 ;
						j++;
					}
				}
			}

			j++;
		}
	}

	return arbre;
}

dictionnaire* parcoursArbre(noeud* arbre, dictionnaire* dico, int positionDico, char* code, int positionCode, int index, int tailleTab){

	/*
	 * nouveauNoeud_0: entier; permet de conserver dans une variable différente d'index lors de l'appel récursif;
	 * nouveauNoeud_1: entier; permet de conserver dans une variable différente d'index lors de l'appel récursif;
	 * positionCode_0: entier; permet de conserver dans une variable différente la position ou écrire dans le 0;
	 * positionCode_1: entier; permet de conserver dans une variable différente la position ou écrire dans le 1;
	 * Rq : j'aurai pu utiliser seulement deux variables mais pour être plus lisible et ne coûtant pas trop cher
	 * j'ai préféré créer deux variables de plus
	*/
	int nouveauNoeud_0 = index;
	int nouveauNoeud_1 = index;
	int positionCode_0 = positionCode;
	int positionCode_1 = positionCode;

	// A chaque itération on recopie le code du noeud précedent dans le dictionnaire pointe par positionDico
	copyPreviousCode(dico, positionDico, code, positionCode);

	/* si j'ai deux feuilles alors 
		| si j'ai un seul caractere alors
		|  | On récupère le caractere;
		|  | On lui affilie le code 0;
		|  | On incrémente la taille de 1;
		| sinon on a au moins 2 caractere donc
		|  | On récupère le caractere de gauche et on rajoute un 0;
		|  | On incrémente la taille puis positionDico car on vient d'ajouter un caractere;
		|  | On copie dans la nouvelle case du dico le code du noeud précédent;
		|  | On récupère le caractere de droite et on rajoute un 1;
		|  | On incrémente positionDico car on vient d'ajouter un caractere;
		| fin si
		| On renvoie le dictionnaire;
	   fin si
	*/
	if ( arbre[index].fils_gauche < 256 && arbre[index].fils_droit < 256 )
	{

		if ( tailleTab == 1 ){
			dico[positionDico].caractere = arbre[0].fils_gauche;
			dico[positionDico].code[positionCode] = '0';
			dico[positionDico].taille = positionCode + 1;
		}

		else{
			dico[positionDico].caractere = arbre[index].fils_gauche;
			dico[positionDico].code[positionCode] = '0';
			dico[positionDico].taille = positionCode + 1;

			positionDico++;

			copyPreviousCode(dico, positionDico, code, positionCode);

			dico[positionDico].caractere = arbre[index].fils_droit;
			dico[positionDico].code[positionCode] = '1';
			dico[positionDico].taille = positionCode + 1;

			positionDico++;
		}

		return dico;
	}

	/* si j'ai noeud a gauche (droite) faire :
		| (on récupère la position du dico;f)
		| si j'ai caractere a droite (gauche) faire :
		|  | On récupère le caractere et on lui rajoute un 1 (0);
		|  | On incrémente la taille de 1;
		|  | On incrémente la position dico;
		| fin si
		| On définis nouveauNoeud_0 (nouveauNoeud_1) comme étant le noeud de gauche (droite);
		| On rajoute un 0 (1) au code;
		| On incrémente positionCode_0 (positionCode_1);
		| Appel recursif pour parcourir l'arbre du nouveau noeud;
	   fin si
	   On renvoie le dictionnaire;
	*/
	if ( arbre[nouveauNoeud_0].fils_gauche > 256 )
	{

		if( arbre[nouveauNoeud_0].fils_droit < 256 )
		{
			dico[positionDico].caractere = arbre[nouveauNoeud_0].fils_droit;
			dico[positionDico].code[positionCode_0] = '1';
			dico[positionDico].taille = positionCode + 1;
			positionDico++;
		}

		nouveauNoeud_0 = arbre[nouveauNoeud_0].fils_gauche - 257;
		code[positionCode_0] = '0';
		positionCode_0++;
		parcoursArbre(arbre, dico, positionDico, code, positionCode_0, nouveauNoeud_0, tailleTab);
	}

	if( arbre[nouveauNoeud_1].fils_droit > 256 )
	{

		positionDico = getSizeDico(dico);

		if( arbre[nouveauNoeud_1].fils_gauche < 256 )
		{
			dico[positionDico].caractere = arbre[nouveauNoeud_1].fils_gauche;
			dico[positionDico].code[positionCode_1] = '0';
			dico[positionDico].taille = positionCode + 1;
			positionDico++;
		}

		nouveauNoeud_1 = arbre[nouveauNoeud_1].fils_droit - 257;
		code[positionCode_1] = '1';
		positionCode_1++;
		parcoursArbre(arbre, dico, positionDico, code, positionCode_1, nouveauNoeud_1, tailleTab);

	}

	return dico;

}


int getSizeArbre(const noeud* arbre){

	int i=0;

	while( arbre[i].pere != -1) { i++; }
	return i+=1;
}

int getSizeDico(const dictionnaire* dico){

	int i = 0;

	while( dico[i].taille != 0 ) {i++;}
	return i;
}

void copyPreviousCode(dictionnaire* dico, int positionDico, char* code, int positionCode){

	int i=0;

	/*
	 * Tant que i est strictement inférieur a la position du Code faire :
	 * 	| La code de la case du dictionnaire prend la valeur de code à chaque itération;
	 *  | On incrémente i;
	 * fint tant que
	*/
	while( i < positionCode ){ dico[positionDico].code[i] = code[i]; i++; }
}

void printArbre(const noeud* arbre){
	printf("\n\n===============================================================\n");
	printf("=                 création de l'arbre d'Huffman               =\n");
	printf("===============================================================\n\n");
	printf("NUM_ARBRE  PERE         FILS_GAUCHE  FILS_DROIT   PROBABILITE");
	printf("\n---------- ------------ ------------ ------------ ------------\n");
	
	int tailleArbre = getSizeArbre(arbre);

	for(int i=0; i < tailleArbre; i++){
		printf("%d          ",i );

		if( arbre[i].pere != -1 ){ printf("[Arbre#%d]    ", arbre[i].pere ); }
		else{ printf("racine       "); }

		if ( arbre[i].fils_gauche > 256 ){ int temp = arbre[i].fils_gauche - 257; printf("[Arbre#%d]    ", temp); }
		else{
			if ( arbre[i].fils_gauche == 9 ){ printf("[TAB]         "); }
			else if ( arbre[i].fils_gauche == 10 ){ printf("[LF]          "); }
			else if ( arbre[i].fils_gauche == 13 ){ printf("[CR]          "); }
			else if ( arbre[i].fils_gauche == 32 ){ printf("[SP]          "); }
			else{ printf("[%c]          ",arbre[i].fils_gauche );}
		}

		if ( arbre[i].fils_droit > 256 ){ int temp2 = arbre[i].fils_droit - 257; printf("[Arbre#%d]    ", temp2); }
		else {
			if ( arbre[i].fils_droit == 9 ){ printf("[TAB]         "); }
			else if ( arbre[i].fils_droit == 10 ){ printf("[LF]          "); }
			else if ( arbre[i].fils_droit == 13 ){ printf("[CR]          "); }
			else if ( arbre[i].fils_droit == 32 ){ printf("[SP]          "); }
			else if( arbre[i].fils_droit == -1 ) { printf("[NULL]        "); }
			else{ printf("[%c]          ",arbre[i].fils_droit ); }
		}
		printf("[%.2f%%]\n",arbre[i].probabilite);
		printf("---------- ------------ ------------ ------------ ------------\n");
	}
	printf("\n");
}

void printDico(const dictionnaire* dico){

	int tailleDico = getSizeDico(dico);
	size_t longueurTotal = 0;
	printf("\n===============================================================\n");
	printf("=                      codage de l'arbre                      =\n");
	printf("===============================================================\n\n");
	for (int i = 0; i < tailleDico; i++){
		if ( dico[i].caractere == 9 ){ printf("caractere [ TAB ] --> code : [ %s ] \n", dico[i].code ); }
		else if ( dico[i].caractere == 10 ){ printf("caractere [ LF ] --> code : [ %s ] \n", dico[i].code ); }
		else if ( dico[i].caractere == 13 ){ printf("caractere [ CR ] --> code : [ %s ] \n", dico[i].code ); }
		else if ( dico[i].caractere == 32 ){ printf("caractere [ SP ] --> code : [ %s ] \n", dico[i].code );}
		else { printf("caractere [ %c ] --> code : [ %s ] \n",dico[i].caractere, dico[i].code); }
		longueurTotal += dico[i].taille;
	}
	
	printf("\n\033[48;5;128m  \033[0m Longueur moyenne de codage = %.2f\n", (float) longueurTotal / (float) tailleDico );
	printf("\n");
}