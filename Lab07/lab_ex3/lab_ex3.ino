#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LiquidCrystal.h"


#define MENU_DELAY 2000
#define EEPROM_SIZE 20
#define KEY_ADDRESS 0xAA
#define ADDRESS 0x00
#define BUFFER_SIZE 15

const char alphabet[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', '*', '#'};

const char keyPad[4][4] =  {{'1','2','3','A'},
                        {'4','5','6','B'},
                        {'7','8','9','C'},
                        {'*','0','#','D'}};

LiquidCrystal lcd(8, 9, 13, 12, 11, 10);

unsigned char colloc, rowloc;

void EEPROMwrite(unsigned int address, char data);
char EEPROMread(unsigned int address);
unsigned char usart_receive(void);
void usart_init(void);
void usart_send(unsigned char data);
char pressedKey();
char encryptChar(char c);
void showSecretKey();
void changeSecretKey();
char charToInt(char c);
void getInput();
void printEncrypted();

int main()
{
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("START");
    _delay_ms(2000);
    char c;

    while (1)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("1. Encrypt");
        lcd.setCursor(0, 1);
        lcd.print("2: Secret key");

        while ((c = pressedKey()) != '1' && c != '2')
            ;

        if (c == '1')
        {
            getInput();
            printEncrypted();
        }

        else
        {
            lcd.clear();
            lcd.print("1. Show key");
            lcd.setCursor(0, 1);
            lcd.print("2. Change key");

            while ((c = pressedKey()) != '1' && c != '2')
                ;

            if (c == '1')
            {
                showSecretKey();
            }

            else
            {
                changeSecretKey();
            }
        }
        _delay_ms(1000);
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

char pressedKey() {

    while (1)
    {
        DDRD = 0xF0; /* set port direction as input-output */
        PORTD = 0xFF;

        // solely for the debouncing effect - repeatedly checking the keypad's column status until it stabilizes in an unpressed state.
        // It helps to prevent false readings and ensure that subsequent key press detection is accurate.
        do
        {
            PORTD &= 0x0F; /* mask PORT for column read only */
            asm("NOP");
            colloc = (PIND & 0x0F); /* read status of column */
        } while (colloc != 0x0F);
        // for debouncing as well
        do
        {
            do
            {
                _delay_ms(20);             /* 20ms key debounce time */
                colloc = (PIND & 0x0F); /* read status of column */
            } while (colloc == 0x0F);      /* check for any key press */

            _delay_ms(40); /* 20 ms key debounce time */
            colloc = (PIND & 0x0F);
        } while (colloc == 0x0F);

        /* now check for rows */
        PORTD = 0xEF; /* check for pressed key in 1st row */
        asm("NOP");
        colloc = (PIND & 0x0F);
        if (colloc != 0x0F)
        {
            rowloc = 0;
            break;
        }

        PORTD = 0xDF; /* check for pressed key in 2nd row */
        asm("NOP");
        colloc = (PIND & 0x0F);
        if (colloc != 0x0F)
        {
            rowloc = 1;
            break;
        }

        PORTD = 0xBF; /* check for pressed key in 3rd row */
        asm("NOP");
        colloc = (PIND & 0x0F);
        if (colloc != 0x0F)
        {
            rowloc = 2;
            break;
        }

        PORTD = 0x7F; /* check for pressed key in 4th row */
        asm("NOP");
        colloc = (PIND & 0x0F);
        if (colloc != 0x0F)
        {
            rowloc = 3;
            break;
        }
    }

    if (colloc == 0x0E)
        return (keyPad[rowloc][0]);
    else if (colloc == 0x0D)
        return (keyPad[rowloc][1]);
    else if (colloc == 0x0B)
        return (keyPad[rowloc][2]);
    else
        return (keyPad[rowloc][3]);
}

void getInput(){
    char c;
    char buffer[BUFFER_SIZE];
    int bufferIndex = 0;

    lcd.clear();
    lcd.print("Enter Message: ");
    lcd.setCursor(0,1);

    while(c != '#' && bufferIndex < 10){
        c = pressedKey();
        lcd.print(c);
        buffer[bufferIndex] = c;
        bufferIndex++;
    }

    buffer[bufferIndex] = '\n';
    bufferIndex++;
    buffer[bufferIndex] = '\0';

    for(int i = 0; i < bufferIndex; i++){
        EEPROMwrite(ADDRESS+i, buffer[i]);
    }

    lcd.clear();
    lcd.print("Message Saved");
    _delay_ms(MENU_DELAY);
}

void printEncrypted(){
    char c;
    char buffer[BUFFER_SIZE];
    int i = 0;

    lcd.clear();
    lcd.print("Encrypted Message: ");
    lcd.setCursor(0,1);

    while(c != '#' && i < 10){
        c =EEPROMread(ADDRESS+i);
        lcd.print(encryptChar(c));
        buffer[i] = encryptChar(c);
        i++;
    }
    _delay_ms(MENU_DELAY);
    lcd.setCursor(0,0);
    lcd.print("Press * to exit:");
    while(pressedKey() != '*');

    // buffer[i] = '\n';
    // i++;
    // buffer[i] = '\0';

    // for(int j = 0; j <i ; i++){
    //     EEPROMwrite(ADDRESS+i, buffer[j]);
    // }

    // lcd.clear();
    // lcd.print("Encrypted Message Saved");
    // _delay_ms(500);
    // lcd.print("Press * to exit");

}


char encryptChar(char c){
    char encryptedChar = c;
    char key = EEPROMread(KEY_ADDRESS);
    int num = charToInt(c);

    return alphabet[(num+key)%16];

}

void showSecretKey(){
    char key = EEPROMread(KEY_ADDRESS);

    // show secret key in lcd display
    lcd.clear();
    lcd.print("Secret Key: ");
    lcd.setCursor(0,1);
    lcd.print(key);

    _delay_ms(MENU_DELAY);
    lcd.clear();
    lcd.print("Press * to exit");
    while(pressedKey() != '*');
}

void changeSecretKey(){
    // get secret key as input
    lcd.clear();
    lcd.print("Enter New Secret Key");
    lcd.setCursor(0,1);
    char c;
    while((c=pressedKey()) < '0' && c > '9'){

    }

    lcd.print(c);
    char key = c;
    EEPROMwrite(KEY_ADDRESS, key);
    lcd.clear();
    lcd.print("Secret Key Changed");

    _delay_ms(MENU_DELAY);
}

char charToInt(char c){
    char num = 0;
    if (c >= '0' && c <= '9'){
        num = c - '0';
    }
    else if (c >= 'A' && c <= 'D'){
        num = c - 'A' + 10;
    }

    else if (c == '*'){
        num = 14;
    }

    else if (c == '#'){
        num = 15;
    }
    return num;
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