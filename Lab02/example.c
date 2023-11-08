#include <avr/io.h>
#include <util/delay.h>
#include<avr/interrupt.h>
#include<stdio.h>

#define BLINK_DELAY_MS 1000

int main(){
    DDRD &=  ~(1<<7);
    DDRB |=(1<<0);

    EICRA |=(1<<ISC01);
    EICRA |=(1<<ISC00);
    sei();

    EIMSK |=(1<<INT0);
    while(1){
    }

    return 0;
}

ISR (INT0_vect){
    PORTB |= (1<<0);
    _delay_ms(BLINK_DELAY_MS);
    // printf("%d", PORTB);
    PORTB &=~(1<<0); 
}