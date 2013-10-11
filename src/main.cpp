/* Copyright 2011 Arthur Wolf

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include <mbed.h>
#include "mcc.h"

Ticker flipper;
//DigitalOut led1(LED1);
//DigitalOut led2(LED2);

MCC mcc;

#include "tasks/AdminTask.h"
AdminTask admin_task;
#include "tasks/TickerTask.h"
TickerTask ticker_task;

/*
void flip() {
    led1 = !led1;
}
*/

int main() 
{


    //led2 = 1;
    //flipper.attach(&flip, 0.5); // the address of the function to be attached (flip) and the interval (2 seconds)

    // spin in a main loop. flipper will interrupt it to call flip
	mcc.send_message(0, EVCODE_STARTED, NULL, 0);
    while(1) {
    	mcc.tick();
        //led1 = !led1;
        //wait(0.1);
    }
}

