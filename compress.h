#ifndef _COMPRESS_H
#define _COMPRESS_H

/*
 * Fonction : puissance;
 * @param : x,n : entier; 
 * @description : Renvoi x à la puissance n;
 *
*/
int puissance(int x, int n);

/*
 * Fonction : compress;
 * @param : argv[]: tableau de caractere;
 *		  : dico: dictionnaire de structure;
 *		  : tailleFichierSource; entier 
 * @description : Renvoi le fichier donnée en paramètre compressé;
 *
*/
void compress(char* argv[], dictionnaire* dico, size_t tailleFichierSource);

#endif