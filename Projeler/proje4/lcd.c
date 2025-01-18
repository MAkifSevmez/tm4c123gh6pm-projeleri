
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "lcd.h"

void otuzhexgonder() {
    SysCtlDelay(100000);
    GPIOPinWrite(LCDPORT, RS, 0); // RS = 0 (Komut)
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, 0x30); // 0x30 gönder
    GPIOPinWrite(LCDPORT, E, E); // Enable HIGH
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0); // Enable LOW
}

void lcdkomut(unsigned char c) {
    GPIOPinWrite(LCDPORT, RS, 0); // RS = 0 (Komut)

    // Üst nibble gönder
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0xF0));
    GPIOPinWrite(LCDPORT, E, E); // Enable HIGH
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0); // Enable LOW

    // Alt nibble gönder
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c << 4) & 0xF0);
    GPIOPinWrite(LCDPORT, E, E); // Enable HIGH
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0); // Enable LOW

    SysCtlDelay(2000); //
}

void lcdkarakteryaz(unsigned char c) {
    GPIOPinWrite(LCDPORT, RS, RS); // RS = 1 (Veri)

    // Üst nibble gönder
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0xF0));
    GPIOPinWrite(LCDPORT, E, E); // Enable HIGH
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0); // Enable LOW

    // Alt nibble gönder
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c << 4) & 0xF0);
    GPIOPinWrite(LCDPORT, E, E); // Enable HIGH
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0); // Enable LOW

    SysCtlDelay(2000);
}

void LCDilkayarlar() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(LCDPORT, RS | E | D4 | D5 | D6 | D7);


    GPIOPinWrite(LCDPORT, RS | E | D4 | D5 | D6 | D7, 0x00); // Tüm pinleri LOW yap
    SysCtlDelay(50000);


    otuzhexgonder();
    SysCtlDelay(50000);
    otuzhexgonder();
    SysCtlDelay(50000);
    otuzhexgonder();
    SysCtlDelay(50000);


    lcdkomut(0x02);
    SysCtlDelay(50000);


    lcdkomut(0x28);
    SysCtlDelay(50000);
    lcdkomut(0x0C);
    SysCtlDelay(50000);
    lcdkomut(0x06);
    SysCtlDelay(50000);


    LCDTemizle();
    SysCtlDelay(50000);
}

void LCDTemizle() {
    lcdkomut(0x01); //
    SysCtlDelay(2000);
}

void LCDgit(unsigned char row, unsigned char col) {
    unsigned char address;
    if (row == 0) {
        address = col;
    } else {
        address = 0x40 + col;
    }
    lcdkomut(0x80 | address);
}

void lcdyaz(const char *str) {
    while (*str) {
        lcdkarakteryaz(*str++);
    }
}


void lcdSayiyaz(int value) {
    char buffer[16];
    sprintf(buffer, "%d", value);
    lcdyaz(buffer); // Stringi LCD'ye yaz
}



