/* @date 2 fevrier 2023
 * @author Jeremie Hamel
 * @file main.c du lab 2
 * @brief Ce programme simule la porte du local 283 dans la fonctione d'une porte 
 *        securis�e par code. l'utilisateur doit enter son num�ro d'�tudiant puis
 *        entrer son mot de passe. Dans la cas ou le mot de passe correspond au
 *        num�ro d'�tudiant un message d'acces accept� apparait a l'�cran. Dans 
 *        le cas contraire un message d'acces refus� apparait.
 */

#include <xc.h> //pour l'utilisation du compilateur XC8
#include <stdbool.h> //pour utiliser des variables de type bool
#include <stdlib.h>
#include <string.h> //pour strcmp
#include "initialisation.h"  //pour l'initialisations des ports du pic
#include "Lcd4Lignes.h" //pour l'utilisation de l'afficheur lcd
#include "serie_458.h"	 //pour la communication s�rie entre le pic et l'ordinateur

/******Constantes *******/
#define _XTAL_FREQ 16000000 //Constante utilis�e par __delay_ms(x). Doit = freq interne du uC
#define NB_CAR_NOM 20   //nom d'usager a 20 caracteres max
#define NB_NIP 4  //les nip on 4 car max 
#define NB_USAGER 12  //nombre d'usagers
#define DEL_R PORTDbits.RD7  //DEL rouge
#define DEL_J PORTCbits.RC0  //DEL jaune
#define ENTER 0x0D //touche Enter
#define ESC 0x1B   //touche escape
#define BCKSPC 0x08 //touche backspace

//  Connstantes des �tats possibles de la machine � �tat
enum
{
    enumEtat_Depart   = 0,
    enumEtat_NoUsager ,
    enumEtat_NIP ,
    enumEtat_AccesOk ,
    enumEtat_AccesRefuse ,
    enumEtat_Max
};

struct stUsager
{
  char nom[NB_CAR_NOM];
  char nip[NB_NIP+1]; //un octet de plus pour le \0 qu'on devra rajouter 
}; 

//prototypes
void initRegistres(void);
bool verifNip(int, char*, int);
void gereTouche(unsigned int *etat, int *usager);
void affiche(unsigned int etat, int usager);

/****** variables globales ******/
//tableau des usagers.
//attention de ne pas d�passer la capacite m�moire du PIC18. 
// 20 char pour le nom + 4 char pour le nip -> 24 octets/usager x 12 usagers = 288 octets

struct stUsager listeUsager [] =
{
    {"Mr Zero", ""},  //pas de nip
    {"Mr Un", "1"},
    {"Mme Deux", "22"},
    {"Mr Trois", "333"},
    {"Mr Quatre", "4444"},
    {"Mme No5 ", "5"},
    {"Mr No6", "6"},
    {"Mme No7", "7"},
    {"Mr No8", "8"},
    {"Mme No9", "9"},
    {"Mr No10", "10"},
    {"Mme No11", "11"}
};


void main(void) 
{
	unsigned int  etatCourant = enumEtat_Depart;
	int  etatAffiche = enumEtat_Max; //utilise pour afficher un changement d'etat. Met a max au debut pour forcer un 1er affichage

	int noUsagerCourant = -1;
	
	initRegistres(); //initialise les registres du pic
	init_serie(); //initialise la communication s.rie
	lcd_init(); //initialise l'afficheur lcd
	lcd_curseurHome();  //positionne le curseur � (1, 1)
	
	while (1) //bouvle infinie
	{
		if(kbhit())  //si une touche au clavier est entr�e
			gereTouche(&etatCourant, &noUsagerCourant);  //g�re la touche entr�e
		
		if(etatAffiche != etatCourant) //s'assure que l'�tat d'affichage correspont � l'�tat des s�quences de touches
		{
			affiche(etatCourant, noUsagerCourant);
			etatAffiche = etatCourant;
		}
	}//du while (1)
       
}//du main()

