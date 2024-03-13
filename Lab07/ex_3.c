#include <avr/io.h>
#include <util/delay.h>
#include "LiquidCrystal.h"
#include <stdio.h>
/*
Configuration -
LCD -
VSS - Ground
VDD - 5V
V0 - Ground  (or Potentiometer)
RS - 8
RW - Ground
E - 9
D4 - 13
D5 - 12
D6 - 11
D7 - 10


KEYPAD -
ROW1 - PD4
ROW2 - PD5
ROW3 - PD6
ROW4 - PD7
COL1 - PD0
COL2 - PD1
COL3 - PD2
COL4 - PD3


*/


#define ADDRESS 0x00
#define KEY_ADDRESS 0X0A

const char alphabet[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', '*', '#'};

unsigned char keypad[4][4] = {{'1', '2', '3', 'A'},
                              {'4', '5', '6', 'B'},
                              {'7', '8', '9', 'C'},
                              {'*', '0', '#', 'D'}};

unsigned char colloc, rowloc;

// LiquidCrystal lcd(0, 1, 5, 4, 3, 2);
LiquidCrystal lcd(8, 9, 13, 12, 11, 10);

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

void EEPROMwrite(unsigned int address, char data)
{

    // wait for completion of previous write
    while (EECR & (1 << EEPE))
        ;

    // set up address and data regs
    EEAR = address;
    EEDR = data;

    // Write logical 1 to EEMPE
    EECR |= (1 << EEMPE);

    // start eeprom write
    EECR |= (1 << EEPE);
}

char EEPROMread(unsigned int address)
{
    // Wait for completion of writes
    while (EECR & (1 << EEPE))
        ;

    // set up address
    EEAR = address;

    // start eeprom read
    EECR |= (1 << EERE);

    // return data
    return EEDR;
}

char getKeyPressed()
{
    while (1)
    {
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

void storeAndEncrypt()
{
    unsigned int i;
    char c;
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Enter 10 Chars");
    _delay_ms(500);
    lcd.setCursor(0, 1);

    // enter each character
    for (i = 0; i < 10; i++)
    {
        c = getKeyPressed(); // get pressed key
        EEPROMwrite(ADDRESS + i, c);
        // eeprom_write_byte((uint8_t *)(ADDRESS + i), c);
        lcd.print(c); // print character
    }

    _delay_ms(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Encrypted Text");
    lcd.setCursor(0, 1);

    _delay_ms(300);

    // encript char by char and show
    for (i = 0; i < 10; i++)
    {
        // read from the memory
        c = EEPROMread(ADDRESS + i);

        lcd.print(encryptChar(c));
        _delay_ms(300);
    }

    _delay_ms(1000);
    lcd.clear();
    lcd.print("A to main menu");
    while ((c = getKeyPressed()) != 'A')
        ;
}

char encryptChar(char c)
{
    int i;

    // find the index
    if (c >= 48 && c <= 57) // for numbers
        i = c - 48;

    else if (c >= 65 && c <= 68) // for A,B,C,D
        i = c - 65 + 10;

    else if (c == '*')
        i = 14;

    else
        i = 15;

    // read the key from the memory
    // char secret_key = eeprom_read_byte((uint8_t *)KEY_ADDR);
    char secret_key = EEPROMread(KEY_ADDR);

    // encrypt
    return alphabet[(i + secret_key) % ALPHABET_SIZE];
}

void showSecretKey()
{
    char c;

    // read from the memory
    // c = eeprom_read_byte((uint8_t *)KEY_ADDR);
    c = EEPROMread(KEY_ADDR);

    // display
    lcd.clear();
    lcd.print("Secret Key");
    lcd.setCursor(0, 1);
    lcd.print(c);

    _delay_ms(2000);
    lcd.clear();
    lcd.print("A to main menu");
    while ((c = getKeyPressed()) != 'A')
        ;
}

void changeSecretKey()
{
    char c;
    lcd.clear();
    lcd.print("Enter New Key");
    lcd.setCursor(0, 1);

    while ((c = getKeyPressed()) < '0' && c > '9')
        ;
    lcd.print(c);
    EEPROMwrite((uint8_t *)KEY_ADDR, c);
    // eeprom_write_byte((uint8_t *)KEY_ADDR, c);

    _delay_ms(2000);
}

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
        lcd.print("Encrypt : 1");
        lcd.setCursor(0, 1);
        lcd.print("Secret Key : 2");

        while ((c = getKeyPressed()) != '1' && c != '2')
            ;

        if (c == '1')
        {
            storeAndEncrypt();
        }

        else
        {
            lcd.clear();
            lcd.print("Show key : 1");
            lcd.setCursor(0, 1);
            lcd.print("Change key : 2");

            while ((c = getKeyPressed()) != '1' && c != '2')
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