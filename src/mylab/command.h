#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "dstruct.h"

void command(Statement *stmt);
void do_exit();
void do_who();
void do_whos();
void do_clear(Statement *stmt);

#endif
