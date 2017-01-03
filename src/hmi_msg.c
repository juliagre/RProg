#include <avr/pgmspace.h>
#include "hmi_msg.h"

static const char month_1[] PROGMEM = "January";
static const char month_2[] PROGMEM = "February";
static const char month_3[] PROGMEM = "March";
static const char month_4[] PROGMEM = "April";
static const char month_5[] PROGMEM = "May";
static const char month_6[] PROGMEM = "June";

PGM_P const nameMonths[] PROGMEM = {month_1, month_2, month_3, month_4, month_5, month_6};

const char help_cmd[] PROGMEM = CLI_HELP_CDM;
const char help_help[] PROGMEM = CLI_HELP_HELP;
const char ver_cmd[] PROGMEM = CLI_VER_CMD;
const char ver_help[] PROGMEM = CLI_VER_HELP;
const char ascii_cmd[] PROGMEM = CLI_ASCII_CMD;
const char ascii_help[] PROGMEM = CLI_ASCII_HELP;
const char month_cmd[] PROGMEM = CLI_MONTH_CMD;
const char month_help[] PROGMEM = CLI_MONTH_HELP;
