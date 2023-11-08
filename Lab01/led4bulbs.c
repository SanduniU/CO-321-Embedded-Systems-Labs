#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 1000
int main (void){
DDRB = DDRB|(1<<5)|(1<<4)|(1<<3)|(1<<2); /* configure pin 5,4,3,2 of PORTB for output*/
while(1){
PORTB = PORTB | (1<<5)|(1<<4)|(1<<3)|(1<<2); /* set pin 5,4,3,2 high to turn led on */
_delay_ms(BLINK_DELAY_MS);
PORTB = PORTB &~(1<<5) &~(1<<4)&~(1<<3)&~(1<<2); /* set pin 5,4,3,2 low to turn led off */
_delay_ms(BLINK_DELAY_MS);

}
}