
#include <xc.h>

/********* Variables globales **********/
extern unsigned char g_nbInt0; //Variable globale d�finie dans le fichier interrupts.c
 
/********* Prototypes **********/ 
void initialisation(void);
 
void main(void)
{
    initialisation(); //Configuration des ports utilis�s et de l'interruption INT0
    PORTD = 0; //�teint toutes les DEL reli�es au port D.
    while(1) // Boucle principale du programme
    {
        PORTD = g_nbInt0;
    }
}
 
/**
* @brief Initialisation des diff�rents registres du PIC
*/
void initialisation(void)
{
    //Port D
    TRISD = 0; //Tout le port D est configur� en sortie
 
    //Port B
    ANSELH = 0; //  PORTB en mode num�rique (pas de fonction analogique)
    TRISBbits.RB0 = 1; //PORTB bit 0 en entr�e
 
 
    INTCONbits.GIE = 1; //Permettre les interruptions globales 
 
    //Configuration de l'interruption INT0
    INTCONbits.INT0IF = 0; //Reset flag d'interruption d'INT0 (pour s'assurer de ne pas avoir d'interruption au d�part)    
    INTCONbits.INT0IE = 1; //Permettre les interruptions d'INT0
    INTCON2bits.INTEDG0 = 0; //Choisir le front de l'interruption INT0
 
    //Configuration de l'interruption INT1
    INTCON3bits.INT1IF = 0; //Reset flag d'interruption d'INT0 (pour s'assurer de ne pas avoir d'interruption au d�part)    
    INTCON3bits.INT1IE = 1; //Permettre les interruptions d'INT0
    INTCON2bits.INTEDG1 = 0; //Choisir le front de l'interruption INT0
}