#include <avr/io.h>
#include <stdio.h>

void usart_init(void);
unsigned char usart_receive(void);
void usart_send(unsigned char data);

int main() {
    // Initialize USART
    usart_init();

    while (1) {
        // Receive a sentence from the PC
        char sentence[100];
        int index = 0;
        char received_char;

        do {
            received_char = usart_receive();

            // Check for the end of the sentence (carriage return)
            if (received_char != '\n') {
                sentence[index++] = received_char;
            }
        } while (received_char != '\n');

        // Apply Caesar cipher with key 3
        for (int i = 0; i < index; i++) {
            if ((sentence[i] >= 'A' && sentence[i] <= 'Z') || (sentence[i] >= 'a' && sentence[i] <= 'z')) {
                sentence[i] = (sentence[i] + 3 - ((sentence[i] <= 'Z') ? 'A' : 'a')) % 26 + ((sentence[i] <= 'Z') ? 'A' : 'a');
            }
        }

        // Transmit the encrypted sentence back to the PC
        for (int i = 0; i < index; i++) {
            usart_send(sentence[i]);
        }

        // Transmit a newline character to separate sentences
        usart_send('\n');
    }

    return 0;
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