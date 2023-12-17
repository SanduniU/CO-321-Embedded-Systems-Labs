#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//to count the timer overflows
int counter = 0;
int i = 1;
int increment = 1;
int pause=0;
int main(){

    //for 2ms
    //prescalar = 256
    //counter increments = 2000/16 = 125
    //initial counter value = 256-125
    TCNT0 = 131;
    DDRB = 0xFF;
    DDRD &=~(1<<3);

    TCCR0A = 0x00;
    TCCR0B = 0x04;

    TIMSK0 |= (1<<TOIE0);

    
    EICRA = EICRA|(1<<ISC11);
    EIMSK = EIMSK|(1<<INT1);

    sei();
    // PORTB=(1<<5);

    while(1);
    return 0;
}

ISR(TIMER0_OVF_vect){
    counter++;
    if(counter==100){
        // PORTB = (1<<4);
        PORTB = (1<<i);
        if(increment){
            i++;
            if(i==5){
                increment = 0;
            }
        }
        else{
            i--;
            if(i==1){
                increment=1;
            }
        }
        counter=0;
    }
}

ISR(INT1_vect){
    if(!pause){
        TIMSK0 &= ~(1 << TOIE0);
        pause=1;
    }

    else{
        // TCCR0B = 0x04;
        TIMSK0 |= (1 << TOIE0);
        pause= 0;
    }
}