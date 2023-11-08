#include <avr/io.h>
#include <util/delay.h>
#include<avr/interrupt.h>

#define BLINK_DELAY_MS 500
int main (void){
    DDRD &=  ~(1<<2); // configure portD bit 7 for input 
    DDRB |= (1<<0)  ; /* configure pin PORTB for output*/

    EICRA |=(1<<ISC01);
    EICRA &= ~(1<<ISC00);
    sei();

    EIMSK |=(1<<INT0);

    while(1){
        
    }

    return 0;
}

ISR (INT0_vect){
    PORTB ^= (1<<0);
    _delay_ms(BLINK_DELAY_MS);
    // printf("%d", PORTB);
}
