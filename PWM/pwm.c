
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
#define _XTAL_FREQ 6000000

unsigned char pwm10_high,pwm10_low;
unsigned char pwm50_high,pwm50_low;
unsigned char pwm80_high,pwm80_low;

void pwm_int(void);             // PWM initialization
void pwm_dutycycle_update(void); // Update PWM duty cycle

void main() {
    pwm_int();                  // Initialize PWM

    while(1)
    {
        pwm_dutycycle_update(); // Change PWM duty cycle
    }
 
}

void pwm_int(void)
{
    TRISC=0xFB;                 // Configure RC2 as PWM output
    CCP1CON=0x0C;               // Set CCP1 in PWM mode
    PR2=0x5E;                   // Set PWM period
    T2CON=0x06;                 // Enable Timer2 with prescaler

    // 10% duty cycle
    pwm10_high=0x09;
    pwm10_low=0x02;

    // 50% duty cycle
    pwm50_high=0x2F;
    pwm50_low=0x00;

    // 80% duty cycle
    pwm80_high=0x4B;
    pwm80_low=0x00;
    
}

void pwm_dutycycle_update(void)
{
    // Set 10% duty cycle
    CCPR1L=pwm10_high;
    CCP1CON=((CCP1CON & 0xCF)|(pwm10_low << 4));
    __delay_ms(3000);           // Wait 3 seconds

    // Set 50% duty cycle
    CCPR1L=pwm50_high;
    CCP1CON=((CCP1CON & 0xCF)|(pwm50_low << 4));
    __delay_ms(3000);           // Wait 3 seconds

    // Set 80% duty cycle
    CCPR1L=pwm80_high;
    CCP1CON=((CCP1CON & 0xCF)|(pwm80_low << 4));
    __delay_ms(3000);           // Wait 3 seconds
}
