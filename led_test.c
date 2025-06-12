/* --------------------------------------------------------------
 * Fichier     :   main.c
 * Auteur(s)   :
 * Description :   Test ADC sur AN0 avec affichage sur LEDs de test (PORTC)
 * -------------------------------------------------------------- */

#include <xc.h>

// Configuration matérielle du PIC :
#pragma config FEXTOSC = OFF           // Pas de source d'horloge externe
#pragma config RSTOSC = HFINTOSC_64MHZ // Horloge interne 64 MHz
#pragma config WDTE = OFF              // Watchdog désactivé

#define _XTAL_FREQ 64000000 // Fréquence d'horloge pour les délais (__delay_ms etc.)

// Déclarations externes (non utilisées ici mais présentes dans le projet)
extern void TX_64LEDS(void);
volatile char LED_MATRIX[256];
volatile const char * pC = LED_MATRIX;

void maint(void) {
    // --- CONFIG PORTS ---
    TRISC = 0x00;  // PORTC = sorties (LEDs RC0 à RC7)
    TRISB &= 0xEF; // RB4 (LEDM) = sortie

    // --- TEST 1 : Allumer toutes les LEDs ---
    LATC = 0xFF;   // RC0 à RC7 ON
    LATB |= 0x10;  // LEDM ON (bit 4 de PORTB)
    
    
    // TRISC &= 0x00; // leds 0-7 en sortie
    // LATC  = 0xFF; // leds 0-7 allumées

    while(1); // Boucle infinie (les LEDs restent allumées)
}
