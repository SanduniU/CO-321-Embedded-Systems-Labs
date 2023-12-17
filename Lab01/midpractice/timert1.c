#include <avr/io.h>
#include <avr/interrupt.h>


int main(){
    DDRB = 0xFF;
    TCNT1 = 34286;
    TCCR1A = 0x00;
    TCCR1B = 0x03;
    
    TIMSK1|=(1<<TOIE1);

    sei();

    while(1);
}

ISR(TIMER1_OVF_vect){
    PORTB = ~PORTB;
    TCNT1 = 34286;
}