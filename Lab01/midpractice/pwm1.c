#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(){
    DDRD |=(1<<6);
    TCNT0=0;
    TCCR0A |=(1<<COM0A1)|(1<<WGM00)|(1<<WGM01);
    TCCR0B |= 0x03;
    
    // TIMSK0|=(1<<TOIE1);

    // sei();
    OCR0A=  255;
    while(1){
        while(OCR0A>0){
            OCR0A-=10;
            _delay_ms(100);
        }
        while(OCR0A<256){
            OCR0A+=10;
            _delay_ms(100);
        }
    }
    return 0;
}

