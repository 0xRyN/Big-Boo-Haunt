#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operations.h"

struct REGIS parse_regis(char* str);
struct NEWPL parse_newpl(char* str);
struct LISTQ parse_listq(char* str);
struct SIZEQ parse_sizeq(char* str);
struct PLMOV parse_plmov(char* str);
struct SENDQ parse_sendq(char* str);
struct MALLQ parse_mallq(char* str);
int parse_operation(char* str);

#endif
