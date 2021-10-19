/*
 * File:   LAB10.c
 * Author: José Sanoli
 *
 * Created on 19 de octubre de 2021, 15:06
 */

//---------------------Bits de configuración-------------------------------
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


//-----------------Librerías utilizadas en código-------------------- 
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//-----------------Definición de frecuencia de cristal---------------
#define _XTAL_FREQ 1000000

//-----------------------Constantes----------------------------------

//-----------------------Variables------------------------------------
char i = 0;
const char data[] = "Bienvenido :)";

//------------Funciones sin retorno de variables----------------------
void setup(void);                               // Función de setup
void env_term(void);                            // Función para enviar cadena a terminal

//----------------------Interrupciones--------------------------------
void __interrupt() isr(void){
    if(PIR1bits.RCIF){                          // Si la bandera de interrupción de recepción es 1
        PORTB = RCREG;                          // PORTB = valor recibido
    }
    
}

//----------------------Main Loop--------------------------------
void main(void) {
    setup();                                    // Subrutina de setup
    while(1){
        __delay_ms(500);
        env_term();                             // Subrutina para enviar lineas de caracteres a terminal
    }
}

//----------------------Subrutinas--------------------------------
void setup(void){
    
    //Configuración de entradas y salidas
    ANSEL = 0;                                  // Ningun pin analógico
    ANSELH = 0;                                 // Pines digitales
    
    TRISD = 0;                                  // PORTD como salida  
    TRISB = 0;                                  // PORTB como salida
    
    PORTD = 0;                                  // Limpiar PORTD
    PORTB = 0;                                  // Limpiar PORTB
    
    //Configuración del oscilador
    OSCCONbits.IRCF = 0b100;                    // Oscilador a 1 MHz
    OSCCONbits.SCS = 1;                         // Oscilador interno
    
    //Configuración de TX y RX
    TXSTAbits.SYNC = 0;                         // Transmisión asíncrona
    TXSTAbits.BRGH = 1;                         // Baud rate a velocidad baja
    
    BAUDCTLbits.BRG16 = 1;                      // Baud rate de 16 bits
    
    SPBRG = 25;                                 // SPBRG:SPBRGH = 25
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;                         // Puertos seriales habilitados
    RCSTAbits.RX9 = 0;                          // Recepción de datos de 8 bits 
    RCSTAbits.CREN = 1;                         // Recepción continua habilitada
    
    TXSTAbits.TXEN = 1;                         // Transmisiones habilitadas
    
    //Configuración de interrupciones
    PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1;
    INTCONbits.GIE = 1;                         // Habilitar interrupciones globales
    INTCONbits.PEIE = 1;                        // Interrupciones periféricas activadas
    
    return;
}

void env_term(void){
    while (i < 14){                             // Mientras el valor de i sea menor al largo de la cadena enviada, evaluar instrucción
        if (PIR1bits.TXIF){                     // Si la bandera TXIF es 1, entonces
            for(i = 0; i<14; i++){              // For loop que recorra el largo de la cadena
                __delay_ms(100);
                TXREG = data[i];                // TXREG = dato i del array "data"
            }
        }
    }
}


