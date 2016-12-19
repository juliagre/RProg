#include <avr/pgmspace.h>
#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_

#define VER_FW "Version: %S built on: %S %S\n"
#define VER_LIBC "avr-libc version: %S\n"
#define GET_MONTH "Enter Month name first letter >"
#define STUD_NAME   "Julia Gretšanaja"
#define UPTIME "Uptime: %lu s"

const char month_1[] PROGMEM = "January";
const char month_2[] PROGMEM = "February";
const char month_3[] PROGMEM = "March";
const char month_4[] PROGMEM = "April";
const char month_5[] PROGMEM = "May";
const char month_6[] PROGMEM = "June";

PGM_P const nameMonths[] PROGMEM = {month_1,month_2,month_3,month_4,month_5,month_6};

#endif /* _HMI_MSG_H_ */
