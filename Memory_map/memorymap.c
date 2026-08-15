
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

// Register access macros
#define DIR_B  *direction_register_b
#define DA_B   *data_register_b
#define DIR_C  *direction_register_c
#define DA_C   *data_register_c
#define PULLUP *pull_up_register

unsigned char value;

// Register addresses
unsigned char DIR_B =(unsigned char *)0x86;   // PORTB direction register
unsigned char DA_B =(unsigned char *)0x06;    // PORTB data register
unsigned char DIR_C =(unsigned char *)0x87;   // PORTC direction register
unsigned char DA_C  =(unsigned char *)0x07;   // PORTC data register
unsigned char PULLUP =(unsigned char *)0x81;  // PORTB pull-up register

void main(void) 
{
   DIR_B=0xF0;                  // RB7-RB4 input, RB3-RB0 output
   DIR_C=0x00;                  // Set PORTC as output
   DA_B=0x00;                   // Clear PORTB
   DA_C=0x00;                   // Clear PORTC
   PULLUP &=0x7F;               // Enable PORTB internal pull-ups

    while(1)
    {
        value= DA_B;            // Read PORTB value

        switch(value)
        {
             case 0xE0:       
             DA_C=0x02;         // Set PORTC output
             break;

             case 0xD0:         
             DA_C=0x40;         // Set PORTC output
             break;

             case 0xB0:        
             DA_C=0x42;         // Set PORTC output
             break;

             case 0x70:         
             DA_C=0x00;         // Clear PORTC
             break;
        }
    }
}
