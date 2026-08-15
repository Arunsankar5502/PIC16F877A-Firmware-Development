// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
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
#include "I2C_eeprom.h"
#define _XTAL_FREQ 16000000       // 16 MHz clock frequency

void main(void) {

    I2C_Master_Init(100000);       // Initialize I2C at 100 kHz

    unsigned int Address=0x0020;   // EEPROM starting address
    unsigned char Data=0x04;       // Starting data value

    // Write data to EEPROM
    EEPROM_Write(Address++, Data++); // Write 0x04 at 0x0020
    EEPROM_Write(Address++, Data++); // Write 0x05 at 0x0021
    EEPROM_Write(Address, Data);     // Write 0x06 at 0x0022

    __delay_ms(10);                 // Wait for EEPROM write to complete

    Address = 0x0020;               // Start reading from 0x0020

    TRISD = 0x00;                   // Set PORTD as output
    PORTD = EEPROM_Read(Address++); // Read 0x04
    __delay_ms(1000);               // Delay 1 second

    PORTD = EEPROM_Read(Address++); // Read 0x05
    __delay_ms(1000);               // Delay 1 second

    PORTD = EEPROM_Read(Address);   // Read 0x06

    while(1)
    {
        // Keep the program running
    }

    return;
}
