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
    memcpy(res.port, str + 15, 4);
    port[4] = '\0';

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
    memcpy(res.port, str + 15, 4);
    port[4] = '\0';
    u_int8_t game_id;
    memcpy(&game_id, str + 19, 1);
    puts("I am here - here");
    res.game_id = game_id;
    printf("%d\n", res.game_id);

    return res;
}

// Protocol : LISTQ id***
struct LISTQ parse_listq(char* str) {
    struct LISTQ res;
    res.opcode = OP_LISTQ;
    memcpy(&res.game_id, str + 6, 1);
    return res;
}

struct SIZEQ parse_sizeq(char* str) {
    struct SIZEQ res;
    res.opcode = OP_SIZEQ;
    memcpy(&res.game_id, str + 6, 1);
    return res;
}

struct SENDQ parse_sendq(char* str) {
    struct SENDQ res;
    res.opcode = OP_SENDQ;
    char* token;
    memcpy(res.id, str + 6, 8);
    res.id[8] = '\0';
    token = strtok(str, "***");
    strcpy(res.message, token + 15);
    return res;
}

struct PLMOV parse_plmov(char* str) {
    struct PLMOV res;
    strncpy(res.distance, str + 6, 3);
    printf("%s\n", res.distance);
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

    else if (strcmp(op, "LIST?") == 0) {
        return OP_LISTQ;
    } else if (strcmp(op, "UNREG") == 0) {
        return OP_UNREG;
    } else if (strcmp(op, "START") == 0) {
        return OP_START;
    }

    else if (strcmp(op, "SIZE?") == 0) {
        return OP_SIZEQ;
    }

    else if (strcmp(op, "GAME?") == 0) {
        return OP_GAMEQ;
    }

    else if (strcmp(op, "UPMOV") == 0) {
        return OP_UPMOV;
    }

    else if (strcmp(op, "LEMOV") == 0) {
        return OP_LEMOV;
    }

    else if (strcmp(op, "RIMOV") == 0) {
        return OP_RIMOV;
    }

    else if (strcmp(op, "DOMOV") == 0) {
        return OP_DOMOV;
    }

    else if (strcmp(op, "IQUIT") == 0) {
        return OP_IQUIT;
    }

    else if (strcmp(op, "GLIS?") == 0) {
        return OP_GLISQ;
    }

    else if (strcmp(op, "MALL?") == 0) {
        return OP_MALLQ;
    }

    else if (strcmp(op, "SEND?") == 0) {
        return OP_SENDQ;
    }

    // TODO: Add all operations

    else {
        return -1;
    }
}
