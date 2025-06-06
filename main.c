/* --------------------------------------------------------------
 * Fichier     :   main.c
 * Auteur(s)   :
 * Description :
 * -------------------------------------------------------------- */

#include <xc.h>

// Configuration materielle du PIC :
#pragma config FEXTOSC = OFF           // Pas de source d'horloge externe
#pragma config RSTOSC = HFINTOSC_64MHZ // Horloge interne de 64 MHz
#pragma config WDTE = OFF              // DÃ©sactiver le watchdog

#define _XTAL_FREQ 64000000 // Frequence d'horloge - necessaire aux macros de delay (_delay(N) ; __delay_us(N) ; __delay_ms(N)))

// DÃ©finition des masques, macros, etc. :
// TODO


// DÃ©claration de fonctions et variables globales permettant au code C et Ã l'asm de les partager
// Une mÃªme fonction ou variable cÃ´tÃ© asm est prÃ©fixÃ©e par un underscore, et ne l'est pas cÃ´tÃ© C
// Avec ce formalisme, elles sont utilisables de faÃ§on intercangeable et transparente :
// | ---- asm ----- | ------------- C ----------------- |
// | _TX_64LEDS  <--|--> void TX_64LEDS(void)           |
// | _pC         <--|--> volatile const char * pC       |
// | _LED_MATRIX <--|--> volatile char LED_MATRIX [256] |

// DÃ©finition des fonctions relatives Ã la matrice de LEDs:
extern void TX_64LEDS(void); // Fonction dÃ©finie dans tx.asm ; Fonction permettant d'envoyer la commande pour piloter les 64 LEDs, telle que dÃ©crite dans LED_MATRIX

// DÃ©finition des constantes / variables relatives Ã la matrice de LEDs :
volatile char LED_MATRIX [256] ; // Definition d'une matrice de 64 x 4 octets contenant les composantes R/G/B/W de chaque LED (1 octet/couleur/LED)
volatile const char * pC = LED_MATRIX; // Pointeur vers LED_MATRIX


void led(int col, int row, int red, int green, int blue, int white) {
  LED_MATRIX[(col-1)*4*8 + (row-1)*4] = green; 
  LED_MATRIX[(col-1)*4*8 + (row-1)*4 + 1] = red;
  LED_MATRIX[(col-1)*4*8 + (row-1)*4 + 2] = blue; 
  LED_MATRIX[(col-1)*4*8 + (row-1)*4 + 3] = white; 
}

void test_col(int col, int red, int green, int blue, int white) {
  led(col, 1, red, green, blue, white);
  led(col, 2, red, green, blue, white);
  led(col, 3, red, green, blue, white);
  led(col, 4, red, green, blue, white);
  led(col, 5, red, green, blue, white);
  led(col, 6, red, green, blue, white);    
  led(col, 7, red, green, blue, white);
  led(col, 8, red, green, blue, white);
}

void test_row(int row, int red, int green, int blue, int white) {
  led(1, row, red, green, blue, white);
  led(2, row, red, green, blue, white);
  led(3, row, red, green, blue, white);
  led(4, row, red, green, blue, white);
  led(5, row, red, green, blue, white);
  led(6, row, red, green, blue, white);    
  led(7, row, red, green, blue, white);
  led(8, row, red, green, blue, white);
}

// volume entre 0 compris et 8 compris
// volume à 0 = aucune led allumée
// volume à 4 = la moité du bas allumé
// volume à 8 = toutes les leds allumés
void allumer_col_volume(int col, int volume, int red, int green, int blue, int white) {
    if(volume>=1) {
        led(col, 1, red, green, blue, white);
    } else {
        led(col, 1, 0, 0, 0, 0);
    }

    if(volume>=2) {
        led(col, 2, red, green, blue, white);
    } else {
        led(col, 2, 0, 0, 0, 0);
    }

    if(volume>=3) {
        led(col, 3, red, green, blue, white);
    } else {
        led(col, 3, 0, 0, 0, 0);
    }

    if(volume>=4) {
        led(col, 4, red, green, blue, white);
    } else {
        led(col, 4, 0, 0, 0, 0);
    }

    if(volume>=5) {
        led(col, 5, red, green, blue, white);
    } else {
        led(col, 5, 0, 0, 0, 0);
    }

    if(volume>=6) {
        led(col, 6, red, green, blue, white);
    } else {
        led(col, 6, 0, 0, 0, 0);
    }

    if(volume>=7) {
        led(col, 7, red, green, blue, white);
    } else {
        led(col, 7, 0, 0, 0, 0);
    }

    if(volume==8) {
        led(col, 8, red, green, blue, white);
    } else {
        led(col, 8, 0, 0, 0, 0);
    }
}

// - Fonction main ----------------------------------------------------------------------
void main(void) {
    /* Configuration des entrées / sorties */
    TRISB &= 0b11011111; // port 5 en sortie (CMD_MATRIX)


    TRISC &= 0x00; // leds 0-7 en sortie
    LATC  = 0xFF; // leds 0-7 allumées
    
    /* Corps du programme */
    // TODO

    
//    Smiley
    led(3, 5, 16, 16, 0, 0);
    led(3, 6, 16, 16, 0, 0);

    led(6, 6, 16, 16, 0, 0);
    led(6, 5, 16, 16, 0, 0);

    led(2, 3, 16, 16, 0, 0);
    led(3, 2, 16, 16, 0, 0);
    led(4, 2, 16, 16, 0, 0);
    led(5, 2, 16, 16, 0, 0);
    led(6, 2, 16, 16, 0, 0);
    led(7, 3, 16, 16, 0, 0);
    
//    Croix
//    led(1, 1, 16, 0, 0, 0);
//    led(2, 2, 16, 0, 0, 0);
//    led(3, 3, 16, 0, 0, 0);
//    led(4, 4, 16, 0, 0, 0);
//    led(5, 5, 16, 0, 0, 0);
//    led(6, 6, 16, 0, 0, 0);
//    led(7, 7, 16, 0, 0, 0);
//    led(8, 8, 16, 0, 0, 0);
//
//    led(1, 8, 16, 0, 0, 0);
//    led(2, 7, 16, 0, 0, 0);
//    led(3, 6, 16, 0, 0, 0);
//    led(4, 5, 16, 0, 0, 0);
//    led(5, 4, 16, 0, 0, 0);
//    led(6, 3, 16, 0, 0, 0);
//    led(7, 2, 16, 0, 0, 0);
//    led(8, 1, 16, 0, 0, 0);
    
//    Ligne
//    test_row(1, 16, 0, 0, 0);

//    Colonne
//    test_col(2, 16, 0, 0, 0);
//    test_col(4, 16, 0, 0, 0); 
//    test_col(8, 16, 0, 0, 0);
    
    while(1) {
        // reset les leds en envoyant 0 pendant plus de 80us (88us mesuré)
        __delay_us(200); 
        TX_64LEDS(); 
    }
      
    return;
}

