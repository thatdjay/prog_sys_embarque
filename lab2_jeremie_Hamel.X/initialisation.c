
/* Entête de fichier à faire */
#include <xc.h>


void initRegistres(void)
{
	ADCON1 = 0x7;
	TRISA  = 0; //Tout le port a en sortie
	TRISD  = 0;
	TRISC = 0x80;
}