/* define FCY before including libpic30.h */
#define FCY 3685000UL

#include "xc.h"
#include <libpic30.h>
#include <string.h>

/* set Configuration Bits */
#pragma config ICS = PGD2    // Communicate on PGED2 (pin 14) and PGEC2 (pin 15)
#pragma config JTAGEN = OFF  // Disable JTAG inorder to use RB8 through RB11

/* define constants for programming the LCD module */
#define pgm_delay_ms 2
#define INSTRUCTION 0
#define DATA 1

/* declare functions */
void Init_DIO_Ports (void);
void Toggle_Enable_line (void);
void Write_LCD_Nibble(int data, int cmd);
void Write_LCD_Byte(int data, int cmd);
void Init_LCD_Module(void);

void Init_DIO_Ports (void) {
    ANSELB = 0;     // use peripheral pins associated with PORTB for digital I/O
    TRISB = 0xC0FF; // set RB8 to RB13 for output, the rest for input
    PORTB =  0;     // set all LCD inputs low
  }

void Toggle_Enable_line (void) {
    __delay_ms(pgm_delay_ms);   // delay
    PORTBbits.RB13 = 1;         // set E high
    __delay_ms(pgm_delay_ms);   // delay
    PORTBbits.RB13 = 0;         // set E low
    __delay_ms(pgm_delay_ms);   // delay
}

void Write_LCD_Nibble(int data, int cmd) {
 PORTB =  data << 8;     // place nibble at LCD inputs DB4 through DB7
 PORTBbits.RB12 = cmd;   // set RS; cmd = 0 for instruction, 1 for data
 Toggle_Enable_line ();  // strobe data into LCD module
 PORTB =  0;             // set all LCD inputs low
}

void Write_LCD_Byte(int data, int cmd) {
    Write_LCD_Nibble((data & 0x00F0) >> 4, cmd); // write upper nibble
    Write_LCD_Nibble( data & 0x000F, cmd);       // write lower nibble
}

void Init_LCD_Module(void) {
    Write_LCD_Nibble(0b0011, INSTRUCTION);  // Initialize the LCD Module
    Write_LCD_Nibble(0b0011, INSTRUCTION);
    Write_LCD_Nibble(0b0011, INSTRUCTION);
    Write_LCD_Nibble(0b0010, INSTRUCTION);  // invoke 4-bit mode
    Write_LCD_Byte(0b00101000, INSTRUCTION);// 4-bit mode, two-line,5X7 dot
    Write_LCD_Byte(0b00000001, INSTRUCTION);// clear display, cursor at 0x00
    Write_LCD_Byte(0b00001111, INSTRUCTION);// display on,cursor blink/underline
}

int main(void) {
    int idx;
    char Line_1_char_Array[] = "   WATER LOW";
    char Line_2_char_Array[] = "  REFILL TANK";

    Init_DIO_Ports();
    Init_LCD_Module();

    Write_LCD_Byte(0b10000000, INSTRUCTION);// place cursor at cell 0x00
    for (idx = 0; idx < strlen(Line_1_char_Array); idx++)
        Write_LCD_Byte(Line_1_char_Array[idx], DATA);
    

    Write_LCD_Byte(0b11000000, INSTRUCTION);// place cursor at cell 0x40
    for (idx = 0; idx < strlen(Line_2_char_Array); idx++)
        Write_LCD_Byte(Line_2_char_Array[idx], DATA);
    
    while (1) {
        __delay_ms(100); // wait 100 ms
        ClrWdt();        // restart the watchdog timer
    }
   return 0;
}
