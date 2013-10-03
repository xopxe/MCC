#include <mbed.h>
#include "MccProtocol/mccprotocol.h"
#include "TickerTask.h"

MccProtocol mccprotocol;

extern Ticker flipper;

DigitalOut led2(LED2);

void tick() {
    led2 = !led2;
	mccprotocol.send_message(2, 3, NULL, 0);
}

TickerTask::TickerTask() {
    //flipper.attach(&tick, 1.0); // the address of the function to be attached (flip) and the interval (2 seconds)
}

void TickerTask::process(int opcode, char * data, uint8_t data_length) {
	if (opcode==0) {
	    flipper.attach(NULL, 1.0);
	} else {
	    flipper.attach(&tick, 1.0);
	}
}

