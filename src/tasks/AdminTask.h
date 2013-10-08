#ifndef __task_admin_h
#define __task_admin_h

#include "mcc.h"

#define OPCODES 2
#define OPCODE_PING 1

class AdminTask
{
	int pid;
	OpcodeCallback opcode_callbacks[OPCODES];
  public:
	AdminTask ();
};


#endif
