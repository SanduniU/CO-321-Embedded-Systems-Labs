#include <avr/io.h>
#include <stdio.h>

void usart_init(void);
unsigned char usart_receive(void);
void usart_send(unsigned char data);

int main(void) {
  usart_init();
  while (1) {
    unsigned char data = usart_receive();
    usart_send(data);
  }
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

void usart_send(unsigned char data) {
  // Wait if a byte is being transmitted
  while (!(UCSR0A & (1 << UDRE0)));

  // Transmit data
  UDR0 = data;
}

unsigned char usart_receive(void) {
  // Wait until a byte has been received
  while (!(UCSR0A & (1 << RXC0)));

  // Return received data
  return UDR0;
}