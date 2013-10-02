#ifndef __task_ticker_h
#define __task_ticker_h

class TickerTask
{
  public:
	TickerTask ();
    void process(int opcode, char * data, uint8_t data_length);
};


#endif
