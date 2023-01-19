/**
 * @file   main.c,  Laboratoire 1
 * @author Jeremie Hamel
 * @date   
 * @brief  
 * Environnement:
 *     D�veloppement: MPLAB X IDE (version 6.0)
 *     Compilateur: XC8 (version 2.20)
 *     Mat�riel: Carte d�mo noire avec PIC 18F458
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
    
    //les diff?rentes initialisations et setup
    initialisation(); //initialise les ports e/s
    init_serie(); //initialise la communication s�rie
    
    lcd_init(); //initialisation de l'afficheur lcd
    lcd_cacheCurseur(); //cache le curseur
    lcd_curseurHome(); //positionne le curseur ?(1,1)
    lcd_effaceAffichage(); //efface l'afficheur lcd
    
    //fait clignoter les dels en les allumants, attendant 500ms, les �teignants pour finalement attendre un autre 500ms. 
    //Ces ?tapes sont r�p�t�es 3 fois
    for(int i = 0; i < 3; i++)
    {
        ledRouge = 1;
        ledJaune = 0;
        __delay_ms(500);
        ledRouge = 0; 
        ledJaune = 1;
        __delay_ms(500);
    }
    
    lcd_putMessage("Lab1 Jeremie Hamel"); //le message est affich� sur l'�cran
    lcd_gotoXY(1, 2); //le curseur bouge � (1,2) pour ne pas afficher les caract�res par dessus le message

    while (1) //boucle infinie
    {
        if(kbhit()) //si une touche est tapp�e au clavier
        {
            c = getch(); //la touche est m�moris�e dans la variable c
            lcd_ecritChar(c); //le caract?re est affich�
            __delay_ms(100); //d?lai pour ne pas afficher des carac�res trop rapidement
        }        
    }
}

/*
 * @brief Fait l'initialisation des diff�rents regis tres et variables.
 * @param Aucun
 * @return Aucun
 */
void initialisation(void) 
{
    ADCON1 = 0x7;
    TRISA  = 0; //Tout le port a en sortie
    TRISD  = 0; //tout le port d est en sortie
    TRISC  = 0b10000000; //tout le port c est en sortie sauf la pin ou se situe la broche RX
}

