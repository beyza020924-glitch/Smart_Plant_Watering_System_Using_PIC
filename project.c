#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 20000000

// Configuration Bits
#pragma config FOSC = HS        
#pragma config WDTE = OFF       
#pragma config PWRTE = ON       
#pragma config BOREN = ON       
#pragma config LVP = OFF        
#pragma config CPD = OFF        
#pragma config WRT = OFF        
#pragma config CP = OFF         

// LCD pin 
#define RS RC1
#define EN RC2
#define D4 RC3
#define D5 RC4
#define D6 RC5
#define D7 RC6

#define MOTOR RC0  // Motor relay
#define MIN_ADC 224
#define MAX_ADC 824
#define THRESHOLD 30  // %30 and below → motor works

// LCD command function
void lcd_cmd(unsigned char cmd) {
    RS = 0;
    D4 = (cmd >> 4) & 1;
    D5 = (cmd >> 5) & 1;
    D6 = (cmd >> 6) & 1;
    D7 = (cmd >> 7) & 1;
    EN = 1; __delay_ms(1); EN = 0;

    D4 = cmd & 1;
    D5 = (cmd >> 1) & 1;
    D6 = (cmd >> 2) & 1;
    D7 = (cmd >> 3) & 1;
    EN = 1; __delay_ms(1); EN = 0;

    __delay_ms(2);
}

// LCD character writing
void lcd_char(unsigned char data) {
    RS = 1;
    D4 = (data >> 4) & 1;
    D5 = (data >> 5) & 1;
    D6 = (data >> 6) & 1;
    D7 = (data >> 7) & 1;
    EN = 1; __delay_ms(1); EN = 0;

    D4 = data & 1;
    D5 = (data >> 1) & 1;
    D6 = (data >> 2) & 1;
    D7 = (data >> 3) & 1;
    EN = 1; __delay_ms(1); EN = 0;

    __delay_ms(2);
}

// LCD starting
void lcd_init(void) {
    TRISC = 0x00;
    __delay_ms(20);
    lcd_cmd(0x02);    
    lcd_cmd(0x28);    
    lcd_cmd(0x0C);    
    lcd_cmd(0x06);    
    lcd_cmd(0x01);    
    __delay_ms(2);
}

// LCD string 
void lcd_string(const char *str) {
    while (*str) {
        lcd_char(*str++);
    }
}

// LCD cursor positioning
void lcd_set_cursor(unsigned char row, unsigned char col) {
    unsigned char pos = (row == 1) ? 0x80 + (col - 1) : 0xC0 + (col - 1);
    lcd_cmd(pos);
}

// ADC reading (left-justified)
unsigned int readADC(unsigned char channel) {
    ADCON0 = 0x01 | (channel << 3);  // choose chanel, ADC open
    __delay_us(20);
    GO_nDONE = 1;
    while(GO_nDONE);
    return ((ADRESH << 2) | (ADRESL >> 6));  // 10-bit transformation
}

void main(void) {
    unsigned int adcValue, percent;
    char buffer[16];

    TRISA = 0xFF;      // RA0 input (moisture sensor)
    TRISC = 0x00;      // All RC pins output
    ADCON1 = 0x06;     // RA0 analog, others digital

    lcd_init();

    while (1) {
        adcValue = readADC(0);  // RA0 (AN0) read

        
        if (adcValue < MIN_ADC) adcValue = MIN_ADC;
        if (adcValue > MAX_ADC) adcValue = MAX_ADC;

        
        percent = (adcValue - MIN_ADC) * 100 / (MAX_ADC - MIN_ADC);
        percent = 100 - percent;

        
        lcd_cmd(0x01);
        __delay_ms(2);

        // 1. row: ADC value
        lcd_set_cursor(1, 1);
        sprintf(buffer, "ADC: %4u     ", adcValue);
        lcd_string(buffer);

        // 2. row: Moisture percentage
        lcd_set_cursor(2, 1);
        sprintf(buffer, "Moisture: %2u%% ", percent);
        lcd_string(buffer);

        
        if (percent < THRESHOLD)
            MOTOR = 0;
        else
            MOTOR = 1;

        __delay_ms(3000);  // 3 saniyede bir ölçüm
    }


}
