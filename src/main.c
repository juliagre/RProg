#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "print_helper.h"
#include "uart.h"
#include "hmi_msg.h"
#include "../lib/hd44780_111/hd44780.h"

#define BLINK_DELAY_MS 100


int main (void)
{
    /* Set pin 3 of PORTA for output*/
    DDRA |= _BV(DDA3);
    /* Init stdin and stdout in UART 0 and stderr in UART 3*/
    uart0_init();
    uart3_init();
    stdin = stdout = &uart0_io;
    stderr = &uart3_out;
    /* init LCD*/
    lcd_init();
    lcd_clrscr();
    fprintf_P(stderr, PSTR(VER_FW),
              PSTR(GIT_DESCR), PSTR(__DATE__), PSTR(__TIME__));
    fprintf_P(stderr, PSTR(VER_LIBC), __AVR_LIBC_VERSION_STRING__ );
    /* print and display my name*/
    fprintf_P(stdout, PSTR(STUD_NAME "\n"));
    lcd_puts_P(PSTR(STUD_NAME));
    /*print ASCII table*/
    print_ascii_tbl(stdout);
    unsigned char ascii[128];

    for (unsigned char i = 0; i < 128; i++) {
        ascii[i] = i;
    }

    print_for_human(stdout, ascii, 128);

    while (1) {
        /*set pin 3 high to turn led on*/
        PORTA |= _BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
        /*char buffer*/
        char firstletter;
        fprintf(stdout, PSTR(GET_MONTH));
        fscanf(stdin, "%c", &firstletter);
        fprintf(stdout, "%c\n", firstletter);
        lcd_goto(0x40);
        /*find and print month */
        for (int i = 0; i < 6; i++) {
            if (!strncmp(&firstletter, (PGM_P)pgm_read_word(&nameMonths[i]), 1)) {
                fprintf_P(stdout, (PGM_P)pgm_read_word(&nameMonths[i]));
                fputc('\n', stdout);
                lcd_puts_P((PGM_P)pgm_read_word(&nameMonths[i]));
                lcd_putc(' ');
            }
        }
        lcd_puts_P(PSTR("                "));
        /*set pin 3 low to turn led off*/
        PORTA &= ~_BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
    }
}
