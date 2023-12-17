#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int counter=0;
int main(){
    DDRD&=~(1<<2);
    DDRB=0xFF;

    EICRA = EICRA|(1<<ISC00)|(1<<ISC01);
    sei();

    EIMSK = EIMSK|(1<<INT0);
    while(1);
    return 0;
}

ISR(INT0_vect){
    counter++;
    PORTB=counter;
    // PORTB=~PORTB;
}

// #include <avr/io.h>
// #include <util/delay.h>
// #include<avr/interrupt.h>
// #include<stdio.h>

// #define BLINK_DELAY_MS 1000

// int main(){
//     DDRD &=  ~(1<<2);
//     DDRB |=(1<<5);

//     EICRA |=(1<<ISC01);
//     EICRA |=(1<<ISC00);
//     sei();

//     EIMSK |=(1<<INT0);
//     while(1){
//     }

//     return 0;
// }

// ISR (INT0_vect){
//     PORTB |= (1<<5);
//     _delay_ms(BLINK_DELAY_MS);
//     // printf("%d", PORTB);
//     PORTB &=~(1<<5); 
// }