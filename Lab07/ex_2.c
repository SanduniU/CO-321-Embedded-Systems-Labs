#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define BLINK_DELAY_MS 500

void EEPROMwrite(unsigned int address, char data);
char EEPROMread(unsigned int address);
unsigned char usart_receive(void);
void usart_init(void);
void usart_send(unsigned char data);

int main(void)
{
    usart_init();
    unsigned int address = 10;
    unsigned char data = 'a';

    while(1){
        data = EEPROMread(address);
        usart_send(data);

        // Check for the end of data condition
        if (data == '\n') {
            break; // or any other appropriate termination condition
        }

        address++;
    }
}


void EEPROMwrite(unsigned int address, char data){
    while(EECR & (1<<EEPE));
    EEAR=address;
    EEDR = data;

    EECR|=(1<<EEMPE);

    EECR|=(1<<EEPE);
}

char EEPROMread(unsigned int address){
    while(EECR & (1<<EEPE));

    EEAR = address;

    EECR|=(1<<EERE);

    return EEDR;
}

void usart_init(void) {
  // Set baud rate
  // baud rate = 9600
  //UBR = (16*10^6 / (16 * 9600)) - 1=103.1667 = 0x0067
  
  UBRR0H = 0x00;
  UBRR0L = 0x67;

  // Enable receiver and transmitter
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);

  // Set frame format: 8data, 1stop bit
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

unsigned char usart_receive(void) {
  // Wait until a byte has been received
  while (!(UCSR0A & (1 << RXC0)));

  // Return received data
  return UDR0;
}

void usart_send(unsigned char data) {
  // Wait if a byte is being transmitted
  while (!(UCSR0A & (1 << UDRE0)));

  // Transmit data
  UDR0 = data;
}