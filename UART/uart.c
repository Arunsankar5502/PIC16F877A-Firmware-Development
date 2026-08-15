
// PIC16F877 Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config CP = OFF         // FLASH Program Memory Code Protection bits (Code protection off)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low Voltage In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection (Code Protection off)
#pragma config WRT = OFF        // FLASH Program Memory Write Enable (Unprotected program memory may not be written to by EECON control)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//---------------Master code------------------------------

#include <xc.h>
#define _XTAL_FREQ 6000000
unsigned char value;// Global variable
void uart_init();// Function declaration
void main() 
{
uart_init();// Initialized UART
while(1)
{
value = PORTB;
switch(value)
   {
       case 0xE0: TXREG='A';// transmitted data A
       __delay_ms(100);
       break;
       case 0xD0: TXREG='B';// transmitted data A
       __delay_ms(100);
       break;
       case 0xB0: TXREG='C';// transmitted data A
       __delay_ms(100);
       break;
       case 0x70: TXREG='D';// transmitted data A
       __delay_ms(100);
       break;
    }
}
  
}
void uart_init()
{
//-------------pin configuration--------------------

    TRISB=0xF0;// PORTB as output
    
    PORTB=0x00;;// initialize RB0 to RB7 is low
    
    OPTION_REG&=0x7F;// internal pull up on
    
//-------------UART configuration--------------------

    TRISC=0xC0;//1100 0000  RC(TX) =1 RC7(RX)=1
    
    TXSTA=0x20;//0010 0000   enable tranmission (TXREN =1),Asynchronous mode
    
    SPBRG=0x09;//0000 1001   baud rate setting for 9600 @6MHz
    
    RCSTA=0x90;//1001 0000  enable serial port and continuous receive 
    
}



//--------------Slave code---------------------------------


#include <xc.h>
#define _XTAL_FREQ 6000000
unsigned char rx_reg;// Global variable
void uart_init();// Function declaration
void main() {
    uart_init();// Initialized UART
    while(1)
    {
        if(PIR1& 0x20)// check if UART receiver interrupt flag is set
        {
        rx_reg=RCREG;// read the received date
        switch(rx_reg)
        {
            case 'A':
            PORTD=0x40;// RD6 on RD1 OFF
            break;
            case 'B':
            PORTD=0x02;// RD6 off RD1 ON
            break;
            case 'C':
            PORTD=0x00;// RD6 off RD1 OFF
            break;
            case 'D':
            PORTD=0x42;// RD6 on RD1 ON
            break;
        }
        }
    }
  
}
void uart_init()
{
    
//---------------- pin configuration------------------

    TRISD=0x00;// PORTD as output
    
    PORTD=0x00;// initialize RD0 to RD7 is low
    
//------------------ UART configuration---------------------

    TRISC=0xC0;//1100 0000 RC(TX) =1 RC7(RX)=1
    
    TXSTA=0x20;// enable tranmission (TXREN =1),Asynchronous mode
    
    SPBRG=0x09;// baud rate setting for 9600 @6MHz
    
    RCSTA=0x90;// enable serial port and continuous receive  
}
