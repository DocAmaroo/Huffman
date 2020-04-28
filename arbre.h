#ifndef _ARBRE_H
#define _ARBRE_H

#include "distri.h"

/*
 * Structure : noeud
 * Typedef struct noeud : alias permettant d'appeler plus facilement la structure
 * @attribut : 
 *	 pere : entier, pere du fils gauche et droit
 *	 fils_gauche : entier, fils gauche
 *	 fils_droit : entier, fils droit
 *
 * @description : Structure définissant un sous arbre reliant un pere à ses deux fils
 *
*/
typedef struct noeud noeud;
struct noeud
{	
	int pere; 		   /* Parent du sous arbre composé d'un fils gauche et droit */
	int fils_gauche;   /* Le fils gauche */
	int fils_droit;	   /* Le fils droit */
	float probabilite; /* probabilite */
};

/*
 * Structure : dictionnaire
 * Typedef struct dictionnaire : alias permettant d'appeler plus facilement la structure
 * @attribut : 
 *	 caractere : entier, le caractere associé à l'arbre
 *	 code : tableau de caractere, code du caractere
 * @description : Structure définissant un dictionnaire composé d'un caractere et de son code binaire
 *
*/
typedef struct dictionnaire dictionnaire;
struct dictionnaire
{	
	int caractere;
	int taille;
	char* code;
};

/*
 * Fonction : creeArbre
 * @param : tab : Tableau de probabilite; 
 * @description : Crée l'arbre d'Huffman;
 *
*/
noeud* creeArbre(const tab_dist* tab);

/*
 * Fonction : parcoursArbre
 * @param : arbre: noeud, arbre de noeud;
 *		    dico: dictionnaire, dictionnaire contenant le caractere et le code de chaque branche de l'arbre;
 * 			positionDico: entier, position du dictionnaire
 *			code: tableau de caractere, renvoi le code au fur est à mesure des différents noeud
 			positionCode : entier, position du code;
 			index : entier, numéro du noeud pointé;
 			tailleTab : entier, taille du tableau de probabilité;
 * @description : Crée le code de chaque caractère de l'arbre
 *
*/
dictionnaire* parcoursArbre(noeud* arbre, dictionnaire* dico, int positionDico, char* code, int positionCode, int index, int tailleTab);

/*
 * Fonction : getSizeArbre
 * @param : arbre : arbre de noeud; 
 * @description : Renvoi la taille de l'arbre 
 *
*/
int getSizeArbre(const noeud* arbre);

/*
 * Fonction : getSizeDico
 * @param : dico : dictionnaire; 
 * @description : Renvoi la taille du dictionnaire
 *
*/
int getSizeDico(const dictionnaire* dico);

/*
 * Fonction : copyPreviousCode
 * @param : dico : dictionnaire;
 			positionDico : entier; position du dictionnaire;
 			code : tableau de caractere; le code à copier;
			positionCode : entier; position du code;
 * @description : Copie le code donnée en parametre dans la position du dictionnaire donnée
 *
*/
void copyPreviousCode(dictionnaire* dico, int positionDico, char* code, int positionCode);

/*
 * Fonction : printArbre
 * @param : tab : Noeud de structure; 
 * @description : Affiche sur la sortie standard l'arbre d'Huffman
 *
*/
void printArbre(const noeud* arbre);

/*
 * Fonction : printDico
 * @param : dico : dictionnaire; 
 * @description : Affiche sur la sortie standard le code binaire de chaque caractere de l'arbre d'Huffman
 *
*/
void printDico(const dictionnaire* dico);

#endif