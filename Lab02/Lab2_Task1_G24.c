#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 500
int main (void){
    DDRD &=  ~(1<<7); // configure portD bit 7 for input 
    DDRB = 0xFF ; /* configure pin PORTB for output*/

    // EICRA |=(1<<ISC01);
    // EICRA |=(1<<ISC00);
    // sei();

    int counter=0;
    while(1){
        if (PIND &(1<<7)){
            counter++;
            PORTB=counter;
            // PORTB|=(1<<2);
            _delay_ms(BLINK_DELAY_MS);
            // PORTB &=~(1<<2);
        }
        
    }
}