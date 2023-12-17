#include <avr/io.h>
#include <util/delay.h>

int main(){
    DDRB|=(1<<5);

    while (1)
    {
        PORTB|=(1<<5);
        _delay_ms(500);

        PORTB&=~(1<<5);
        _delay_ms(500);
    }
    return 0;
}