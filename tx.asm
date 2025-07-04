#include <xc.inc>

; When assembly code is placed in a psect, it can be manipulated as a
; whole by the linker and placed in memory.
psect   txfunc,local,class=CODE,reloc=2 ; PIC18's should have a reloc (alignment) flag of 2 for any psect which contains executable code.

; -----------------------------------------------------------------
; GLOBALS
;
; Déclaration de fonctions et variables globales permettant au code C et à l'asm de les partager
; Une même fonction ou variable côté asm est préfixée par un underscore, et ne l'est pas côté C
; Avec ce formalisme, elles sont utilisables de façon intercangeable et transparente :
; | ---- asm ----- | ------------- C ----------------- |
; | _TX_64LEDS  <--|--> void TX_64LEDS(void)           |
; | _pC         <--|--> volatile const char * pC       |
; | _LED_MATRIX <--|--> volatile char LED_MATRIX [256] |

; Fonction globales
global _TX_64LEDS ; Fonction définie dans tx.asm ; Fonction permettant d'envoyer la commande pour piloter les 64 LEDs, telle que décrite dans LED_MATRIX

; Constantes/variables globales
global _pC         ; Constante définie dans main.c ; Pointeur vers LED_MATRIX
global _LED_MATRIX ; Variable  définie dans main.c ; Tableau (256 octets = 64 x 4) des composantes RGBW de la matrice LED (1 octet/couleur/LED)

SEND_0 MACRO
    ; Le signal "0" correspond à :
    ; HAUT pendant 0,32us
    ; BAS pendant 0,93us

    BSF LATB, 5 ; allumer le bit de contrôle de la matrice
    ; on doit envoyer le signal pendant 0,32us
    ; (nombre de NOP déterminé expérimentalement)
    NOP
    NOP
    NOP
    NOP

    BCF LATB, 5 ; éteindre le bit de contrôle de la matrice
    ; on doit envoyer le signal pendant 0,93us
    NOP
    NOP
    NOP
    NOP
    NOP
ENDM   

    
SEND_1 MACRO
    ; Le signal "1" correspond à :
    ; HAUT pendant 0,82us
    ; BAS pendant 0,43us

    BSF LATB, 5 ; allumer le bit de contrôle de la matrice
    ; on doit envoyer le signal pendant 0,82us
    ; (nombre de NOP déterminé expérimentalement)
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP

    BCF LATB, 5 ; éteindre le bit de contrôle de la matrice
    ; on doit envoyer le signal pendant 0,43us
    ; pas besoin de NOP car les tests/goto prennent déjà ce temps à s'exécuter
ENDM
      
    
; on fait une "fonction" plutôt qu'une macro pour éviter la collision de labels
SEND_OCTET: 
    ; Incrémente de un octet
    ; Dans l'exécution de l'instruction suivante, la valeur pointée par <FSR0H-FSR0L> est chargée dans WREG, et <FSR0H-FSR0L> est incrémenté :
    MOVF POSTINC0, 0, 0
    
    ; Les if/elses suivant sont optimisés pour prendre le moins de temps possible.
    ; Ce temps gagné est primordial car on ne peut pas enlever de NOP dans la macro SEND_1. 
    ; Il faut rogner du temps ici.
    ; On n'a pas trouvé meilleure façon de faire des if/else jusqu'à présent :
    ; - équilibré (autant de GOTO dans le if que dans le else)
    ; - rapide : 1 test de bit + 1 GOTO dans chaque cas.

    BTFSC WREG, 7 ; saute l'instruction suivante si le bit==0
    GOTO B7_SEND_1 ; si le bit==1
    SEND_0
    GOTO END_B7
    B7_SEND_1:
    SEND_1
    END_B7:

    BTFSC WREG, 6 ; saute l'instruction suivante si le bit==0
    GOTO B6_SEND_1 ; si le bit==1
    SEND_0
    GOTO END_B6
    B6_SEND_1:
    SEND_1
    END_B6:

    BTFSC WREG, 5 ; saute l'instruction suivante si le bit==0
    GOTO B5_SEND_1 ; si le bit==1
    SEND_0
    GOTO END_B5
    B5_SEND_1:
    SEND_1
    END_B5:
 
    BTFSC WREG, 4 ; saute l'instruction suivante si le bit==0
    GOTO B4_SEND_1 ; si le bit==1
    SEND_0
    GOTO END_B4
    B4_SEND_1:
    SEND_1
    END_B4:

    BTFSC WREG, 3 ; saute l'instruction suivante si le bit==0
    GOTO B3_SEND_1 ; si le bit==1
    SEND_0
    GOTO END_B3
    B3_SEND_1:
    SEND_1
    END_B3:

    BTFSC WREG, 2 ; saute l'instruction suivante si le bit==0
    GOTO B2_SEND_1 ; si le bit==1
    SEND_0
    GOTO END_B2
    B2_SEND_1:
    SEND_1
    END_B2:
    
    BTFSC WREG, 1 ; saute l'instruction suivante si le bit==0
    GOTO B1_SEND_1 ; si le bit==1
    SEND_0
    GOTO END_B1
    B1_SEND_1:
    SEND_1
    END_B1:

    BTFSC WREG, 0 ; saute l'instruction suivante si le bit==0
    GOTO B0_SEND_1 ; si le bit==1
    SEND_0
    GOTO END_B0
    B0_SEND_1:
    SEND_1
    END_B0:
    
    RETURN

_TX_64LEDS:
    ; Cette fonction envoie sur CMD_MATRIX l'intégralité de la matrice LED_MATRIX,
    ; Chaque bit de chaque octet encodé en largeur d'impulsion

    ; Place un pointeur au début de la matrice LED_MATRIX
    ; Voir section 10.8.12 (p. 150) de la datasheet PIC18F25K40
    MOVFF _pC + 0, WREG ; Charge le LSB du pointeur de LED_MATRIX dans WREG
    MOVWF FSR0L, 0      ; Définit le LSB du registre d'adressage indirect
    MOVFF _pC + 1, WREG ; Charge le MSB du pointeur de LED_MATRIX dans WREG
    MOVWF FSR0H, 0      ; Définit le MSB du registre d'adressage indirect
    
    BANKSEL LATB ; sélectionner CMD_MATRIX
    
    REPT 256
	CALL SEND_OCTET
    ENDM