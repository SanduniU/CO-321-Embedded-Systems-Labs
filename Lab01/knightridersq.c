#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 50
int main (void){
DDRB = 0xFF; /* configure pin 5,4,3,2 of PORTB for output*/
DDRD = 0xFF;
while(1){
    //led5-> led4 -> led 3 -> led 2 ->led 3 -> led 4 -> led 5...
    for (int i=5;i>=0;i--){ 
        PORTB = PORTB | (1<<i); /* set i th pin high to turn led on */
        _delay_ms(BLINK_DELAY_MS);
        PORTB = PORTB &~(1<<i); /* set i th pin low to turn led off */
        _delay_ms(BLINK_DELAY_MS);
    }

    for (int i=7;i>=2;i--){ 
        PORTD = PORTD | (1<<i); /* set i th pin high to turn led on */
        _delay_ms(BLINK_DELAY_MS);
        PORTD = PORTD &~(1<<i); /* set i th pin low to turn led off */
        _delay_ms(BLINK_DELAY_MS);
    }

    
    // for (int i=1;i<5;i++){
    //     PORTB = PORTB | (1<<i); /* set i th pin high to turn led on */
    //     _delay_ms(BLINK_DELAY_MS);
    //     PORTB = PORTB &~(1<<i); /* set i th pin low to turn led off */
    //     _delay_ms(BLINK_DELAY_MS);
    // }
}
}