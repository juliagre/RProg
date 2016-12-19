#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "print_helper.h"
#include "uart-wrapper.h"
#include "hmi_msg.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"

#define BAUD 9600
#define BLINK_DELAY_MS 100

volatile uint32_t counter;

static inline void init_system_clock(void)
{
    TCCR5A = 0; // Clear control register A
    TCCR5B = 0; // Clear control register B
    TCCR5B |= _BV(WGM52) | _BV(CS52); // CTC and fCPU/256
    OCR5A = 62549; // 1 s
    TIMSK5 |= _BV(OCIE5A); // Output Compare A Match Interrupt Enable
}

static inline void init_p()
{
    /* Set pin 3 of PORTA for output*/
    DDRA |= _BV(DDA3);
    /* Init stdin and stdout in UART 0 and stderr in UART 3*/
    uart0_init(UART_BAUD_SELECT(BAUD, F_CPU));
    uart3_init(UART_BAUD_SELECT(BAUD, F_CPU));
    stdout = stdin = &uart0_io;
    stderr = &uart3_out;
    /* Init counter*/
    init_system_clock();
    /* init LCD*/
    lcd_init();
    lcd_clrscr();
    /*Enable all interrupts*/
    sei();
}

static inline void print_avr_version()
{
    fprintf_P(stderr, PSTR(VER_FW),
              PSTR(GIT_DESCR), PSTR(__DATE__), PSTR(__TIME__));
    fprintf_P(stderr, PSTR(VER_LIBC), PSTR(__AVR_LIBC_VERSION_STRING__));
}

static inline void print_user_info()
{
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
    fprintf_P(stdout, PSTR(GET_MONTH));
}

static inline void month_info()
{
    char firstletter;
    fscanf(stdin, "%c", &firstletter);
    fprintf(stdout, "%c\n", firstletter);
    lcd_goto(0x40);

    /*find and print month */
    for (int i = 0; i < 6; i++) {
        if (!strncmp_P(&firstletter, (PGM_P)pgm_read_word(&nameMonths[i]), 1)) {
            fprintf_P(stdout, (PGM_P)pgm_read_word(&nameMonths[i]));
            fputc('\n', stdout);
            lcd_puts_P((PGM_P)pgm_read_word(&nameMonths[i]));
            lcd_putc(' ');
        }
    }

    fprintf_P(stdout, PSTR(GET_MONTH));
}

static inline void heartbeat()
{
    static uint32_t time_prev;
    uint32_t time_cpy;
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        time_cpy = counter;
    }

    if ((time_prev - time_cpy) > 0) {
        // Toggle led on arduino pin 25
        PORTA ^= _BV(PORTA3);
        fprintf_P(stderr, PSTR(UPTIME "\n"), time_cpy);
    }

    time_prev = time_cpy;
}

int main (void)
{
    init_p();
    print_avr_version();
    print_user_info();

    while (1) {
        heartbeat();

        if (uart0_available()) {
            month_info();
        }
    }
}

/* System clock ISR */
ISR(TIMER5_COMPA_vect)
{
    counter++;
}
