
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
#include <stdint.h>

 
void SPI_Slave_Init();             // Initialize SPI in Slave mode
uint8_t SPI_Read();                // Read data from SPI

uint8_t Data;                      // Global data variable


void main(void) 
{
    SPI_Slave_Init();              // Initialize SPI slave

    TRISB = 0x00;                  // Set PORTB as output

    while(1)
    {
        PORTB = Data;              // Display received data on PORTB
    }

    return;
}


void SPI_Slave_Init()
{
    // Set SPI Slave mode with SS enabled
    SSPM0 = 0;
    SSPM1 = 0;
    SSPM2 = 1;
    SSPM3 = 0;

    SSPEN = 1;                     // Enable SPI module

    // SPI clock polarity and phase
    CKP = 0;
    CKE = 0;

    SMP = 0;                       // Clear sample bit

    // Configure SPI pins
    TRISC5 = 0;                    // RC5 (SDO) as output
    TRISC4 = 1;                    // RC4 (SDI) as input
    TRISC3 = 1;                    // RC3 (SCK) as input

    // Configure SS pin as digital input
    PCFG3 = 0;
    PCFG2 = 1;
    PCFG1 = 0;
    PCFG0 = 0;
    TRISA5 = 1;                    // RA5 (SS) as input

    // Enable SPI interrupts
    SSPIE = 1;
    PEIE = 1;
    GIE = 1;
}


uint8_t SPI_Read()                   // Read SPI data
{
    uint8_t Data;

    if(BF)                            // Check if new data is received
    {
        Data = SSPBUF;                // Read received data
        BF = 0;                       // Clear buffer flag
        SSPIF = 0;                    // Clear interrupt flag
        SSPOV = 0;                    // Clear overflow flag

        return Data;                  // Return received data
    }
}


void __interrupt() ISR(void)
{
    if(SSPIF)                         // Check SPI interrupt flag
    {
        Data = SSPBUF;                // Read received data
        SSPIF = 0;                    // Clear interrupt flag
    }
}