void gereTouche(unsigned int *etat, int *usager)
{
	char touche;
	static int indexNIP = 0;
	
	static char nip[NB_NIP + 1];

	if(kbhit()) //si une touche est entr�e au clavier
	{
		touche = getch(); //la touche est enregistr� dans une variable
		
		if (touche >= '0' && touche <= '9') //si la touche entr�e est comprise entre 0 et 9
		{
			if (*etat == enumEtat_NoUsager) //si l'�tape est de rentrer le numro d'usager
			{
				lcd_ecritChar(touche);  //on affiche la touche entr�e

				if(*usager < 0) *usager = atoi(&touche);
				else *usager = *usager *10 + atoi(&touche); //* 10 pour d�caler le chiffre actuel d'une position
			}
			else if (*etat == enumEtat_NIP) 
			{
				if (indexNIP < NB_NIP)  //ne pas depasser le max de car pour le nip 
				{
					lcd_ecritChar('*'); //on cache le nip a l'�cran
					nip[indexNIP] = touche;
					indexNIP++;
				}
			}
		}
		
		else if(touche == ENTER) 
		{
			
			if(*etat == enumEtat_Depart) //si l'�tat actuel est "d�part" on le change pour "usager"
				*etat = enumEtat_NoUsager;				
		
			else if(*etat == enumEtat_NoUsager)
					if(*usager >= 0 && *usager < NB_USAGER)  //si le chiffre entr� est compris entre 0 et NB_USAGER l'�tat est chang� pour nip
						*etat = enumEtat_NIP;
					else *etat = enumEtat_AccesRefuse; //sinon l'acces est refus�
			
			else if(*etat == enumEtat_NIP) //si l'�tat est sur nip
			{
				if(verifNip(*usager, nip, indexNIP))//le nip est v�rifi�
					*etat = enumEtat_AccesOk; //si le nip est bon, l'acces est donné
				else
				{
					*etat = enumEtat_AccesRefuse; //sinon l'acces est refus� et les diff�rentes variables sont r�initialis�es
					*usager = -1;
					memset(nip, '\0', 5);
					indexNIP = 0;
				}
			}
			
			else 
			{
				*etat = enumEtat_Depart; //si aucun autre �tat peut-�tre chang� l'�tat de d�part est remis par d�faut
				*usager = -1;
				memset(nip, '\0', 5);
				indexNIP = 0;
			}
		}
		
		else if(touche == ESC) //si la touche escape est appuy�e
		{
			if(*etat == enumEtat_NIP) //si l'etat est celui d'entrer le nip
			{
				*etat = enumEtat_NoUsager;
				*usager = -1;
				memset(nip, '\0', 5);
				indexNIP = 0;
			}
			else if(*etat == enumEtat_NoUsager)
			{
				*etat = enumEtat_Depart;
				*usager = -1;
				memset(nip, '\0', 5);
				indexNIP = 0;
			}
		}
	}
}

/**
 * @brief Verifie si on a un NIP valide pour un usager de la liste globale listeUsager
 * @param le no d'usager, le tableau de char contenant le nip, le nombre de char dans le nip
 * @return vrai si nip est bon, faux sinon
 */
bool verifNip(int usager, char* no, int index)
{
    
	no[index] = '\0'; //ajoute un '\0' pour faire fonctionner la fonction strcmp
	
	if(!strcmp(no, listeUsager[usager].nip)) //si le nip entr� est pareil au nip de l'usager choisi
		return true; //le nip est bon

	return false; //le nip est pas bon
    
}

/**
 * @brief Cette fonction g�re l'affichage avec l'�tape du processus d'entr� actuel
 * @param etat actuel de la sequence d'entr�e
 * @param usager qui desir entrer
 */
void affiche(unsigned int etat, int usager)
{
	switch(etat) 
	{
		case 0: //d�part
			lcd_effaceAffichage();
			lcd_curseurHome();
			lcd_putMessage("acces local 283");
			lcd_gotoXY(1, 2);
			lcd_putMessage("<Enter>");
			break;

		case 1: //entrer no usager
			lcd_effaceAffichage();
			lcd_curseurHome();
			lcd_putMessage("Entrez votre no d'etudiant");
			lcd_gotoXY(1, 3);
			lcd_putMessage("<Enter>");
			lcd_gotoXY(1, 4);
			break;

		case 2: //entrer nip
			lcd_effaceAffichage();
			lcd_curseurHome();
			lcd_putMessage("Entrez votre NIP");
			lcd_gotoXY(1, 2);
			lcd_putMessage("<Enter>");
			lcd_gotoXY(1, 3);
			break;

		case 3: //acces ok
			lcd_effaceAffichage();
			lcd_curseurHome();
			lcd_putMessage("Bonjour ");
			lcd_putMessage(listeUsager[usager].nom);
			lcd_gotoXY(1, 2);
			lcd_putMessage("<Enter>");
			lcd_gotoXY(1, 3);
			DEL_J = 1;
			break;

		case 4: //acces refus�
			lcd_effaceAffichage();
			lcd_curseurHome();
			lcd_putMessage("acces refuse");
			lcd_gotoXY(1, 2);
			lcd_putMessage("<Enter>");
			DEL_R = 1;
			break;
	}
}


