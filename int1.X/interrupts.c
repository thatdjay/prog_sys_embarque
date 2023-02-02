#include <xc.h>
#include "interrupts.h"

unsigned char g_nbInt0; //Variable globale définie dans le fichier interrupts.c

/* High-priority interrupt service routine */
void __interrupt(high_priority) high_isr(void)
{
    if(INTCONbits.INT0IF)   //interruption INT0?
    {
        g_nbInt0++; //Incrémente le nombre de fois que l'interruption INT0 est survenue
        INTCONbits.INT0IF = 0;  //Remise à zéro du flag d'interruption
    }
 
    if(INTCON3bits.INT1IF)   //interruption INT1?
    {
        g_nbInt0--; //Nombre d'interruption -1
        INTCON3bits.INT1IF = 0;  //Remise à zéro du flag d'interruption
    }
 
 
}