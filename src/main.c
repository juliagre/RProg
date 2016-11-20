#include <stdio.h>
#include <assert.h>
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

#define BLINK_DELAY_MS 1000


int main (void)
{
    DDRA |= _BV(DDA3);
    uart3_init();
    stderr = &uart3_out;
    fprintf(stderr, "Version: %s built on: %s %s\n",
            GIT_DESCR, __DATE__, __TIME__);
    fprintf(stderr, "avr-libc version: %s\n", __AVR_LIBC_VERSION_STRING__);
    char *array;
    uint32_t i = 1;
    extern int __heap_start, *__brkval;
    int v;
    array = malloc( i * sizeof(char));
    assert(array);

    while (1) {
        PORTA |= _BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
        array = realloc( array, (i++ * 100) * sizeof(char));
        fprintf(stderr, "%d ",
                (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval));
        assert(array);
        PORTA &= ~_BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
    }
}
