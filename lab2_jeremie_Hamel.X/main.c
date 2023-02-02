/* @date 2 fevrier 2023
 * @author Jeremie Hamel
 * @file main.c du lab 2
 * @brief Ce programme simule la porte du local 283 dans la fonctione d'une porte 
 *        securisée par code. l'utilisateur doit enter son numéro d'étudiant puis
 *        entrer son mot de passe. Dans la cas ou le mot de passe correspond au
 *        numéro d'étudiant un message d'acces accepté apparait a l'écran. Dans 
 *        le cas contraire un message d'acces refusé apparait.
 */

#include <xc.h> //pour l'utilisation du compilateur XC8
#include <stdbool.h> //pour utiliser des variables de type bool
#include <stdlib.h>
#include <string.h> //pour strcmp
#include "initialisation.h"  //pour l'initialisations des ports du pic
#include "Lcd4Lignes.h" //pour l'utilisation de l'afficheur lcd
#include "serie_458.h"	 //pour la communication série entre le pic et l'ordinateur

/******Constantes *******/
#define _XTAL_FREQ 16000000 //Constante utilisée par __delay_ms(x). Doit = freq interne du uC
#define NB_CAR_NOM 20   //nom d'usager a 20 caracteres max
#define NB_NIP 4  //les nip on 4 car max 
#define NB_USAGER 12  //nombre d'usagers
#define DEL_R PORTDbits.RD7  //DEL rouge
#define DEL_J PORTCbits.RC0  //DEL jaune
#define ENTER 0x0D //touche Enter
#define ESC 0x1B   //touche escape
#define BCKSPC 0x08 //touche backspace

//  Connstantes des états possibles de la machine à  état
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
//attention de ne pas dépasser la capacite mémoire du PIC18. 
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
	lcd_curseurHome();  //positionne le curseur à  (1, 1)
	
	while (1) //bouvle infinie
	{
		if(kbhit())  //si une touche au clavier est entrée
			gereTouche(&etatCourant, &noUsagerCourant);  //gère la touche entrée
		
		if(etatAffiche != etatCourant) //s'assure que l'état d'affichage correspont à  l'état des séquences de touches
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

	if(kbhit()) //si une touche est entrée au clavier
	{
		touche = getch(); //la touche est enregistré dans une variable
		
		if (touche >= '0' && touche <= '9') //si la touche entrée est comprise entre 0 et 9
		{
			if (*etat == enumEtat_NoUsager) //si l'étape est de rentrer le numro d'usager
			{
				lcd_ecritChar(touche);  //on affiche la touche entrée

				if(*usager < 0) *usager = atoi(&touche);
				else *usager = *usager *10 + atoi(&touche); //* 10 pour décaler le chiffre actuel d'une position
			}
			else if (*etat == enumEtat_NIP) 
			{
				if (indexNIP < NB_NIP)  //ne pas depasser le max de car pour le nip 
				{
					lcd_ecritChar('*'); //on cache le nip a l'écran
					nip[indexNIP] = touche;
					indexNIP++;
				}
			}
		}
		
		else if(touche == ENTER) 
		{
			
			if(*etat == enumEtat_Depart) //si l'état actuel est "départ" on le change pour "usager"
				*etat = enumEtat_NoUsager;				
		
			else if(*etat == enumEtat_NoUsager)
					if(*usager >= 0 && *usager < NB_USAGER)  //si le chiffre entré est compris entre 0 et NB_USAGER l'état est changé pour nip
						*etat = enumEtat_NIP;
					else *etat = enumEtat_AccesRefuse; //sinon l'acces est refusé
			
			else if(*etat == enumEtat_NIP) //si l'état est sur nip
			{
				if(verifNip(*usager, nip, indexNIP))//le nip est vérifié
					*etat = enumEtat_AccesOk; //si le nip est bon, l'acces est donnÃ©
				else
				{
					*etat = enumEtat_AccesRefuse; //sinon l'acces est refusé et les différentes variables sont réinitialisées
					*usager = -1;
					memset(nip, '\0', 5);
					indexNIP = 0;
				}
			}
			
			else 
			{
				*etat = enumEtat_Depart; //si aucun autre état peut-être changé l'état de départ est remis par défaut
				*usager = -1;
				memset(nip, '\0', 5);
				indexNIP = 0;
			}
		}
		
		else if(touche == ESC) //si la touche escape est appuyée
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
	
	if(!strcmp(no, listeUsager[usager].nip)) //si le nip entré est pareil au nip de l'usager choisi
		return true; //le nip est bon

	return false; //le nip est pas bon
    
}

/**
 * @brief Cette fonction gère l'affichage avec l'étape du processus d'entré actuel
 * @param etat actuel de la sequence d'entrée
 * @param usager qui desir entrer
 */
void affiche(unsigned int etat, int usager)
{
	switch(etat) 
	{
		case 0: //départ
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

		case 4: //acces refusé
			lcd_effaceAffichage();
			lcd_curseurHome();
			lcd_putMessage("acces refuse");
			lcd_gotoXY(1, 2);
			lcd_putMessage("<Enter>");
			DEL_R = 1;
			break;
	}
}


