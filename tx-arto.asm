#include <xc.inc>

; When assembly code is placed in a psect, it can be manipulated as a
; whole by the linker and placed in memory.
psect   txfunc,local,class=CODE,reloc=2 ; PIC18's should have a reloc (alignment) flag of 2 for any psect which contains executable code.

; -----------------------------------------------------------------
; GLOBALS
;
; DÃ©claration de fonctions et variables globales permettant au code C et Ã  l'asm de les partager
; Une mÃªme fonction ou variable cÃ´tÃ© asm est prÃ©fixÃ©e par un underscore, et ne l'est pas cÃ´tÃ© C
; Avec ce formalisme, elles sont utilisables de faÃ§on intercangeable et transparente :
; | ---- asm ----- | ------------- C ----------------- |
; | _TX_64LEDS  <--|--> void TX_64LEDS(void)           |
; | _pC         <--|--> volatile const char * pC       |
; | _LED_MATRIX <--|--> volatile char LED_MATRIX [256] |

; Fonction globales
global _TX_64LEDS ; Fonction dÃ©finie dans tx.asm ; Fonction permettant d'envoyer la commande pour piloter les 64 LEDs, telle que dÃ©crite dans LED_MATRIX

; Constantes/variables globales
global _pC         ; Constante dÃ©finie dans main.c ; Pointeur vers LED_MATRIX
global _LED_MATRIX ; Variable  dÃ©finie dans main.c ; Tableau (256 octets = 64 x 4) des composantes RGBW de la matrice LED (1 octet/couleur/LED)

_TX_64LEDS:
    ; Cette fonction envoie sur CMD_MATRIX l'intÃ©gralitÃ© de la matrice LED_MATRIX,
    ; Chaque bit de chaque octet encodÃ© en largeur d'impulsion

    ; Place un pointeur au dÃ©but de la matrice LED_MATRIX
    ; Voir section 10.8.12 (p. 150) de la datasheet PIC18F25K40
    MOVFF _pC + 0, WREG ; Charge le LSB du pointeur de LED_MATRIX dans WREG
    MOVWF FSR0L, 0      ; DÃ©finit le LSB du registre d'adressage indirect
    MOVFF _pC + 1, WREG ; Charge le MSB du pointeur de LED_MATRIX dans WREG
    MOVWF FSR0H, 0      ; DÃ©finit le MSB du registre d'adressage indirect
    
   

    ; il faut incrémenter sur chaque bit de l'octet
    ;TODO
    
    ; si bit == 1 :
     SET_1
    ; sinon :
    SET_0

    ; Désormais, dans l'exécution de l'instruction suivante, la valeur pointée par <FSR0H-FSR0L> est chargée dans WREG, et <FSR0H-FSR0L> est incrémenté :
    ; incrémente de un octet
    MOVF POSTINC0, 0, 0
    
    ;boucler au début 63 fois
    ; TODO
    
    RETURN

SET_0:
    ; on envoie HAUT pendant 0,32us, puis BAS pendant 0,93us
    ; c'est à dire qu'on envoie 2,9 fois plus de HAUT que de BAS
    BANKSEL LATB ; sélectionner CMD_MATRIX
   
    BSF LATB, 5 ; allumer le bit 5 de CMD_MATRIX
    ; on détermine expérimentalement le nombre d'instructions pour atteindre 0,32us
    NOP   
    NOP

    
    BCF LATB, 5 ; éteindre le bit 5 de CMD_MATRIX
    ; on écrit 2,9 fois plus d'instructions pour atteindre 0,93us
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    
    RETURN
    
SET_1:
    ; on envoie HAUT pendant 0,82us, puis BAS pendant 0,43us
    ; c'est à dire qu'on envoie 2,9 fois plus de HAUT que de BAS
    BANKSEL LATB ; sélectionner CMD_MATRIX
   
    BSF LATB, 5 ; allumer le bit 5 de CMD_MATRIX
    ; on détermine expérimentalement le nombre d'instructions pour atteindre 0,82us
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    
    BCF LATB, 5 ; éteindre le bit 5 de CMD_MATRIX
    ; on écrit 0,5 fois plus d'instructions pour atteindre 0,43us
    NOP
    NOP
    NOP
    
    RETURN
