
#include <xc.h>

/********* Variables globales **********/
extern unsigned char g_nbInt0; //Variable globale définie dans le fichier interrupts.c
 
/********* Prototypes **********/ 
void initialisation(void);
 
void main(void)
{
    initialisation(); //Configuration des ports utilisés et de l'interruption INT0
    PORTD = 0; //Éteint toutes les DEL reliées au port D.
    while(1) // Boucle principale du programme
    {
        PORTD = g_nbInt0;
    }
}
 
/**
* @brief Initialisation des différents registres du PIC
*/
void initialisation(void)
{
    //Port D
    TRISD = 0; //Tout le port D est configuré en sortie
 
    //Port B
    ANSELH = 0; //  PORTB en mode numérique (pas de fonction analogique)
    TRISBbits.RB0 = 1; //PORTB bit 0 en entrée
 
 
    INTCONbits.GIE = 1; //Permettre les interruptions globales 
 
    //Configuration de l'interruption INT0
    INTCONbits.INT0IF = 0; //Reset flag d'interruption d'INT0 (pour s'assurer de ne pas avoir d'interruption au départ)    
    INTCONbits.INT0IE = 1; //Permettre les interruptions d'INT0
    INTCON2bits.INTEDG0 = 0; //Choisir le front de l'interruption INT0
 
    //Configuration de l'interruption INT1
    INTCON3bits.INT1IF = 0; //Reset flag d'interruption d'INT0 (pour s'assurer de ne pas avoir d'interruption au départ)    
    INTCON3bits.INT1IE = 1; //Permettre les interruptions d'INT0
    INTCON2bits.INTEDG1 = 0; //Choisir le front de l'interruption INT0
}