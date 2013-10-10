/*
 * mcc.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: jvisca
 */

#include "mbed.h"
#include "mcc.h"
#include "EmBencode/EmBencode.h"

Serial pc(USBTX, USBRX);

enum ProtocolStates {
	MESSAGE, TPID, OPCODE, DATA, END
};

enum ProtocolStates protocol_state = MESSAGE;

char embuf[200];
EmBdecode decoder(embuf, sizeof embuf);
EmBencode encoder;

int tpid;
uint8_t opcode;
char *data;
uint8_t data_length;

void EmBencode::PushChar(char ch) {
	//Serial.write(ch);
	pc.putc(ch);
}

MCC::MCC() {
	n_poll_callbacks = 0;
	n_opcode_callbacks = 0;
	//pc.baud(9600);
}

int MCC::register_poll_callback(PollCallback cb) {
	if (MCC::n_poll_callbacks >= MAX_POLL_CALLBACKS) {
		return -1;
	}
	MCC::poll_callbacks[MCC::n_poll_callbacks++] = cb;
	return MCC::n_poll_callbacks;
}

void MCC::unregister_poll_callback(int n) {
	if (n<MCC::n_poll_callbacks-1){
		MCC::poll_callbacks[n] = MCC::poll_callbacks[MCC::n_poll_callbacks-1];
	}
	--MCC::n_poll_callbacks;
}

void MCC::tick() {
	MCC::process_incomming();
	for (int i=0; i<MCC::n_poll_callbacks; ++i){
		MCC::poll_callbacks[i]();
	}
}

int MCC::register_opcode_callbacks(OpcodeCallbacks opcode_callbacks, uint8_t opcodes_count) {
	int pid = MCC::n_opcode_callbacks++;
	MCC::opcode_callbacks[pid] = opcode_callbacks;
	MCC::opcode_callbacks_count[pid] = opcodes_count;
	return pid;
}
//void unregister_opcode_callbacks(int pid);

/*
void MCC::process_echo() {
	if (pc.readable()) {
		pc.putc(pc.getc());
	}
}
*/

void MCC::send_message(int spid, uint8_t opcode, char *data, uint8_t data_length) {
	encoder.startList();
	encoder.push(spid);
	encoder.push(opcode);
	if (data!=NULL) {
		encoder.push(data, data_length);
	}
	encoder.endList();
}

void MCC::process_incomming() {
	if (pc.readable()) {
		char ch = pc.getc();
		pc.putc(ch);
		uint8_t bytes = decoder.process(ch);
		if (bytes > 0) {
			for (;;) {
				uint8_t token = decoder.nextToken();
				if (token == EmBdecode::T_END)
					break;

				switch (protocol_state) {
				case MESSAGE:
					if (token == EmBdecode::T_LIST) {
						//pc.putc('A');
						protocol_state = TPID;
					} else {
						//pc.putc('A');
						protocol_state = MESSAGE;
					}
					break;
				case TPID:
					if (token == EmBdecode::T_NUMBER) {
						//pc.putc('B');
						tpid = decoder.asNumber();
						protocol_state = OPCODE;
					} else {
						//pc.putc('B');
						protocol_state = MESSAGE;
					}
					break;
				case OPCODE:
					if (token == EmBdecode::T_NUMBER) {
						//pc.putc('C');
						opcode = decoder.asNumber();
						protocol_state = DATA;
					} else {
						//pc.putc('C');
						protocol_state = MESSAGE;
					}
					break;
				case DATA:
					if (token == EmBdecode::T_STRING) {
						//pc.putc('D');
						data = decoder.asString(&data_length);
						protocol_state = END;
					} else if (token == EmBdecode::T_POP) { //no data section
						if (tpid<MAX_PIDS
						&& tpid<MCC::n_opcode_callbacks
						&& opcode<MCC::opcode_callbacks_count[tpid]) {
							MCC::opcode_callbacks[tpid][opcode](tpid, opcode, NULL, 0);
						}
						protocol_state = MESSAGE;
						/*if (tpid==0) {
							if (opcode==16) { //ping, no params
								send_message(0, 16, NULL, 0);
							}
						}*/
					} else {
						//pc.putc('D');
						protocol_state = MESSAGE;
					}
					break;
				case END:
					if (token == EmBdecode::T_POP) {
						if (tpid<MAX_PIDS
						&& tpid<MCC::n_opcode_callbacks
						&& opcode<MCC::opcode_callbacks_count[tpid]) {
							MCC::opcode_callbacks[tpid][opcode](tpid, opcode, data, data_length);
						}
						protocol_state = MESSAGE;
						//pc.putc('E');
						/*if (tpid==0) {
							if (opcode==16) { //ping
								send_message(0, 16, data, data_length);
							}
						} else if (tpid==2) {
							ticker_task.process(opcode, data, data_length);
						}*/
					} else {
						//pc.putc('E');
						protocol_state = MESSAGE;
					}
					break;
				}
			}
			decoder.reset();
		}
	}
}
