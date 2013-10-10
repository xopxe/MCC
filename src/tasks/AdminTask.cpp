#include <mbed.h>
#include "AdminTask.h"

extern MCC mcc;

void handle_ping(int pid, uint8_t opcode, char *data, uint8_t data_length) {
	mcc.send_message(pid, OPCODE_PING, data, data_length);
}

void tick_admin() {
	//mcc.send_message(pid, 0, NULL, 0);
}

AdminTask::AdminTask() {
	for (int i=0; i<OPCODES; ++i) {
		AdminTask::opcode_callbacks[i]=NULL;
	}
	AdminTask::opcode_callbacks[OPCODE_PING] = &handle_ping;
	AdminTask::pid = mcc.register_opcode_callbacks(AdminTask::opcode_callbacks, OPCODES);
}

