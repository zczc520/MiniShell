#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

//success
#define ERR_SUCC 0 // success code

// overmuch error
#define ERR_OVER_TOKEN -0x10 // overmuch tokens
#define ERR_OVER_OPTION -0x11 // overmuch options
#define ERR_OVER_ARG -0x12 // toomuch arguments
#define ERR_OVER_INPUT -0x13 // toomuch input source
#define ERR_OVER_OUTPUT -0x14 // toomuch output channel
#define ERR_OVER_CMD -0x15 // toomuch commands

// invalid error
#define ERR_INVALID_PIPE -0x20 // invalid using of pipe symbol(blank after pipe)
#define ERR_INVALID_OUTPUT_TYPE -0x21 // invalid output type
#define ERR_INVALID_OPTION -0x22 // invalid option character

// C operation error
#define ERR_MEMCPY -0x30 // memcpy error
#define ERR_STRCPY -0x31 // strcpy error

extern void printerr(int errcode);

#endif
