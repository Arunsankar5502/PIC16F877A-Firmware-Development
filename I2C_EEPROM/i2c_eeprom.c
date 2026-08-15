
#include <xc.h>
#include "I2C_eeprom.h"
#define _XTAL_FREQ 16000000       // 16 MHz clock frequency

#define EEPROM_Address_R  0xA1    // EEPROM read address
#define EEPROM_Address_W  0xA0    // EEPROM write address


//---------------[ I2C Routines ]-------------------

// Initialize I2C Master
void I2C_Master_Init(const unsigned long baud)
{
    SSPCON = 0b00101000;           // Enable I2C Master mode
    SSPCON2 = 0;                   // Clear control register
    SSPADD = (_XTAL_FREQ/(4*baud))-1; // Set I2C clock frequency
    SSPSTAT = 0;                   // Clear status register

    TRISC3 = 1;                    // RC3 as SCL input
    TRISC4 = 1;                    // RC4 as SDA input
}

// Wait until I2C bus is free
void I2C_Master_Wait()
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

// Generate I2C Start condition
void I2C_Master_Start()
{
    I2C_Master_Wait();
    SEN = 1;                       // Start condition
}

// Generate I2C Repeated Start condition
void I2C_Master_RepeatedStart()
{
    I2C_Master_Wait();
    RSEN = 1;                      // Repeated Start condition
}

// Generate I2C Stop condition
void I2C_Master_Stop()
{
    I2C_Master_Wait();
    PEN = 1;                       // Stop condition
}

// Send one byte through I2C
unsigned char I2C_Master_Write(unsigned char data)
{
    I2C_Master_Wait();

    SSPBUF = data;                 // Load data into buffer

    while(!SSPIF);                 // Wait until transmission completes
    SSPIF = 0;                     // Clear interrupt flag

    return ACKSTAT;                // Return ACK status
}

// Read one byte through I2C
unsigned char I2C_Read_Byte(void)
{
    I2C_Master_Wait();

    RCEN = 1;                      // Enable data reception

    while(!SSPIF);                 // Wait until reception completes
    SSPIF = 0;                     // Clear interrupt flag

    I2C_Master_Wait();

    return SSPBUF;                 // Return received data
}

// Send ACK to slave
void I2C_ACK(void)
{
    ACKDT = 0;                     // 0 = ACK
    I2C_Master_Wait();
    ACKEN = 1;                     // Send ACK
}

// Send NACK to slave
void I2C_NACK(void)
{
    ACKDT = 1;                     // 1 = NACK
    I2C_Master_Wait();
    ACKEN = 1;                     // Send NACK
}


//---------------[ I2C EEPROM Routines ]----------------


// Write one byte to EEPROM
void EEPROM_Write(unsigned int add, unsigned char data)
{
    I2C_Master_Start();            // Start I2C communication

    // Wait until EEPROM is ready
    while(I2C_Master_Write(EEPROM_Address_W))
        I2C_Master_RepeatedStart();

    I2C_Master_Write(add>>8);       // Send address high byte
    I2C_Master_Write((unsigned char)add); // Send address low byte
    I2C_Master_Write(data);         // Send data

    I2C_Master_Stop();              // Stop I2C communication
    __delay_ms(10);                 // Wait for EEPROM write
}


// Write multiple bytes to EEPROM
void EEPROM_Write_Page(unsigned int add, unsigned char* data, unsigned char len)
{
    I2C_Master_Start();             // Start I2C communication

    // Wait until EEPROM is ready
    while(I2C_Master_Write(EEPROM_Address_W))
        I2C_Master_RepeatedStart();

    I2C_Master_Write(add>>8);       // Send address high byte
    I2C_Master_Write((unsigned char)add); // Send address low byte

    for(unsigned int i=0; i<len; i++)
        I2C_Master_Write(data[i]);  // Send each data byte

    I2C_Master_Stop();              // Stop I2C communication
    __delay_ms(10);                 // Wait for EEPROM write
}


//-----------------------------
//-----[ Read Operations ]-----


// Read one byte from EEPROM
unsigned char EEPROM_Read(unsigned int add)
{
    unsigned char Data;

    I2C_Master_Start();             // Start I2C communication

    // Wait until EEPROM is ready
    while(I2C_Master_Write(EEPROM_Address_W))
        I2C_Master_RepeatedStart();

    I2C_Master_Write(add>>8);       // Send address high byte
    I2C_Master_Write((unsigned char)add); // Send address low byte

    I2C_Master_RepeatedStart();     // Restart for read operation

    I2C_Master_Write(EEPROM_Address_R); // Send EEPROM read address

    Data = I2C_Read_Byte();         // Read data from EEPROM

    I2C_NACK();                     // Send NACK after reading
    I2C_Master_Stop();              // Stop I2C communication

    return Data;                    // Return EEPROM data
}


// Read multiple bytes from EEPROM
void EEPROM_Read_Page(unsigned int add, unsigned char* data, unsigned int len)
{
    I2C_Master_Start();             // Start I2C communication

    // Wait until EEPROM is ready
    while(I2C_Master_Write(EEPROM_Address_W))
        I2C_Master_RepeatedStart();

    I2C_Master_Write(add>>8);       // Send address high byte
    I2C_Master_Write((unsigned char)add); // Send address low byte

    I2C_Master_RepeatedStart();     // Restart for read operation
    I2C_Master_Write(EEPROM_Address_R); // Send EEPROM read address

    for(unsigned int i=0; i<len; i++)
    {
        data[i] = I2C_Read_Byte();  // Read each byte
        I2C_ACK();                  // Send ACK for next byte
    }

    I2C_Master_Stop();              // Stop I2C communication
}
