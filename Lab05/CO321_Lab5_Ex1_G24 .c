#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define XTAL 16000000UL

void adc_init(void);
void adc_start(void);

int main(void)
{
    adc_init();

    while(1)
    {
        adc_start();
        _delay_ms(1000);
    }

}

void adc_init()
{
    // make ADC1 as input
    DDRC &= ~(1 << PC1);
    // make port D as output
    DDRD = 0xFF;

    // Turn  on ADC module and set prescaler to 128
    // 16000000/128 = 125000Hz<200000Hz
    ADCSRA = 0x00;
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  

    // select voltage reference to AVCC and set ADC1 as input channel, left adjusted
    ADMUX = 0x00;
    ADMUX |= (1 << REFS0) | (1 << MUX0) | (1 << ADLAR);
}

void adc_start()
{
    // start conversion
    ADCSRA |= (1 << ADSC);
    while(!(ADCSRA & (1 << ADIF))){
        // wait for conversion to complete
    };
    
    // read the result from ADCH and display it on PORTD
    PORTD = ADCH ;

}