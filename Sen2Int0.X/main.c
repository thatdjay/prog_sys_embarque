 /**
  @file   main.c, projet:
  @author 
  @date   
  @brief  
  
  Environnement:
      Développement: MPLAB X IDE (version 6.00)
      Compilateur: XC8 (version 2.40)
      Matériel: Carte démo du Pickit4. PIC 18F45K20
 */

/****************** Liste des INCLUDES ****************************************/
#include <xc.h>


/********************** CONSTANTES *******************************************/
#define _XTAL_FREQ 1000000 //Constante utilisée par __delay_ms(x). Doit = fréq interne du uC

/********************** PROTOTYPES *******************************************/
void initialisation(void);

/****************** VARIABLES GLOBALES ****************************************/
int g_etat = 0;
void main(void)
{
    /***Variables locales****/

    /*Les codes d'initialisations*/
    initialisation();
   
    
    while(1) // Boucle principale du programme
    {
        switch(g_etat)
        {
           case 0:
                PORTD += 1;
                break;
            case 1:
                PORTD *= 2;
                if(PORTD == 0)
                    PORTD = 1;
                break;
            case 2:
                PORTD = PORTD>>1;
                if (PORTD == 0)
                    PORTD = 0x80;
                break;
            case 3:
                PORTD = 0xFF;
                break;
 
        }
        __delay_ms(150);
    }
}


/**
 * @brief Initialisation des différents regesitres du PIC.
 */
void initialisation(void)
{
    TRISD = 0; //Tout le port D en sortie
    
    //Attention! 
    ANSELH = 0;  // RB0 à RB4 en mode digital. Sur 18F45K20 AN et PortB sont sur les memes broches
    TRISB = 0xFF; //tout le port B en entree
    
    TRISBbits.RB0 = 1; //PORTB bit 0 en entrée
 
 
    INTCONbits.GIE = 1; //Permettre les interruptions globales 
 
    //Configuration de l'interruption INT0
    INTCONbits.INT0IF = 0; //Reset flag d'interruption d'INT0 (pour s'assurer de ne pas avoir d'interruption au départ)    
    INTCONbits.INT0IE = 1; //Permettre les interruptions d'INT0
    INTCON2bits.INTEDG0 = 0; //Choisir le front de l'interruption INT0
}

