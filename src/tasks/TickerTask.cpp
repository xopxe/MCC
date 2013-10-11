#include <mbed.h>
#include "TickerTask.h"

extern Ticker flipper;
extern MCC mcc;
int ticker_id=-1;

DigitalOut led1(LED1);
DigitalOut led2(LED2);

int i=0;
void tick2() {
	i++;
	if (i>1000000) {
		i=0;
	    led1 = !led1;
		mcc.send_message(1, OPCODE_TICKTOGGLE, NULL, 0);
	}
}

void tick() {
    led2 = !led2;
	mcc.send_message(1, OPCODE_TICKTOGGLE, NULL, 0);
}

static void handle_report(int pid, uint8_t opcode, char *data, uint8_t data_length) {
	//TODO
}
void handle_tickertoggle(int pid, uint8_t opcode, char * data, uint8_t data_length) {
	/*if (opcode==0) {
	    flipper.attach(NULL, 1.0);
	} else {
	    flipper.attach(&tick, 1.0);
	}*/
	if (ticker_id < 0) {
		//ticker_id = mcc.register_poll_callback(tick2);
		ticker_id=1; flipper.attach(&tick, 1.0);
	} else {
		//mcc.unregister_poll_callback(ticker_id);
		//ticker_id = -1;
		ticker_id = -1; flipper.detach();
	}
}


TickerTask::TickerTask() {
    //flipper.attach(&tick, 1.0); // the address of the function to be attached (flip) and the interval (2 seconds)
	for (int i=0; i<TICKER_OPCODES; ++i) {
		TickerTask::opcode_callbacks[i]=NULL;
	}
	TickerTask::opcode_callbacks[OPCODE_REPORT] = &handle_report;
	TickerTask::opcode_callbacks[OPCODE_TICKTOGGLE] = &handle_tickertoggle;
	TickerTask::pid = mcc.register_opcode_callbacks(TickerTask::opcode_callbacks, TICKER_OPCODES);

	//mcc.register_poll_callback(tick2);
}

