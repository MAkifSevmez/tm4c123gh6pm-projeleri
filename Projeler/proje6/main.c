#include <stdint.h>
#define SYSCTL_RCGCGPIO_R (*((volatile uint32_t *)0x400FE608))
#define GPIO_PORTF_DIR_R  (*((volatile uint32_t *)0x40025400))
#define GPIO_PORTF_DEN_R  (*((volatile uint32_t *)0x4002551C))
#define GPIO_PORTF_DATA_R (*((volatile uint32_t *)0x400253FC))

int main(void) {

    volatile unsigned int delay;


    SYSCTL_RCGCGPIO_R |= 0x20;
    for (delay = 0; delay < 200; delay++);


    GPIO_PORTF_DIR_R |= 0x0E;
    GPIO_PORTF_DEN_R |= 0x0E;


    float value = 0.0f;
    float increment = 0.1f;

    while (1) {

        value += increment;
        if (value > 1.0f || value < 0.0f) {
            increment = -increment;
        }


        if (value > 0.5f) {
            GPIO_PORTF_DATA_R |= 0x02;
        } else {
            GPIO_PORTF_DATA_R &= ~0x02;
        }


        for (delay = 0; delay < 200000; delay++);
    }
}
