#include <mbed.h>
#include "AdminTask.h"

extern MCC mcc;
extern "C" void mbed_reset();

void handle_ping(int pid, uint8_t opcode, char *data, uint8_t data_length) {
	mcc.send_message(pid, OPCODE_PING, data, data_length);
}
void handle_reset(int pid, uint8_t opcode, char *data, uint8_t data_length) {
	mbed_reset();
}

void tick_admin() {
	//mcc.send_message(pid, 0, NULL, 0);
}

AdminTask::AdminTask() {
	for (int i=0; i<ADMIN_OPCODES; ++i) {
		AdminTask::opcode_callbacks[i]=NULL;
	}
	AdminTask::opcode_callbacks[OPCODE_PING] = &handle_ping;
	AdminTask::opcode_callbacks[OPCODE_RESET] = &handle_reset;
	AdminTask::pid = mcc.register_opcode_callbacks(AdminTask::opcode_callbacks, ADMIN_OPCODES);
}

