#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "hmi_msg.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "print_helper.h"
#include "cli_microrl.h"

typedef struct  cli_cmd {
    PGM_P cmd;
    PGM_P help;
    void (*func_p)();
    const uint8_t func_argc;
} cli_cmd_t;

const cli_cmd_t cli_cmds[] = {
    {help_cmd, help_help, cli_print_help, 0},
    {ver_cmd, ver_help, cli_print_ver, 0},
    {ascii_cmd, ascii_help, cli_print_ascii_tbls, 0},
    {month_cmd, month_help, cli_handle_month, 1}
};

void cli_print(const char *str)
{
    printf("%s", str);
}

char cli_get_char(void)
{
    if (uart0_peek() != UART_NO_DATA) {
        return uart0_getc() & UART_STATUS_MASK;
    } else {
        return 0x00;
    }
}

void cli_print_help(const char *const *argv)
{
    (void) argv;
    printf_P(PSTR("\n"));
    printf_P(PSTR(CLI_HELP_MSG));
    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        printf_P(cli_cmds[i].cmd);
        printf_P(PSTR(" : "));
        printf_P(cli_cmds[i].help);
        printf_P(PSTR("\n"));
    }
}

void print_avr_version(FILE *stream)
{
    fprintf_P(stream, PSTR(VER_FW),
              PSTR(GIT_DESCR), PSTR(__DATE__), PSTR(__TIME__));
    fprintf_P(stream, PSTR(VER_LIBC), PSTR(__AVR_LIBC_VERSION_STRING__));
    fprintf_P(stream, PSTR(" "VER_GCC), PSTR(__VERSION__));
}

void cli_print_ver(const char *const *argv)
{
    (void) argv;
    printf_P(PSTR("\n"));
    print_avr_version(stdout);
}

void cli_print_ascii_tbls(const char *const *argv)
{
    (void) argv;
    printf_P(PSTR("\n"));
    print_ascii_tbl(stdout);
    unsigned char ascii[128] = {0};
    for (unsigned char i = 0; i < 128; i++) {
        ascii[i] = i;
    }

    print_for_human(stdout, ascii, 128);
}

void cli_handle_month(const char *const *argv)
{
    printf_P(PSTR("\n"));
    lcd_goto(0x40);
    char lcd_spaces = 16;
    for (int i = 0; i < 6; i++) {
        if (!strncmp_P(argv[1], (PGM_P)pgm_read_word(&nameMonths[i]),
                       strlen(argv[1]))) {
            char lcd_count;
            lcd_count = fprintf_P(stdout, (PGM_P)pgm_read_word(&nameMonths[i]));
            printf_P(PSTR("\n"));
            lcd_puts_P((PGM_P)pgm_read_word(&nameMonths[i]));
            lcd_putc(' ');
            lcd_spaces -= (lcd_count + 1);
        }
    }

    for (; lcd_spaces >= 0; lcd_spaces--) {
        lcd_putc(' ');
    }
}

void cli_print_cmd_error(void)
{
    printf_P(PSTR("\n"));
    printf_P(PSTR(CLI_ARGS));
}

void cli_print_cmd_arg_error(void)
{
    printf_P(PSTR("\n"));
    printf_P(PSTR(CLI_ARGS));
}

int cli_execute(int argc, const char *const *argv)
{
    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        if (!strcmp_P(argv[0], cli_cmds[i].cmd)) {
            //Test do we have correct arguments to run command
            // Function arguments count shall be defined in  struct
            if ((argc - 1) != cli_cmds[i].func_argc) {
                cli_print_cmd_arg_error();
                return 0;
            }
            //Hand argv over to function pointer,
            //cross fingers and hope that funcion handlesit properly
            cli_cmds[i].func_p (argv);
            return 0;
        }
    }
    cli_print_cmd_error();
    return 0;
}
