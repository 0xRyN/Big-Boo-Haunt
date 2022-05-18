#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operations.h"

struct REGIS parse_regis(char* str);
struct NEWPL parse_newpl(char* str);
struct LISTQ parse_listq(char* str);
int parse_operation(char* str);

#endif
