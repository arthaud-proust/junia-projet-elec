/* --------------------------------------------------------------
 * Fichier     :   main.c
 * Auteur(s)   :
 * Description :
 * -------------------------------------------------------------- */

#include <xc.h>

// Configuration materielle du PIC :
#pragma config FEXTOSC = OFF           // Pas de source d'horloge externe
#pragma config RSTOSC = HFINTOSC_64MHZ // Horloge interne de 64 MHz
#pragma config WDTE = OFF              // Désactiver le watchdog

#define _XTAL_FREQ 64000000 // Frequence d'horloge - necessaire aux macros de delay (_delay(N) ; __delay_us(N) ; __delay_ms(N)))

// Les canaux d'entrée du son
#define CHANNEL_4 0b000010 // RA2 = TPE4
#define CHANNEL_3 0b000011 // RA3 = TPE3
#define CHANNEL_2 0b000100 // RA4 = TPE2
#define CHANNEL_1 0b000101 // RA5 = TPE1

// Déclaration de fonctions et variables globales permettant au code C et à l'asm de les partager
// Une même fonction ou variable côté asm est préfixée par un underscore, et ne l'est pas côté C
// Avec ce formalisme, elles sont utilisables de façon intercangeable et transparente :
// | ---- asm ----- | ------------- C ----------------- |
// | _TX_64LEDS  <--|--> void TX_64LEDS(void)           |
// | _pC         <--|--> volatile const char * pC       |
// | _LED_MATRIX <--|--> volatile char LED_MATRIX [256] |

// Définition des fonctions relatives à la matrice de LEDs:
extern void TX_64LEDS(void); // Fonction définie dans tx.asm ; Fonction permettant d'envoyer la commande pour piloter les 64 LEDs, telle que décrite dans LED_MATRIX

// Définition des constantes / variables relatives à la matrice de LEDs :
volatile char LED_MATRIX [256] ; // Definition d'une matrice de 64 x 4 octets contenant les composantes R/G/B/W de chaque LED (1 octet/couleur/LED)
volatile const char * pC = LED_MATRIX; // Pointeur vers LED_MATRIX

// Définir l'intensité RGBW d'une led par la colonne et la ligne
void led(int colonne, int ligne, int rouge, int vert, int bleu, int blanc) {
  LED_MATRIX[(ligne-1)*4*8 + (colonne-1)*4] = vert; 
  LED_MATRIX[(ligne-1)*4*8 + (colonne-1)*4 + 1] = rouge;
  LED_MATRIX[(ligne-1)*4*8 + (colonne-1)*4 + 2] = bleu; 
  LED_MATRIX[(ligne-1)*4*8 + (colonne-1)*4 + 3] = blanc; 
}

// Allumer une colonne en fonction du volume.
// volume entre 0 compris et 8 compris
// volume à 0 = aucune led allumée
// volume à 4 = la moité du bas allumé
// volume à 8 = toutes les leds allumés
void allumer_colonne_volume(int colonne, int volume, int rouge, int vert, int bleu, int blanc) {
    if(volume>=1) {
        led(colonne, 8, rouge, vert, bleu, blanc);
    } else {
        led(colonne, 8, 0, 0, 0, 0);
    }

    if(volume>=2) {
        led(colonne, 7, rouge, vert, bleu, blanc);
    } else {
        led(colonne, 7, 0, 0, 0, 0);
    }

    if(volume>=3) {
        led(colonne, 6, rouge, vert, bleu, blanc);
    } else {
        led(colonne, 6, 0, 0, 0, 0);
    }

    if(volume>=4) {
        led(colonne, 5, rouge, vert, bleu, blanc);
    } else {
        led(colonne, 5, 0, 0, 0, 0);
    }

    if(volume>=5) {
        led(colonne, 4, rouge, vert, bleu, blanc);
    } else {
        led(colonne, 4, 0, 0, 0, 0);
    }

    if(volume>=6) {
        led(colonne, 3, rouge, vert, bleu, blanc);
    } else {
        led(colonne, 3, 0, 0, 0, 0);
    }

    if(volume>=7) {
        led(colonne, 2, rouge, vert, bleu, blanc);
    } else {
        led(colonne, 2, 0, 0, 0, 0);
    }

    if(volume>=8) {
        led(colonne, 1, rouge, vert, bleu, blanc);
    } else {
        led(colonne, 1, 0, 0, 0, 0);
    }
}

void intialiser_convertisseur_analogique(void) {
    TRISA = 0b00111100;     // RA2 à RA5 en entrée
    ANSELA = 0b00111100;    // RA2 à RA5 en analogique

    ADREF = 0x00;           // Référence = VDD et VSS
    ADCLK = 0x3F;           // Clock = Fosc / 128
    ADCON0 = 0x84;          // ADC ON, justifié à gauche, Fosc clock
}

// Lit la valeur analogique du canal indiqué.
// La valeur de retour est normalisée entre 0 et 8
unsigned int lire_valeur_analogique(int canal) {
    // Sélectionner le canal
    ADPCH = canal; 
    
    // Lire la valeur
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO);
    
    // On retroune une valeur normalisée entre 0 et 8
    return ((ADRESH << 8) | ADRESL) / 50;
}

// - Fonction main ----------------------------------------------------------------------
void main(void) {
    // Configuration des entrées / sorties
    TRISB &= 0b11011111; // port 5 en sortie (CMD_MATRIX)
    intialiser_convertisseur_analogique();
    
    while(1) {
        // Configurer la matrice à afficher
        unsigned int volume_ch1 = lire_valeur_analogique(CHANNEL_1);
        allumer_colonne_volume(1, volume_ch1, 16, 0, 0, 0);
        allumer_colonne_volume(2, volume_ch1, 16, 0, 0, 0);
        
        unsigned int volume_ch2 = lire_valeur_analogique(CHANNEL_2);
        allumer_colonne_volume(3, volume_ch2, 16, 16, 0, 0);
        allumer_colonne_volume(4, volume_ch2, 16, 16, 0, 0);
        
        unsigned int volume_ch3 = lire_valeur_analogique(CHANNEL_3);
        allumer_colonne_volume(5, volume_ch3, 16, 16, 16, 0);
        allumer_colonne_volume(6, volume_ch3, 16, 16, 16, 0);
        
        unsigned int volume_ch4 = lire_valeur_analogique(CHANNEL_4);
        allumer_colonne_volume(7, volume_ch4, 16, 0, 16, 0);
        allumer_colonne_volume(8, volume_ch4, 16, 0, 16, 0);
        
        // éteindre la led colonne8 ligne1 car défectueuse sur une matrice
        led(8, 1, 0, 0, 0, 0);

        // Afficher la matrice
        TX_64LEDS(); 

        // Il faut réinitialiser la matrice régulièrement pour annuler les décalages
        // et éviter qu'un décalage induit au démarrage de la carte persiste.
        // Pour ça il faut envoyer 0v pendant plus de 80us
        // Comme les signaux envoyés se terminent toujours par 0v, on mets juste du délai.
        __delay_us(100);     
    }
      
    return;
}

