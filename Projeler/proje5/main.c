#include <stdint.h>
#include "inc/tm4c123gh6pm.h"

void PortF_Init(void);
void Hibernation_Init(void);
void DelayMs(uint32_t delay);

int main(void) {
    PortF_Init();         // PortF initialization
    Hibernation_Init();   // Hibernation initialization

    // LED'i 5 saniye boyunca yak
    GPIO_PORTF_DATA_R |= 0x02; // PF1 LED ON
    DelayMs(5000);             // 5 sn delay

    GPIO_PORTF_DATA_R &= ~0x02; // PF1 LED OFF

    // Hibernation moduna gir ve 10 saniye sonra uyan
    HIB_RTCLD_R = 10;            // 10 saniye sonra uyanma zamanı ayarla
    HIB_CTL_R |= HIB_CTL_RTCEN | HIB_CTL_CLK32EN | HIB_CTL_HIBREQ;
    __asm(" WFI");             // Hibernation moduna geç (kesme bekle)

    // Uyanınca LED tekrar yanar
    GPIO_PORTF_DATA_R |= 0x02;   // PF1 LED ON

    while (1) {
        // İşlem tamamlandıktan sonra LED yanık kalır
    }
}

void PortF_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;         // PortF saati etkinleştirildi
    while ((SYSCTL_PRGPIO_R & 0x20) == 0);

    GPIO_PORTF_LOCK_R = 0x4C4F434B;    // PortF kilidini aç
    GPIO_PORTF_CR_R |= 0x02;           // PF1 için değişikliğe izin verildi
    GPIO_PORTF_DIR_R |= 0x02;          // PF1 çıkış
    GPIO_PORTF_DEN_R |= 0x02;          // PF1 dijital etkin
}

void Hibernation_Init(void) {
    SYSCTL_RCGCHIB_R |= 0x01;          // Hibernation saati etkinleştirildi
    while ((SYSCTL_PRHIB_R & 0x01) == 0);

    HIB_CTL_R |= HIB_CTL_CLK32EN;      // 32.768 kHz osilatör etkinleştirildi
    while ((HIB_CTL_R & HIB_CTL_WRC) == 0);

    HIB_IM_R |= HIB_IM_RTCALT0;        // RTC Alarm kesmesi etkinleştirildi
    NVIC_EN1_R |= 1 << (INT_HIBERNATE - 16 - 32); // Hibernation kesmesi etkinleştirildi
}

void DelayMs(uint32_t delay) {
    volatile uint32_t i;
    for (i = 0; i < delay * 16000; i++); // 16 MHz saat hızı için basit bir gecikme
}

void Hibernation_Handler(void) {
    HIB_IC_R |= HIB_IC_RTCALT0;        // RTC Alarm kesmesini temizlenir
}
