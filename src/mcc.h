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

typedef void (*PollCallback) ();
typedef void (*OpcodeCallback) (int pid, uint8_t opcode, char *data, uint8_t data_length);
typedef OpcodeCallback* OpcodeCallbacks;


class MCC {
	PollCallback poll_callbacks[MAX_POLL_CALLBACKS];
	int n_poll_callbacks;

	OpcodeCallbacks opcode_callbacks[MAX_PIDS];
	int n_opcode_callbacks;

public:
	MCC();

	int register_poll_callback(PollCallback cb);
	void unregister_poll_callback(int);

	int register_opcode_callbacks(OpcodeCallback* opcode_callbacks, uint8_t opcodes_count);

    void process_incomming();
    void send_message(int spid, uint8_t opcode, char *data, uint8_t data_length);

	void tick();
};

#endif /* MCC_H_ */
