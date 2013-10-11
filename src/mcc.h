/*
 * mcc.h
 *
 *  Created on: Oct 4, 2013
 *      Author: jvisca
 */

#ifndef MCC_H_
#define MCC_H_

#define MAX_POLL_CALLBACKS 50
#define MAX_PIDS 50

//#define EVCODE_STARTED 2

typedef void (*PollCallback) ();
typedef void (*OpcodeCallback) (int pid, uint8_t opcode, char *data, uint8_t data_length);
typedef OpcodeCallback* OpcodeCallbacks;
struct OpcodesCallbackReg {
	OpcodeCallbacks callbacks;
	uint8_t n_callbacks;
};
typedef OpcodesCallbackReg OpcodesCalbbackReg;


class MCC {
	PollCallback poll_callbacks[MAX_POLL_CALLBACKS];
	int n_poll_callbacks;

	OpcodesCallbackReg opcode_callbacks[MAX_PIDS];
	//uint8_t opcode_callbacks_count[MAX_PIDS]; //FIXME merge with previous as an array of structs.
	int n_opcode_callbacks;

    void process_incomming();

public:
	MCC();

	int register_poll_callback(PollCallback cb);
	void unregister_poll_callback(int);

	int register_opcode_callbacks(OpcodeCallback* opcode_callbacks, uint8_t opcodes_count);

    void send_message(int spid, uint8_t opcode, char *data, uint8_t data_length);

	void tick();
};

#endif /* MCC_H_ */
