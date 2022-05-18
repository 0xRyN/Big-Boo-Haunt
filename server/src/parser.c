#include "parser.h"

// WARNING : Carefully follow the protocol

// Protocol : NEWPL id port***
struct NEWPL parse_newpl(char* str) {
    struct NEWPL res;
    res.opcode = OP_NEWPL;
    memcpy(res.id, str + 6, 8);
    res.id[8] = '\0';

    // Convert port to int
    char port[5];
    memcpy(port, str + 15, 4);
    port[4] = '\0';
    res.port = atoi(port);

    return res;
}

// Protocol : REGIS id port m***
struct REGIS parse_regis(char* str) {
    struct REGIS res;
    res.opcode = OP_REGIS;
    memcpy(res.id, str + 6, 8);
    res.id[8] = '\0';

    // Convert port to int
    char port[5];
    memcpy(port, str + 15, 4);
    port[4] = '\0';
    res.port = atoi(port);
    u_int8_t game_id;
    memcpy(&game_id, str + 19, 1);
    puts("I am here - here");
    res.game_id = game_id;
    printf("%d\n", res.game_id);

    return res;
}

int parse_operation(char* str) {
    char op[6];
    memcpy(op, str, 5);
    op[5] = '\0';
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
