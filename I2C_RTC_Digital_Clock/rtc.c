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
#define RS RC2
#define EN RC1
#define _XTAL_FREQ 20000000       // 20 MHz clock frequency

// Function declarations
void init(void);                  // Initialize LCD and GPIO
void i2c_init(const unsigned long); // Initialize I2C
void i2c_start();                 // Generate I2C start condition
int i2c_read(int);                // Read data from I2C
void i2c_wait();                  // Wait for I2C bus
void i2c_stop();                  // Generate I2C stop condition
int i2c_write(unsigned char);     // Write data to I2C
void LCD_Position(unsigned char); // Send LCD command
void LCD_Input(unsigned char);    // Send LCD data
int bcd_2_dec(int);               // Convert BCD to decimal
int dec_2_bcd(int);               // Convert decimal to BCD
void settime(void);               // Set time in RTC
void update(void);                // Read time from RTC

// Variable declarations
unsigned char msg1[5]={"TIME:"};  // LCD time label
unsigned char msg2[5]={"DATE:"};  // LCD date label

int i,j,k,l;

unsigned char sec=56;             // Initial seconds
unsigned char min=59;             // Initial minutes
unsigned char hour=23;            // Initial hours
unsigned char date=28;            // Initial date
unsigned char month=12;           // Initial month
unsigned int year=25;             // Initial year

// Variables for individual digits
unsigned char sec1,sec2,min1,min2,hour1,hour2;
unsigned char date1,date2,month1,month2,year1,year2;


void main()
{
    init();                        // Initialize LCD and GPIO
    i2c_init(100);                 // Initialize I2C at 100 kHz
    settime();                     // Set initial RTC time

    while(1)
    {
        update();                  // Read current time from RTC

        // Break each value into two digits for LCD
        sec1=sec/10;
        sec2=sec%10;

        min1=min/10;
        min2=min%10;

        hour1=hour/10;
        hour2=hour%10;

        date1=date/10;
        date2=date%10;

        month1=month/10;
        month2=month%10;

        year1=year/10;
        year2=year%10;
        
        // Display TIME:HH-MM-SS on LCD line 1
        LCD_Position(0x80);

        for(i=0;i<5;i++)
            LCD_Input(msg1[i]);

        LCD_Input(hour1+'0');
        LCD_Input(hour2+'0');
        LCD_Input(0x2D);            // '-'
        LCD_Input(min1+'0');
        LCD_Input(min2+'0');
        LCD_Input(0x2D);            // '-'
        LCD_Input(sec1+'0');
        LCD_Input(sec2+'0');

        // Display DATE:DD-MM-YY on LCD line 2
        LCD_Position(0xC0);

        for(i=0;i<5;i++)
            LCD_Input(msg2[i]);

        LCD_Input(date1+'0');
        LCD_Input(date2+'0');
        LCD_Input(0x2D);            // '-'
        LCD_Input(month1+'0');
        LCD_Input(month2+'0');
        LCD_Input(0x2D);            // '-'
        LCD_Input(year1+'0');
        LCD_Input(year2+'0');

        __delay_ms(500);            // Update display every 500 ms
    }
}


void init(void)
{
    TRISC=0x18;                     // RC3(SCL) and RC4(SDA) as input
    TRISD=0x00;                     // Set PORTD as output
    PORTD=0x00;                     // Clear PORTD
  
    // LCD initialization
    LCD_Position(0x30);             // LCD initialization
    __delay_ms(100);

    LCD_Position(0x30);
    __delay_ms(100);

    LCD_Position(0x30);
    __delay_ms(100);

    LCD_Position(0x38);             // 8-bit mode, 2-line display
    __delay_ms(100);

    LCD_Position(0x06);             // Increment cursor
    __delay_ms(100);

    LCD_Position(0x0C);             // Display ON, cursor OFF
    __delay_ms(100);

    LCD_Position(0x01);             // Clear display
    __delay_ms(100);
}


void LCD_Position(unsigned char i)
{
    RS=0;                           // Select command mode
    PORTD=i;                        // Send command
    EN=1;                           // Enable LCD
    EN=0;
    __delay_ms(100);
}


void LCD_Input(unsigned char i)
{
    RS=1;                           // Select data mode
    PORTD=i;                        // Send data
    EN=1;                           // Enable LCD
    EN=0;
    __delay_ms(100);
}


void i2c_init(const unsigned long feq_k)
{
    SSPCON=0x28;                    // Enable MSSP in I2C master mode
    SSPSTAT=0x00;                   // Clear status register
    SSPCON2=0x00;                   // Clear control register
    SSPADD= (_XTAL_FREQ/(4*feq_k*100))-1; // Set I2C clock
}


void i2c_wait()
{
    while(SSPCON2 & 0x1F || SSPSTAT & 0x04); // Wait for I2C bus idle
}


void i2c_stop()
{
    i2c_wait();                     // Wait for bus
    PEN=1;                           // Generate stop condition
}


void i2c_start()
{
    i2c_wait();                     // Wait for bus
    SEN=1;                           // Generate start condition
}


int i2c_read(int ack)
{
    int value;

    i2c_wait();                     // Wait for bus
    RCEN=1;                         // Enable receive mode
    i2c_wait();                     // Wait for data

    value=SSPBUF;                   // Read received data

    i2c_wait();                     // Wait for bus

    ACKDT=(ack)?0:1;                // Select ACK or NACK
    ACKEN=1;                        // Send ACK/NACK

    return value;                   // Return received data
}


int i2c_write(unsigned char temp)
{
    i2c_wait();                     // Wait for bus
    SSPBUF=temp;                    // Send data
}


int bcd_2_dec(int temp)
{
    return ((temp>>4)*10)+(temp&0x0F); // Convert BCD to decimal
}


int dec_2_bcd(int temp)
{
    return ((temp/10)<<4)+(temp%10);   // Convert decimal to BCD
}


void settime(void)
{
    i2c_start();                    // Start I2C communication

    i2c_write(0xD0);                // RTC write address
    i2c_write(0);                   // Set register pointer to 00h

    i2c_write(dec_2_bcd(sec));      // Write seconds
    i2c_write(dec_2_bcd(min));      // Write minutes
    i2c_write(dec_2_bcd(hour));     // Write hours

    i2c_write(1);                   // Skip day register

    i2c_write(dec_2_bcd(date));     // Write date
    i2c_write(dec_2_bcd(month));    // Write month
    i2c_write(dec_2_bcd(year));     // Write year

    i2c_stop();                     // Stop I2C communication
}


void update()
{
    i2c_start();                    // Start I2C communication

    i2c_write(0xD0);                // RTC write address
    i2c_write(0x00);                // Set pointer to seconds register

    i2c_stop();                     // Stop communication

    i2c_start();                    // Start read operation
    i2c_write(0xD1);                // RTC read address

    sec=(bcd_2_dec(i2c_read(1)));   // Read seconds
    min=(bcd_2_dec(i2c_read(1)));   // Read minutes
    hour=(bcd_2_dec(i2c_read(1)));  // Read hours

    (i2c_read(1));                  // Skip day register

    date=(bcd_2_dec(i2c_read(1)));  // Read date
    month=(bcd_2_dec(i2c_read(1))); // Read month
    year=(bcd_2_dec(i2c_read(1)));  // Read year

    i2c_stop();                     // Stop I2C communication

    i2c_start();                    // Start I2C communication
    i2c_write(0xD1);                // RTC read address
    i2c_read(1);                    // Read data
    i2c_stop();                     // Stop communication
}
