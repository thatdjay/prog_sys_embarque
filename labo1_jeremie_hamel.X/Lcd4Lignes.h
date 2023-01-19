/**
 * @file   Lcd4Lignes.h
 * @author Richard Cloutier
 * @date   Janvier 2014
 * @brief  Fichier de d�finition pour les diff�rentes fonctions utilitaires pour le fonctionnement de l'afficheur LCD.
 * @version 1.0 : Adaptation du code fait par Christian Francoeur en d�cembre 2012 pour la carte BigPic
 */

#ifndef _LCD4LIGNES
#define _LCD4LIGNES

#define _XTAL_FREQ  16000000 //Fr�quence de l'oscillateur utile pour la fonction de d�lai
/**   
 *  Vous avez aussi les carat�res suivants qui ne sont pas dans le standard ASCII
 *  � = 0         � = 1         � = 2        � = 3        � = 4    
 *  � = 5         � = 6         � = 7
 * 
 *  Exemple: lcdPutMess("\1t\1");  ==> �t�
 */

/**
 * @brief Fonction d'initialisation. Elle doit �tre appel�e avant de pouvoit utiliser les autres.
 */
void lcd_init(void);

/**
 * @brief Permet de modifier la position du curseur sur l'afficheur.
 * @param x valide entre 1 et 20
 * @param y valide entre 1 et 4
 */
void lcd_gotoXY(unsigned char x, unsigned char y);

/**
 * @brief Retourne le curseur � la position de base (en haut � gauche)
 */
void lcd_curseurHome(void);

/**
 * @brief �criture du caract�re sp�cifi� � la position courante du curseur.
 * @param car Valeur ASCII que l'on veut afficher.
 */
void lcd_ecritChar(unsigned char car);

/**
 * @brief Permet l'affichage d'une cha�ne de caract�res � partir de la position courante du curseur.
 * @param chaine Cha�ne qui contient les caract�res ASCII � afficher. La valeur '\0' ne sera pas affich�e.
 */
void lcd_putMessage(const unsigned char *chaine);

/**
 * @brief Efface l'affichage et retourne le curseur � la position intiale.
 */
void lcd_effaceAffichage(void);                     

/**
 * @brief Efface une ligne compl�te sur l'afficheur.
 * @param y Num�ro de la ligne � effacer. Valide entre 1 et 4.
 */
void lcd_effaceLigne(unsigned char y);              

/**
 * @brief Effa�age � partir du curseur.
 * @param nbr Nombre de caract�res � effacer.
 */
void lcd_effaceChar(unsigned char nbr);             // Effacer nombre de caract�res

/**
 * @brief Cacher le curseur.
 */
void lcd_cacheCurseur(void);

/**
 * @brief Afficher le curseur.
 */
void lcd_montreCurseur(void);

#endif