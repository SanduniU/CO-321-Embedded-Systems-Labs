#include <avr/io.h>
// #include <util/delay.h>
// #include <avr/interrupt.h>

int main(){
 DDRD |= 1<<6; // set PB3 as output(digital pin 11) 
    //N = 16*10^6/(256*976.56) = 64 prescalar

    //COM0A1 - non inverting
    // WGM01, WGM02 - Fast PWM mode
    // CS00, CS01 -64 prescalar
    TCCR0A |= (1<<COM0A1) | (1<< WGM01) | (1<< WGM00);
    TCCR0B |= (0x03);

    //OCR0= 256*50/100-1 = 127
    OCR0A = 127; // 50% duty cycle
    // TIMSK0 |= (1<<OCIE0A);

    while(1);
    return 0;
}
