#include "parser.h"

// WARNING : Carefully follow the protocol

// Protocol : NEWPL id port***
struct NEWPL parse_newpl(char* str) {
    struct NEWPL res;
    res.opcode = OP_NEWPL;
    memcpy(res.id, str + 6, 8);
    res.id[9] = '\0';
    memcpy(res.port, str + 14, 4);
    res.port[5] = '\0';
    return res;
}

// Protocol : REGIS id port m***
struct REGIS parse_regis(char* str) {
    struct REGIS res;
    res.opcode = OP_REGIS;
    memcpy(res.id, str + 6, 8);
    res.id[9] = '\0';
    memcpy(res.port, str + 14, 4);
    res.port[5] = '\0';
    res.game_id = str[18];
    return res;
}

int parse_operation(char* str) {
    char op[5] = memcpy(op, str, 5);
    if (strcmp(op, "NEWPL") == 0) {
        return OP_NEWPL;
    }

    else if (strcmp(op, "REGIS") == 0) {
        return OP_REGIS;
    }

    // TODO: Add all operations

    else {
        return -1;
    }
}