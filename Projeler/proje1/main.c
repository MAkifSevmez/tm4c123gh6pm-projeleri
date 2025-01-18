#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/gpio.c"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.c"

#define LCD_PORT_ENABLE SYSCTL_PERIPH_GPIOB
#define LCD_PORT GPIO_PORTB_BASE

#define LCD_DATA 0xF0               //LCD ports-> PB4-b4, PB5-b5, PB6-b6, PB7-b7
#define RS 0x01                     //RS -> PB0
#define RW 0x02                     //RW -> PB1
#define EN 0x04                     //EN -> PB2


void setup();
void LCDCommandWrite(unsigned char Command);
void delayMs(int n);
void delayUs(int n);

int main(void)
{
    setup();
    delayMs(100);
    LCDCommandWrite(0x01);

    GPIOPinWrite(LCD_PORT, LCD_DATA, 0x40);
    GPIOPinWrite(LCD_PORT, RS | RW | EN, 0x05);
    delayUs(20);
    GPIOPinWrite(LCD_PORT, EN, 0x00);

    GPIOPinWrite(LCD_PORT, LCD_DATA, 0xD0);
    GPIOPinWrite(LCD_PORT, RS | RW | EN, 0x05);
    delayUs(20);
    GPIOPinWrite(LCD_PORT, EN, 0x00);



}

void setup() {
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,0xFF);

    delayMs(20);
    GPIOPinWrite(LCD_PORT, LCD_DATA, 0x30);
    GPIOPinWrite(LCD_PORT, RS | RW | EN, 0x04);
    delayUs(20);
    GPIOPinWrite(LCD_PORT, RS | RW | EN, 0x00);

    delayMs(5);
    GPIOPinWrite(LCD_PORT, LCD_DATA, 0x30);
    GPIOPinWrite(LCD_PORT, RS | RW | EN, 0x04);
    delayUs(20);
    GPIOPinWrite(LCD_PORT, RS | RW | EN, 0x00);

    delayUs(250);
    GPIOPinWrite(LCD_PORT, LCD_DATA, 0x30);
    GPIOPinWrite(LCD_PORT, RS | RW | EN, 0x04);
    delayUs(20);
    GPIOPinWrite(LCD_PORT, RS | RW | EN, 0x00);


    GPIOPinWrite(LCD_PORT, LCD_DATA, 0x20);
    GPIOPinWrite(LCD_PORT, RS | RW | EN, 0x04);
    delayUs(20);
    GPIOPinWrite(LCD_PORT, RS | RW | EN, 0x00);

    LCDCommandWrite(0x02);
}

void LCDCommandWrite(unsigned char Command)
{
    GPIOPinWrite(LCD_PORT, LCD_DATA, (Command & 0xf0));   // YAZDIRACAÐIN HARFÝ LCD DATASHEET BAKARAK BURADAKÝ DEÐERLERÝ DEÐÝÞTÝREREK DEÐÝÞTÝREBÝLÝRSÝN
    GPIOPinWrite(LCD_PORT, RS | RW | EN, 0x04);
    delayMs(25);
    GPIOPinWrite(LCD_PORT, RS | RW | EN, 0x00);

    GPIOPinWrite(LCD_PORT, LCD_DATA, ((Command & 0x0f) << 4));
    GPIOPinWrite(LCD_PORT, RS | RW | EN, 0x04);
    delayMs(25);
    GPIOPinWrite(LCD_PORT, RS | RW | EN, 0x00);
}


void delayMs(int n)
{
    volatile int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < 3180; j++)
        {
        }
}

void delayUs(int n)
{
    volatile int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < 3; j++)
        {
        }
}
