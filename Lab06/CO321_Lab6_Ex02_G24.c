// Program the AVR in C to transmit the names and E-numbers of your group mates.

#include <avr/io.h>
#include <stdio.h>

void usart_init(void);
char usart_receive(void);
void usart_send(char data);

int main(void) {
  usart_init();
  char* group_mates = "E/19/375: Dasun, E/19/405: Rajitha, E/19/408: Sanduni";
  char data;
  int i = 0;

  while(1) {
    data = group_mates[i++];
    if(data=='\0') break;
    usart_send(data);
  }
}


void usart_init(void) {
  // Set baud rate
  // baud rate = 9600
  //UBR = (16*10^6 / (16 * 9600)) - 1=103.1667 = 0x67

  UBRR0H = 0x00;
  UBRR0L = 0x67;

  // Enable receiver and transmitter
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);

  // Set frame format: 8data, 1stop bit
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void usart_send(char data) {
  // Wait if a byte is being transmitted
  while (!(UCSR0A & (1 << UDRE0)));

  // Transmit data
  UDR0 = data;
}

char usart_receive(void) {
  // Wait until a byte has been received
  while (!(UCSR0A & (1 << RXC0)));

  // Return received data
  return UDR0;
}