#ifndef __task_ticker_h
#define __task_ticker_h

#include "mcc.h"

#define OPCODES 2
#define OPCODE_TICKTOGGLE 1

class TickerTask
{
	int pid;
	OpcodeCallback opcode_callbacks[OPCODES];
  public:
	TickerTask ();
};


#endif
