#include <avr/io.h>
#include <util/delay.h>

void delay_timer0(){
    // prescalar = 256
    // counter increments = 256-2*1000/16;
    TCNT0 = 131; 

    TCCR0A = 0x00;
    TCCR0B = 0x04;

    while((TIFR0 & 0x01)==0);
    TCCR0A = 0x00;
    TCCR0B = 0x00;

    TIFR0 = 0x01;

}

int main(void){
    DDRB|=(1<<5);
    while(1){
        PORTB|=(1<<5);

        delay_timer0();

        PORTB &= ~(1<<5);
        delay_timer0();
    }
}