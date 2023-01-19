/**
 * @file   main.c,  Laboratoire 1
 * @author Jeremie Hamel
 * @date   19/01/2023 
 * @brief  Ce programme allume des dels jaune et rouge en alternance aux 500ms trois fois. 
 *         Ensuite, il affiche "Lab1 Jeremie Hamel" sur l'afficheur lcd. Pour finir, il
 *         affiche les caractères recus par le port serie à l'afficheur.
 * Environnement:
 *     Développement: MPLAB X IDE (version 6.0)
 *     Compilateur: XC8 (version 2.20)
 *     Matériel:    PIC 18F458
 */

/****************** Liste des INCLUDES ****************************************/
#include <xc.h>
#include <stdbool.h>  // pour l'utilisation du type bool
#include "Lcd4Lignes.h"
#include "serie_458.h"

/********************** CONSTANTES *******************************************/
#define NB_LIGNE 4  //afficheur LCD 4x202424
#define NB_COL 20

#define ledRouge PORTDbits.RD7
#define ledJaune PORTCbits.RC0

/********************** PROTOTYPES *******************************************/
void initialisation(void);

/****************** VARIABLES GLOBALES ****************************************/

/*               ***** PROGRAMME PRINCPAL *****                             */
void main(void) 
{

    /******* variables locale au main ******/
    char c; //variable pour le caract?re ? afficher
    
    //les différentes initialisations et setup
    initialisation(); //initialise les ports e/s
    init_serie(); //initialise la communication série
    
    lcd_init(); //initialisation de l'afficheur lcd
    lcd_cacheCurseur(); //cache le curseur
    lcd_curseurHome(); //positionne le curseur ?(1,1)
    lcd_effaceAffichage(); //efface l'afficheur lcd
    
    //fait clignoter les dels en alternances en les allumants, attendant 500ms, les éteignants pour finalement attendre un autre 500ms. 
    //Ces étapes sont répétées 3 fois
    for(int i = 0; i < 3; i++)
    {
        ledRouge = 1;
        ledJaune = 0;
        __delay_ms(500);
        ledRouge = 0; 
        ledJaune = 1;
        __delay_ms(500);
    }
    
    lcd_putMessage("Lab1 Jeremie Hamel"); //le message est affiché sur l'écran
    lcd_gotoXY(1, 2); //le curseur bouge à (1,2) pour ne pas afficher les caractères par dessus le message

    while (1) //boucle infinie
    {
        if(kbhit()) //si une touche est tappée au clavier
        {
            c = getch(); //la touche est mémorisée dans la variable c
            lcd_ecritChar(c); //le caractère est affiché
            __delay_ms(100); //délai pour ne pas afficher des caracères trop rapidement
        }        
    }
}

/*
 * @brief Fait l'initialisation des différents registres et variables.
 * @param Aucun
 * @return Aucun
 */
void initialisation(void) 
{
    ADCON1 = 0x7; //force le mode digital pour le port B
    TRISA  = 0; //Tout le port a en sortie
    TRISD  = 0; //tout le port d est en sortie
    TRISC  = 0b10000000; //tout le port C est en sortie sauf la pin ou se situe la broche RX
}

