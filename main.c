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


void allumer_led(int col, int row) {
   LED_MATRIX[(col-1)*4*8 + (row-1)*4] = 16; 
}

// - Fonction main ----------------------------------------------------------------------
void main(void) {
    
    
    
    /* Configuration des entrées / sorties */
    TRISB &= 0b11011111; // port 5 en sortie (CMD_MATRIX)


    TRISC &= 0x00; // leds 0-7 en sortie
    LATC  = 0xFF; // leds 0-7 allumées
    
    /* Corps du programme */
    // TODO

    // allume le vert de la première led

    allumer_led(3, 5);
    allumer_led(3, 6);
    
    allumer_led(6, 6);
    allumer_led(6, 5);
    
    allumer_led(2, 3);
    allumer_led(3, 2);
    allumer_led(4, 2);
    allumer_led(5, 2);
    allumer_led(6, 2);
    allumer_led(7, 3);
    
    while(1) {
        TX_64LEDS(); 
    }
      
    return;
}

