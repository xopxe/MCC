#include <EmBencode.h>
#include "mccprotocol.h"

#include "tasks/TickerTask.h"
TickerTask ticker_task;

#include "mbed.h"

Serial pc(USBTX, USBRX);

enum ProtocolStates {
	MESSAGE, TPID, OPCODE, DATA, END
};

enum ProtocolStates protocol_state = MESSAGE;

char embuf[200];
EmBdecode decoder(embuf, sizeof embuf);
EmBencode encoder;

int tpid;
char opcode;
char *data;
uint8_t data_length;

void EmBencode::PushChar(char ch) {
	//Serial.write(ch);
	pc.putc(ch);
}

MccProtocol::MccProtocol() {
	//pc.baud(9600);
}

void MccProtocol::process_echo() {
	if (pc.readable()) {
		pc.putc(pc.getc());
	}
}

void MccProtocol::send_message(int spid, uint8_t opcode, char *data, uint8_t data_length) {
	encoder.startList();
	encoder.push(spid);
	encoder.push(opcode);
	if (data!=NULL) {
		encoder.push(data, data_length);
	}
	encoder.endList();
}

void MccProtocol::process() {
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
						//TODO err
					}
					break;
				case TPID:
					if (token == EmBdecode::T_NUMBER) {
						//pc.putc('B');
						tpid = decoder.asNumber();
						protocol_state = OPCODE;
					} else {
						//TODO err
					}
					break;
				case OPCODE:
					if (token == EmBdecode::T_NUMBER) {
						//pc.putc('C');
						opcode = decoder.asNumber();
						protocol_state = DATA;
					} else {
						//TODO err
					}
					break;
				case DATA:
					if (token == EmBdecode::T_STRING) {
						//pc.putc('D');
						data = decoder.asString(&data_length);
						protocol_state = END;
					} else if (token == EmBdecode::T_POP) {
						if (tpid==0) {
							if (opcode==16) { //ping, no params
								send_message(0, 16, NULL, 0);
							}
						}
					} else {
						//TODO err
					}
					break;
				case END:
					if (token == EmBdecode::T_POP) {
						//pc.putc('E');
						if (tpid==0) {
							if (opcode==16) { //ping
								send_message(0, 16, data, data_length);
							}
						} else if (tpid==2) {
							ticker_task.process(opcode, data, data_length);
						}
					} else {
						//TODO err
					}
					break;
				}
			}
			decoder.reset();
		}
	}
}

