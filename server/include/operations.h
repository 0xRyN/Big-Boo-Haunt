#ifndef __OPERATIONS_H__
#define __OPERATIONS_H__

// E at the end : Exclamation mark
// Q at the end : Question mark

#include <sys/types.h>

#define OP_GAMES 0x01
#define OP_OGAME 0x02
#define OP_NEWPL 0x03
#define OP_REGIS 0x04
#define OP_REGOK 0x05
#define OP_REGNO 0x06
#define OP_START 0x07
#define OP_UNREG 0x08
#define OP_UNROK 0x09
#define OP_DUNNO 0x0A
#define OP_SIZEQ 0x0B
#define OP_SIZEE 0x0C
#define OP_LISTQ 0x0D
#define OP_LISTE 0x0E
#define OP_PLAYR 0x0F
#define OP_GAMEQ 0x10
#define OP_WELCO 0x11
#define OP_POSIT 0x12
#define OP_UPMOV 0x13
#define OP_DOMOV 0x14
#define OP_LEMOV 0x15
#define OP_RIMOV 0x16
#define OP_MOVEE 0x17
#define OP_MOVEF 0x18
#define OP_IQUIT 0x19
#define OP_GOBYE 0x1A
#define OP_GLISQ 0x1B
#define OP_GLISE 0x1C
#define OP_GPLYR 0x1D
#define OP_MALLQ 0x1E
#define OP_MALLE 0x1F
#define OP_SENDQ 0x20
#define OP_SENDE 0x21
#define OP_NSEND 0x22

struct NEWPL {
    int opcode;
    char id[8 + 1];
    char port[4 + 1];
};

struct REGIS {
    int opcode;
    char id[8 + 1];
    char port[4 + 1];
    u_int8_t game_id;
};

#endif