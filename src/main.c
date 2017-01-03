#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "print_helper.h"
#include "uart-wrapper.h"
#include <util/atomic.h>
#include "hmi_msg.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/helius_microrl/microrl.h"
#include "cli_microrl.h"
#include <assert.h>
#define __ASSERT_USE_STDERR
#define BAUD 9600

volatile uint32_t counter;
//Create microrl object and pointer on it
microrl_t rl;
microrl_t *prl = &rl;

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

static inline void print_user_info()
{
    /* print and display my name*/
    fprintf_P(stdout, PSTR(STUD_NAME "\n"));
    lcd_puts_P(PSTR(STUD_NAME));
}

static inline void heartbeat()
{
    static uint32_t time_prev;
    uint32_t time_cpy;
    time_cpy = time_prev;

    if (time_prev != time_cpy) {
        PORTA ^= _BV(PORTA3);
        fprintf_P(stderr, PSTR(UPTIME "\n"), time_cpy);
    }

    time_prev = time_cpy;
}

static inline void microrl_in()
{
    // embed lib to you project
    // Call init with ptr to microrl instance and print callback
    microrl_init (prl, cli_print);
    // Set callback for execute
    microrl_set_execute_callback (prl, cli_execute);
}

int main (void)
{
    init_p();
    print_user_info();
    microrl_in();

    while (1) {
        heartbeat();
        microrl_insert_char (prl, cli_get_char());
    }
}

/* System clock ISR */
ISR(TIMER5_COMPA_vect)
{
    counter++;
}
