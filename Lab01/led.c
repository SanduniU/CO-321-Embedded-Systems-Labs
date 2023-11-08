#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 1000
int main (void){
DDRB = DDRB|(1<<5)|(1<<4)|(1<<3)|(1<<2); /* configure pin 5 of PORTB for output*/
while(1){
    for (int i=5;i>1;i--){
        PORTB = PORTB | (1<<i); /* set pin 5 high to turn led on */
        _delay_ms(BLINK_DELAY_MS);
        PORTB = PORTB &~(1<<i); /* set pin 5 low to turn led off */
        _delay_ms(BLINK_DELAY_MS);
    }

    for (int i=2;i<6;i++){
        PORTB = PORTB | (1<<i); /* set pin 5 high to turn led on */
        _delay_ms(BLINK_DELAY_MS);
        PORTB = PORTB &~(1<<i); /* set pin 5 low to turn led off */
        _delay_ms(BLINK_DELAY_MS);
    }
// PORTB = PORTB | (1<<5); /* set pin 5 high to turn led on */
// _delay_ms(BLINK_DELAY_MS);
// PORTB = PORTB &~(1<<5); /* set pin 5 low to turn led off */
// _delay_ms(BLINK_DELAY_MS);
// PORTB = PORTB | (1<<4); /* set pin 5 high to turn led on */
// _delay_ms(BLINK_DELAY_MS);
// PORTB = PORTB &~(1<<4); /* set pin 5 low to turn led off */
// _delay_ms(BLINK_DELAY_MS);
// PORTB = PORTB | (1<<3); /* set pin 5 high to turn led on */
// _delay_ms(BLINK_DELAY_MS);
// PORTB = PORTB &~(1<<3); /* set pin 5 low to turn led off */
// _delay_ms(BLINK_DELAY_MS);
// PORTB = PORTB | (1<<2); /* set pin 5 high to turn led on */
// _delay_ms(BLINK_DELAY_MS);
// PORTB = PORTB &~(1<<2); /* set pin 5 low to turn led off */
// _delay_ms(BLINK_DELAY_MS);
}
}