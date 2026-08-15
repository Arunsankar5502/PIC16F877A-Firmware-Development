
// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.




#include <xc.h>                
#define _XTAL_FREQ 6000000      // 6 MHz clock frequency

unsigned char value;            // Stores PORTB input value

void main()
{
    TRISB=0xF0;                 // RB7-RB4 input, RB3-RB0 output
    PORTB=0x00;                 // Clear PORTB

    TRISC=0x00;                 // Set PORTC as output
    PORTC=0x00;                 // Clear PORTC

    TRISD=0x00;                 // Set PORTD as output
    PORTD=0x00;                 // Clear PORTD

    OPTION_REG &= 0x7F;         // Enable PORTB internal pull-ups
    INTCON &= ~(0x01);          // Clear RB port-change interrupt flag
    INTCON |=0x88;              // Enable global and RB port-change interrupts
    
    while(1)
    {
        PORTD=0xFF;             // Turn ON PORTD
        __delay_ms(500);        // Delay 500 ms

        PORTD=0x00;             // Turn OFF PORTD
        __delay_ms(500);        // Delay 500 ms
    }
}


// Interrupt Service Routine
void __interrupt() _ISR(void)
{
    if(INTCON & 0x01)           // Check PORTB change interrupt flag
    {
        value=PORTB;            // Read PORTB value

        if(value == 0xE0)       // Check input condition
        {
            PORTC= ~PORTC;      // Toggle PORTC output
        }

        INTCON &= ~(0x01);      // Clear interrupt flag
    }
}
