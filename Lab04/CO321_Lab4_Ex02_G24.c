#include <avr/io.h>
#include <util/delay.h>
// #include <avr/interrupt.h>

int main(){
 DDRD |= 1<<6; // set PD6 as output(digital pin 11)
    TCNT0 = 0; 
    //N = 16*10^6/(256*976.56) = 64 prescalar

    //COM0A1 - non inverting
    // WGM01, WGM02 - Fast PWM mode
    // CS00, CS01 -64 prescalar
    TCCR0A |= (1<<COM0A1) | (1<< WGM01) | (1<< WGM00);
    TCCR0B |=  (1<<CS00)|(1<<CS01);

    //OCR0= 256*50/100-1 = 127
    OCR0A = 255;

    while(1){
        while(OCR0A>0){
        OCR0A = OCR0A-1;
        _delay_ms(10);
        }
        while(OCR0A<255){
        OCR0A = OCR0A+1;
        _delay_ms(10);
        }
    }
    return 0;
}