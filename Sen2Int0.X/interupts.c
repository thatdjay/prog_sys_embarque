#include <xc.>
#include "interupts.h"


extern int g_etat = 0;
/* High-priority interrupt service routine */
void __interrupt(high_priority) high_isr(void)
{
    if(INTCONbits.INT0IF)   //interruption INT0?
    {
        PORTD = 0; //� ch changement d'�tat, on recommence au d�but.
        g_etat++;
 
        if (g_etat > 4) 
            g_etat = 0;
 
 
        __delay_ms(200); //debounce SW1 (pour ne pas changer d'�tat lors des rebonds hardware)
        INTCONbits.INT0IF=0;  //reset flag d'intr.
    }
}