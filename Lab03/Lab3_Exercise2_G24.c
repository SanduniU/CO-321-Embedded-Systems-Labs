#include <avr/io.h>
#include <util/delay.h>

void delay_timer0(){
    TCNT0 = 100; // Set the delay to 10ms

    TCCR0A = 0x00;
    TCCR0B = 0x05;

    while((TIFR0 & 0x01)==0);
    TCCR0A = 0x00;
    TCCR0B = 0x00;

    TIFR0 = 0x01;

}

// the maximum possible delay is 16.384ms with 1024 prescalar and 0 as initial counter
// we can get larger delays by looping for required amount of time

int main(void){
    DDRB|=(1<<5);
    while(1){
        PORTB|=(1<<5);
        // _delay_ms(16.384);
        for(int i=0; i<50;i++){

        delay_timer0();
        }

        PORTB &= ~(1<<5);
        // _delay_ms(16.384);

        //count for 50 times  to get 500ms 
        for(int i=0; i<50;i++){

        delay_timer0();
        }
    }
}