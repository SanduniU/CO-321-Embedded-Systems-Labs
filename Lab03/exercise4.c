#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BLINK_DELAY_MS 100

int main(void){
    DDRB = 0xFF;
    TCNT1 = 3036; //for 1000ms using 256 prescalar
    TCCR1A = 0x00;
    TCCR1B = 0x04;// 256 pre scalar
    TIMSK1 = 1 << TOIE1; 
    // TIFR1  = 1<<TOV1;

    sei();

    while(1){
    //led5-> led4 -> led 3 -> led 2 ->led 3 -> led 4 -> led 5...
    for (int i=3;i>0;i--){ 
        PORTB = PORTB | (1<<i); /* set i th pin high to turn led on */
        _delay_ms(BLINK_DELAY_MS);
        PORTB = PORTB &~(1<<i); /* set i th pin low to turn led off */
        // delay_timer0();
        _delay_ms(BLINK_DELAY_MS);
    }
    
    for (int i=2;i<5;i++){
        PORTB = PORTB | (1<<i); /* set i th pin high to turn led on */
        // delay_timer0();
        _delay_ms(BLINK_DELAY_MS);
        PORTB = PORTB &~(1<<i); /* set i th pin low to turn led off */
        // delay_timer0();
        _delay_ms(BLINK_DELAY_MS);

    }

}

}

ISR(TIMER1_OVF_vect){
    PORTB ^=(1<<5);
    TCNT1 = 3036;
    // TCCR1A = 0x00;
    // TCCR1B = 0x04;// 256 pre scalar

}