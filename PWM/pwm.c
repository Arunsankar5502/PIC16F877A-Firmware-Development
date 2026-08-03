
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
unsigned char pwm20_high,pwm20_low;
unsigned char pwm60_high,pwm60_low;
unsigned char pwm90_high,pwm90_low;
void PWM_Int(void);
void PWM_Dutycycle_Update(void);
void main() {
    PWM_Int();
    while(1)
    {
     PWM_Dutycycle_Update();
    }
 
}
void PWM_Int(void)
{
    TRISC=0xFB;
    CCP1CON=0x0C;
    PR2=0x2E;
    T2CON=0x06;
    pwm20_high=0x09;
    pwm20_low=0x02;
    pwm60_high=0x1C;
    pwm60_low=0x01;
    pwm90_high=0x2A;
    pwm90_low=0x01;
    
    
}
void PWM_Dutycycle_Update(void)
{
    CCPR1L=pwm20_high; // 20% high 8 bits
    CCP1CON=((CCP1CON & 0xCF)|(pwm20_low << 4)); // 20% low 2 bits
    __delay_ms(3000);
    CCPR1L=pwm60_high; // 50% high 8 bits
    CCP1CON=((CCP1CON & 0xCF)|(pwm60_low << 4)); // 60% low 2 bits
    __delay_ms(3000);
    CCPR1L=pwm90_high; // 80% high 8 bits
    CCP1CON=((CCP1CON & 0xCF)|(pwm90_low << 4)); // 90% low 2 bits
    __delay_ms(3000);
}
