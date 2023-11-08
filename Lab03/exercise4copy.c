#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void delay_timer0(){
    TCNT1 = 55120; //for 166.67ms using 256 prescalar

    TCCR1A = 0x00;
    TCCR1B = 0x04;// 256 pre scalar

    sei();
    while(1);  

}

int main(void){
    DDRB = 0xFF;

    while(1){
    //led5-> led4 -> led 3 -> led 2 ->led 3 -> led 4 -> led 5...
    for (int i=3;i>0;i--){ 
        PORTB = PORTB | (1<<i); /* set i th pin high to turn led on */
        delay_timer0();
        PORTB = PORTB &~(1<<i); /* set i th pin low to turn led off */
        delay_timer0();
        // _delay_ms(BLINK_DELAY_MS);
    }
    //toggle 5th  pin of port b after 1s
    //knight rider loop executes 6 times, 166.67*6=1000
    PORTB ^=(1<<5);

    for (int i=2;i<5;i++){
        PORTB = PORTB | (1<<i); /* set i th pin high to turn led on */
        delay_timer0();
        // _delay_ms(BLINK_DELAY_MS);
        PORTB = PORTB &~(1<<i); /* set i th pin low to turn led off */
        delay_timer0();
        // _delay_ms(BLINK_DELAY_MS);

    }
    //toggle 5th  pin of port b after 1s
    //knight rider loop executes 6 times, 166.67*6=1000
    PORTB ^=(1<<5);

}

}

ISR(TIMER0_OVF_vect){
    TCCR1A = 0x00;
    TCCR1B = 0x00;
}