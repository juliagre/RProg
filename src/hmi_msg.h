#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_

#define VER_FW "Version: %S built on: %S %S\n"
#define VER_LIBC "avr-libc version: %S"
#define VER_GCC "avr-gcc version: %S\n"
#define GET_MONTH "Enter Month name first letter >"
#define STUD_NAME   "Julia Gretšanaja"
#define UPTIME "Uptime: %lu s"

#define CLI_HELP_CDM "help"
#define CLI_HELP_HELP "Get help"
#define CLI_VER_CMD "version"
#define CLI_VER_HELP "Print FW version"
#define CLI_ASCII_CMD "ascii"
#define CLI_ASCII_HELP "print ASCII tables"
#define CLI_MONTH_CMD "month"
#define CLI_MONTH_HELP "Find matching month from lookup list. Usage: month <string>"
#define CLI_HELP_MSG "Implemented commands:"
#define CLI_NO_CMD "Command not implemented.\n Use <help> to get help."
#define CLI_ARGS "To few or to many arguments for this command\nUse <help>"

extern PGM_P const nameMonths[];

extern const char help_cmd[];
extern const char help_help[];
extern const char ver_cmd[];
extern const char ver_help[];
extern const char ascii_cmd[];
extern const char ascii_help[];
extern const char month_cmd[];
extern const char month_help[];

#endif /* _HMI_MSG_H_ */
