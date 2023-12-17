#include <avr/io.h>
#include <util/delay.h>

int main() {
    DDRD |= 1 << 6; // set PD6 as output (digital pin 6)

    // Fast PWM mode, non-inverting mode
    TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);

    uint8_t count = 1;
    OCR0A = 50;

    //N = 16*10^6/(256*7812.5) = 8 prescalar 0x02
    // N = 16*10^6/(256*976.56) = 64 prescalar 0x03
    // N = 16*10^6/(256*244.14) = 256 prescalar 0x04
    // N = 16*10^6/(256*61.04) = 1024 prescalar 0x05

    // Configure the timer with a prescaler of 64
    TCCR0B |= (1 << CS01);

    while (1) {
        _delay_ms(1000);

        // Toggle between prescaler values 8,64,256 and 1024
        switch(count){
            case(1):// 8 prescalar 010
                //7812.5Hz
                TCCR0B = (TCCR0B & ~((1 << CS02) | (1 << CS01) | (1 << CS00))) | (1 << CS01);
                break;
            case(2): //64 prescalar 011
                // 976.56Hz
                TCCR0B = (TCCR0B & ~((1 << CS02) | (1 << CS01) | (1 << CS00))) | (1 << CS01) |(1<<CS00);
                break;
            case(3): //256 prescalar 100
                // 244.14Hz
                TCCR0B = (TCCR0B & ~((1 << CS02) | (1 << CS01) | (1 << CS00))) | (1 << CS02);
                break;
            case(4): //1024 prescalar 101
                //61.04Hz
                TCCR0B = (TCCR0B & ~((1 << CS02) | (1 << CS01) | (1 << CS00))) | (1 << CS02) | (1 << CS00);
                break;
        }

        _delay_ms(1000);
        count++;

        if (count > 4) {
            count = 1;
        }
    }

    return 0;
}