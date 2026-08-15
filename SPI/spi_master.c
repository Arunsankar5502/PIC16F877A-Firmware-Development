
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
#define _XTAL_FREQ 4000000       // 4 MHz clock frequency

// IO Pin Definitions
#define UP RB0                    // UP button
#define Down RB1                  // DOWN button
#define Send RB2                  // SEND button

// Function Declarations
void SPI_Master_Init();           // Initialize SPI in Master mode
void SPI_Write(uint8_t);          // Send data through SPI


// Main Routine
void main(void)
{
    // Initialize SPI in Master mode at Fosc/64
    SPI_Master_Init();

    uint8_t Data = 0;             // Data value

    TRISB = 0x07;                 // RB0, RB1 and RB2 as inputs
    TRISD = 0x00;                 // Set PORTD as output
    PORTD = 0x00;                 // Clear PORTD

    while(1)
    {
        if (UP)                   // Check UP button
        {
            Data++;               // Increase data value
            __delay_ms(250);      // Button delay
        }

        if (Down)                 // Check DOWN button
        {
            Data--;               // Decrease data value
            __delay_ms(250);      // Button delay
        }

        if (Send)                 // Check SEND button
        {
            SPI_Write(Data);      // Send data through SPI
            __delay_ms(250);      // Button delay
        }

        PORTD = Data;             // Display data on PORTD
    }

    return;
}


// SPI Master Initialization
void SPI_Master_Init()
{
    // Select SPI Master mode with Fosc/64 clock
    SSPM0 = 0;
    SSPM1 = 0;
    SSPM2 = 0;
    SSPM3 = 0;

    SSPEN = 1;                    // Enable SPI module

    // SPI clock polarity and phase
    CKP = 0;
    CKE = 0;

    SMP = 0;                      // Sample data at middle

    // Configure SPI pins
    TRISC5 = 0;                   // RC5 (SDO) as output
    TRISC4 = 1;                   // RC4 (SDI) as input
    TRISC3 = 0;                   // RC3 (SCK) as output
}


// SPI Write Function
void SPI_Write(uint8_t Data)
{
    SSPBUF = Data;                // Load data into SPI buffer
}
