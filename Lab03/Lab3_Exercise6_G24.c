#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BLINK_DELAY_MS 100

int counter50 = 0;
int  counter500 = 0;

int main(void){
    DDRB = 0xFF;
    TCNT0 = 131; //for 2ms using 256 prescalar using timer 0
    TCNT1 = 59286; //for 100ms using 256 prescalar using timer1
    TIMSK0 |= (1 << TOIE0); 
    TIMSK1 |= (1 << TOIE1); 
    TCCR0A = 0x00;
    TCCR0B = 0x04;// 256 pre scalar
    TCCR1A = 0x00;
    TCCR1B = 0x04;// 256 pre scalar

    sei();
    PORTB|=(1<<5);
    PORTB|=(1<<4);

    while(1);

}

ISR(TIMER0_OVF_vect){
    counter50++;
    //2ms *25 = 50ms
    if(counter50 ==25){
        PORTB ^= (1<<5);
        counter50 = 0;
    }
    TCNT0=131;
}

ISR(TIMER1_OVF_vect){
    counter500++;
    //100ms *25 = 500ms
    if(counter500 ==5){
        PORTB ^= (1<<4);
        counter500 = 0;
    }
    TCNT1=59286;
}