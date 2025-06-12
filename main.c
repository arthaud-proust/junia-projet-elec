#include <xc.h>

#pragma config FEXTOSC = OFF           
#pragma config RSTOSC = HFINTOSC_64MHZ 
#pragma config WDTE = OFF              

#define _XTAL_FREQ 64000000

void ADC_Init(void) {
    TRISAbits.TRISA2 = 1;     // RA0 en entrée
    ANSELAbits.ANSELA2 = 1;   // RA0 en analogique

    ADPCH = 0b00000010;             // Sélectionner ANA0 / RA0
    ADREF = 0x00;             // Référence = VDD et VSS
    ADCLK = 0x3F;             // Clock = Fosc / 128
    ADCON0 = 0x84;            // ADC ON, Right justified, Fosc clock
}

unsigned int ADC_Read(void) {
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO);
    return ((ADRESH << 8) | ADRESL);
}

void main(void) {
    TRISC = 0x00; // LEDs de test en sortie
    LATC = 0x00;

    ADC_Init();

    while (1) {
        unsigned int val = ADC_Read();

        LATC = 0x00;
        if(val > 100) LATC |= 0x01;
        if(val > 200) LATC |= 0x02;
        if(val > 300) LATC |= 0x04;
        if(val > 400) LATC |= 0x08;

        __delay_ms(100);
    }
}
