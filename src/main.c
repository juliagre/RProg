#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 1000


int main (void)
{
    
    DDRA |= _BV(DDA3);
    
    while(1){
        
        PORTA |= _BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
        
        
        PORTA &= ~_BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
    }

}
