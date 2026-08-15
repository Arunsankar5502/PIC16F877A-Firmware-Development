
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

void adc_lcd_init();             
void lcd_number_convert(unsigned int i); 
void lcd_command(unsigned char i);      
void lcd_data(unsigned char i);        

unsigned int adc_calibrated_data,adc_volt,m,n;
unsigned char adc_high,adc_low,b[5];

void main() {
    adc_lcd_init();              // Initialize ADC and LCD

    while(1)
    {
        ADCON0 |=0x04;           // Start ADC conversion
        while(ADCON0 & 0x04);    // Wait until conversion completes

        adc_high=ADRESH;         // Read ADC high byte
        adc_low=ADRESL;          // Read ADC low byte

        adc_volt=(adc_high << 8)+adc_low; // Combine ADC result

        adc_calibrated_data=(adc_volt*48)/1024; // Convert ADC value

        lcd_command(0x80);       // Set LCD cursor position
        lcd_number_convert(adc_calibrated_data); // Display ADC value
    }
}

void adc_lcd_init()
{
    TRISC=0x00;                  // Set PORTC as output
    TRISD=0x00;                  // Set PORTD as output
    PORTD=0x00;                  // Clear PORTD
    PORTC=0x00;                  // Clear PORTC

    //------------ LCD INITIALIZATION ----------------------

    lcd_command(0x30);           // LCD initialization
    __delay_ms(100);

    lcd_command(0x30);           // LCD initialization
    __delay_ms(100);

    lcd_command(0x30);           // LCD initialization
    __delay_ms(100);

    lcd_command(0x38);           // 8-bit mode, 2-line display
    __delay_ms(100);

    lcd_command(0x06);           // Increment cursor position
    __delay_ms(100);

    lcd_command(0x0C);           // Display ON, cursor OFF
    __delay_ms(100);

    lcd_command(0x01);           // Clear LCD display
    __delay_ms(100);

    //----------- ADC INITIALIZATION ---------------------

    ADCON1=0x8E;                 // AN0 analog, other pins digital
    ADCON0=0x81;                 // Enable ADC, select AN0, FOSC/32
    __delay_ms(10);              // ADC acquisition time
}

void lcd_command(unsigned char i)
{
    PORTC&=~0x08;                // RS = 0 for command
    PORTD=i;                     // Send command to LCD

    PORTC|=0x01;                 // Enable = 1
    PORTC&=~0x01;                // Enable = 0

    __delay_ms(100);              // LCD delay
}

void lcd_data(unsigned char i)
{
    PORTC|=0x08;                 // RS = 1 for data
    PORTD=i;                     // Send data to LCD

    PORTC|=0x01;                 // Enable = 1
    PORTC&=~0x01;                // Enable = 0

    __delay_ms(100);              // LCD delay
}

void lcd_number_convert(unsigned int i )
{
    unsigned int s,a=1;
    m=i;

    // Separate each digit of the number
    while(m!=0)
    {
        s=m%10;                
        b[a]=s;                  
        a++;
        m=m/10;                  
    }

    b[a]='\0';                   
    a--;

    // Display two-digit value
    if(b[2]>0)
    {
        lcd_command(0x80);       
        lcd_data(0x30 + b[2]);  

        lcd_command(0x81);       
        lcd_data(0x30 + b[1]);   
    }
    else
    {
        lcd_command(0x80);       
        lcd_data(0x20);          // Display space

        lcd_command(0x81);       
        lcd_data(0x30 + b[1]);   
    }

    lcd_command(0x82);           
    lcd_data(0x76);              // Display 'v'
}
