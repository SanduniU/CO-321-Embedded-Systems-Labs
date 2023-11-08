#include <avr/io.h>
#include <util/delay.h>

void delay_timer0(){
    TCNT1 = 3036;

    TCCR1A = 0x00;
    TCCR1B = 0x04;

    while((TIFR1 & 0x01)==0);
    TCCR1A = 0x00;
    TCCR1B = 0x00;

    TIFR1 = 0x01;

}

int main(void){
    DDRB = 0xFF;
    // while(1){
    //     PORTB|=(1<<5);
    //     // _delay_ms(16.384);
    //     delay_timer0();

    //     PORTB &= ~(1<<5);
    //     // _delay_ms(16.384);
    //     delay_timer0();
    // }

    while(1){
    //led5-> led4 -> led 3 -> led 2 ->led 3 -> led 4 -> led 5...
    for (int i=5;i>=0;i--){ 
        PORTB = PORTB | (1<<i); /* set i th pin high to turn led on */
        PORTB ^=(1<<5);
        delay_timer0();
        PORTB = PORTB &~(1<<i); /* set i th pin low to turn led off */
        PORTB ^=(1<<5);
        delay_timer0();
        // _delay_ms(BLINK_DELAY_MS);
    }

    for (int i=1;i<5;i++){
        PORTB = PORTB | (1<<i); /* set i th pin high to turn led on */
        PORTB ^=(1<<5);
        delay_timer0();
        // _delay_ms(BLINK_DELAY_MS);
        PORTB = PORTB &~(1<<i); /* set i th pin low to turn led off */
        PORTB ^=(1<<5);
        delay_timer0();
        // _delay_ms(BLINK_DELAY_MS);
    }
}

}