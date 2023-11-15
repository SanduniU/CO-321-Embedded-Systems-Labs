#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BLINK_DELAY_MS 100

int counter = 0;
int main(void){
    DDRB = 0xFF;
    TCNT0 = 131; //for 2ms using 256 prescalar using timer 0
    
    TIMSK0 |= (1 << TOIE0); 
    TCCR0A = 0x00;
    TCCR0B = 0x04;// 256 pre scalar

    sei();
    PORTB|=(1<<5);

    while(1);

}

ISR(TIMER0_OVF_vect){
    counter++;
    if(counter >=50){
        PORTB ^= (1<<5);
        counter = 0;
    }
    TCNT0=131;
}