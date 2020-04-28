#ifndef _DISTRI_H
#define _DISTRI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * NMAX : Nombre max pour l'affichage de la table ascii
 * NMAX <= 127 => Affichage basique de la table ascii
 * NMAX <= 255 => Affichage de la table basique et extend 
 * /!\ Risque de bug d'affichage des caractères pour la table extend /!\
 * /!\ TRUC BIZARRE PLUSIEURS LETTRE SONT CONSIDERER A LA FOIS SEUL ET EN GROUPE /!\
*/
#define NMAX 256

/*
 * Structure : tab_dist
 * Typedef struct tab_dist : alias d'appeler plus facilement la table struct
 * @attribut : 
 *   caractere : Une lettre
 *   nboccurence : entier, nombre d'occurence d'une lettre
 *   probabilite : flottant, taux d'apparition d'une lettre
 *
 * @description : Lit un fichier donner dans la sortie standard
 *
*/
typedef struct tab_dist tab_dist;
struct tab_dist
{ 
  int caractere;
  int nboccurence;
  double probabilite;
};

/*
 * Fonction : readFile
 * @param : argv[] : Tableau de charactere; 
 * @description : Lit un le premier nom donner dans la sortie standard  (./huf [ICI] [...])
 *          et renvoie sa taille;
 *
*/
size_t readFile(char* argv[]);

/*
 * Fonction : initTab;
 * @param: file : Un fichier;
 *       tailleFichier : La taille d'un fichier;
 *
 * @description : Initialise un tableau 
 *
*/
tab_dist* initTab(FILE *file, const int tailleFichier);

/*
 * Fonction : getSizeTab;
 * @param : tab : Tableau de structure; 
 * @description : Renvoi la taille d'un tableau;
 *
*/
int getSizeTab(const tab_dist* tab);

/*
 * Fonction : getMinProb;
 * @param : tab : Tableau de structure; 
 * @description : Renvoi la probabilité minimum du tableau;
 *
*/
float getMinProb(tab_dist* tab);

/*
 * Fonction : triTab;
 * @param : tab : Tableau de structure; 
 * @description : Renvoi le tableau donnée en paramètre trié
 *          dans l'ordre croissant;
*/
tab_dist* triTab(tab_dist* tab);

/*
 * Fonction : printTab
 * @param : tab : Tableau de structure; 
 * @description : Affiche sur la sortie standard le contenu du tableau
 *
*/
void printTab(const tab_dist* tab);

#endif