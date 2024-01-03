// -------------------------------------------------------------
// University of Peradeniya - Faculty of Engineering
// Mid-Semester Examination - December 2023
// CO321 Embedded Systems
// -------------------------------------------------------------
// Registration Number: E/19/408
// -------------------------------------------------------------

// Include library files here
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Define your global variables here
int counter = 0;
int i=1;
int increment = 1;
int pause = 0;

// Main Function
int main (void){
    //timer 0 configuration for 2ms
    //counter increments = 2ms/0.016ms = 125
    //intial counter value = 256-125
    TCNT0 = 131; //Initial Value
    
    //prescalar =64
    TCCR0A = 0x00; //Normal Mode
    TCCR0B = 0x04; 
    
    DDRB = 0xFF; //Set Port B as output
    
    //set PD2 as input
    DDRD &= ~(1<<2);
    
    //button releasing ->falling edge(1 0)
    EICRA |= (1 << ISC01);
    EIMSK |= (1 << INT0); // Enable INT0

    
    //non-inverting fast PWM mode
    TCCR2A |= (1 << COM2A1)|(1 << WGM21) | (1 << WGM20);
    //64 prescaler to generate 976.56Hz PWM waveform
    TCCR2B = 0x03; 
    
    OCR2A = 255; //Set initial duty cycle to 100%
    
    sei(); //Set enable global interrupts
    TIMSK0 |= (1 << TOIE0); //Enable timer0 Interrupt;
    //flag for fading
    int fade = 1;
    
    
    while(1){
        if(pause && PORTB&(1<<3)){
            //enable PWM mode in timer 2
            TCCR2A |= (1 << COM2A1)|(1 << WGM21) | (1 << WGM20); //Fast PWM
            TCCR2B = 0x03;
            //fade in 
            if(fade){
                OCR2A--;
                if(OCR2A==0){
                    fade = 0;
                }
            }
            //fade out
            else{
                OCR2A++;
                if(OCR2A==255){
                    fade = 1;
                }
            }

            //(1000ms to fade in and fade out)
            // 1000ms /255 = 4ms
            _delay_ms(4);
        }
        else{
            TCCR2A = 0x00;
            TCCR2B = 0x00;
            OCR2A = 0;
        }
    }
    return 0;
}

// External Intererupt 0 Service Routine
ISR (INT0_vect){
    if (pause){
        pause = 0;
        TIMSK0 |= (1 << TOIE0);
    }
    else{
        pause = 1;
        TIMSK0 &= ~(1 << TOIE0);
    }
  
}

// Timer0 Overflow Interrupt Service Routine
ISR(TIMER0_OVF_vect){
    counter++;
    if(counter==100){
        // PORTB = (1<<4);
        PORTB = (1<<i);
        //A -> B -> C -> D -> E -> D -> C -> B -> A
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
    TCNT0 = 131;
}
