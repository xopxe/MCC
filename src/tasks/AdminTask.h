#ifndef __task_admin_h
#define __task_admin_h

#include "mcc.h"

#define ADMIN_OPCODES 3
#define OPCODE_PING 1
#define OPCODE_RESET 2


class AdminTask
{
	int pid;
	OpcodeCallback opcode_callbacks[ADMIN_OPCODES];
  public:
	AdminTask ();
};


#endif
