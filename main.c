#include <xc.h>

#pragma config FEXTOSC = OFF           
#pragma config RSTOSC = HFINTOSC_64MHZ 
#pragma config WDTE = OFF              

#define _XTAL_FREQ 64000000

void ADC_Init(void) {
    ADCON0 = 0x01;              // ADC ON, AN0 sélectionné
    ADCON1 = 0x00;              // Tous les canaux analogiques
    ADCON2 = 0b10111110;        // Fosc/64, Justification à droite, TACQ = 20 TAD
}

unsigned int ADC_Read(unsigned char channel) {
    if(channel > 13) return 0;

    ADCON0 &= 0xC3;              // Clear bits de canal
    ADCON0 |= (channel << 2);    // Choix du canal
    __delay_us(10);              // Acquisition
    GO_nDONE = 1;                
    while(GO_nDONE);            
    return ((ADRESH << 8) | ADRESL);
}

void main(void) {
    TRISC = 0x00; // LEDs en sortie
    LATC = 0x00;

    TRISA = 0xFF; // RA0 en entrée
    ADC_Init();   // Initialiser l?ADC

    while(1) {
        unsigned int val = ADC_Read(0); // Lire AN0

        // Vu-mètre simple
        LATC = 0x00;
        if(val > 100) LATC |= 0x01; // RC0
        if(val > 200) LATC |= 0x02; // RC1
        if(val > 300) LATC |= 0x04; // RC2
        if(val > 400) LATC |= 0x08; // RC3

        __delay_ms(100);
    }
}