#include <avr/io.h>
#include <util/delay.h>
#include<avr/interrupt.h>

#define BLINK_DELAY_MS 2000
int userInput =0;
int number=0;
int main (void){
    DDRD &=  ~(1<<2); // configure portD bit 2 and 3 for input 
    DDRD &= ~(1<<3);
    DDRB = 0xFF; /* configure pin PORTB for output*/
    
    //generate interrupt request at rising edge for int0 and int1
    EICRA |=(1<<ISC01);
    EICRA |=(1<<ISC00);
    EICRA |=(1<<ISC11);
    EICRA |=(1<<ISC10);
    //global interrupt enable
    sei();
    //external interrupt 1 and 0 enable
    EIMSK |=(1<<INT0);
    EIMSK |=(1<<INT1);

    while(1){        
    }

    return 0;
}

//interrupt service routing for int0
ISR (INT0_vect){
    userInput = (userInput<<1);
    if (userInput>=64){
        userInput&=~(1<<6);
    }
    PORTB =userInput;
    _delay_ms(BLINK_DELAY_MS);
}

//interrupt service routing for int1
ISR (INT1_vect){
    userInput = (userInput<<1);
    if (userInput>=64){
        userInput&=~(1<<6);
    }
    userInput|=(1<<0);
    PORTB =userInput;
    _delay_ms(BLINK_DELAY_MS);
}