#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "lcd.h"
#include <string.h>
#include <stdio.h>

void updateLCD();
void endADCandTime();
void initSystem();
void UART0Handler();

char saatBilgisi[9] = "00:00:00";
uint32_t adcValue = 0;
bool yeniSaatGeldi = false;
bool saatGuncelle = true;

// UART Kesme Servis Rutini
void UART0Handler(void) {
    UARTIntClear(UART0_BASE, UART_INT_RX | UART_INT_RT);

    static char buffer[16];
    static int index = 0;

    while (UARTCharsAvail(UART0_BASE)) {
        char receivedChar = UARTCharGet(UART0_BASE);

        // Gelen veriyi buffer'a yaz
        if (receivedChar == '\n') {
            buffer[index] = '\0';
            index = 0;

            // "RESET_SYSTEM" Komutu
            if (strcmp(buffer, "RESET_SYSTEM") == 0) {
                LCDTemizle();                    // LCD temizle
                strcpy(saatBilgisi, "00:00:00");
                yeniSaatGeldi = false;
                saatGuncelle = true;
                adcValue = 0;
            }

            else if (strlen(buffer) == 8 && buffer[2] == ':' && buffer[5] == ':') {
                strcpy(saatBilgisi, buffer);    // Gelen saat bilgisini kaydet
                yeniSaatGeldi = true;
                saatGuncelle = true;
            }
        } else {
            if (index < sizeof(buffer) - 1) {
                buffer[index++] = receivedChar;
            }
        }
    }
}


void sendADCandTime() {
    char buffer[32];
    int i;


    ADCProcessorTrigger(ADC0_BASE, 3);
    while (!ADCIntStatus(ADC0_BASE, 3, false)) {}
    ADCIntClear(ADC0_BASE, 3);
    ADCSequenceDataGet(ADC0_BASE, 3, &adcValue);


    snprintf(buffer, sizeof(buffer), "Time:%s ADC:%03d\n", saatBilgisi, adcValue);
    for (i = 0; buffer[i] != '\0'; i++) {
        UARTCharPut(UART0_BASE, buffer[i]);
    }


    if (yeniSaatGeldi) {
        yeniSaatGeldi = false;
    } else if (saatGuncelle) {

        int hour, min, sec;
        sscanf(saatBilgisi, "%2d:%2d:%2d", &hour, &min, &sec);
        sec++;
        if (sec == 60) { sec = 0; min++; }
        if (min == 60) { min = 0; hour++; }
        if (hour == 24) { hour = 0; }
        snprintf(saatBilgisi, sizeof(saatBilgisi), "%02d:%02d:%02d", hour, min, sec);
    }
}


void updateLCD() {
    char buffer[16];


    LCDgit(0, 0);
    lcdyaz("Veriler:");


    LCDgit(1, 0);
    snprintf(buffer, sizeof(buffer), "P:%04d", adcValue);
    lcdyaz(buffer);


    LCDgit(1, 8);
    lcdyaz(saatBilgisi);
}


void initSystem() {

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);


    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX); //
    GPIOPinConfigure(GPIO_PA1_U0TX); //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    UARTIntRegister(UART0_BASE, UART0Handler);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    IntMasterEnable();


    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);


    LCDilkayarlar();
    LCDTemizle();
}

int main(void) {

    initSystem();

    while (1) {
        sendADCandTime();
        updateLCD();
        SysCtlDelay(SysCtlClockGet() / 3);
    }
}
