/*
 * File:   LAB10.c
 * Author: Jos� Sanoli
 *
 * Created on 19 de octubre de 2021, 15:06
 */

//---------------------Bits de configuraci�n-------------------------------
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


//-----------------Librer�as utilizadas en c�digo-------------------- 
#include <xc.h>
#include <stdint.h>
#include <stdio.h>

//-----------------Definici�n de frecuencia de cristal---------------
#define _XTAL_FREQ 8000000

//-----------------------Constantes----------------------------------
#define  valor_tmr0 156                        // valor_tmr0 = 156 (0.05 ms)

//-----------------------Variables------------------------------------
const char data = 10;
char cont = 0;
int limite = 0;

//------------Funciones sin retorno de variables----------------------
void setup(void);                               // Funci�n de setup
void divisor(void);                             // Funci�n para dividir n�meros en d�gitos
void tmr0(void);                                // Funci�n para reiniciar TMR0
void displays(void);                            // Funci�n para alternar valores mostrados en displays

//-------------Funciones que retornan variables-----------------------
int tabla(int a);                              // Tabla para traducir valores a displays de 7 segmentos
int tabla_p(int a);                            // Tabla que traduce valores a displays de 7 segmentos pero con punto decimal incluido

//----------------------Interrupciones--------------------------------
void __interrupt() isr(void){
    if(PIR1bits.RCIF){
        PORTD = RCREG;
    }
    
}

//----------------------Main Loop--------------------------------
void main(void) {
    setup();                                    // Subrutina de setup
    while(1){
        __delay_ms(500);
        
        if(PIR1bits.TXIF){
            TXREG = data;
        }
    }
}

//----------------------Subrutinas--------------------------------
void setup(void){
    
    //Configuraci�n de entradas y salidas
    ANSEL = 0;                                  // Ningun pin anal�gico
    ANSELH = 0;                                 // Pines digitales
    
    TRISD = 0;                                  // PORTD como salida                           
    
    PORTD = 0;                                  // Limpiar PORTD
    
    //Configuraci�n del oscilador
    OSCCONbits.IRCF = 0b111;                    // Oscilador a 8 MHz
    OSCCONbits.SCS = 1;                         // Oscilador interno
    
    //Configuraci�n de TX y RX
    TXSTAbits.SYNC = 0;                         // Transmisi�n as�ncrona
    TXSTAbits.BRGH = 0;                         // Baud rate a velocidad baja
    
    BAUDCTLbits.BRG16 = 1;                      // Baud rate de 16 bits
    
    SPBRG = 25;                                 // SPBRG:SPBRGH = 25
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;                         // Puertos seriales habilitados
    RCSTAbits.RX9 = 0;                          // Recepci�n de datos de 8 bits 
    RCSTAbits.CREN = 1;                         // Recepci�n continua habilitada
    
    TXSTAbits.TXEN = 1;                         // Transmisiones habilitadas
    
    //Configuraci�n de interrupciones
    PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1;
    INTCONbits.GIE = 1;                         // Habilitar interrupciones globales
    INTCONbits.PEIE = 1;                        // Interrupciones perif�ricas activadas
    
    return;
}

void tmr0(void){
    INTCONbits.T0IF = 0;                        // Limpiar bandera de TIMER 0
    TMR0 = valor_tmr0;                          // TMR0 = 255
    return;
}

