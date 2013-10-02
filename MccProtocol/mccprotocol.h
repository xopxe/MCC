#ifndef __MccProtocol_h
#define __MccProtocol_h

class MccProtocol
{
  public:
    MccProtocol ();
    void process();
    void process_echo();
    
    void send_message(int spid, uint8_t opcode, char *data, uint8_t data_length);

    //byte nop(byte);

};

#endif //__MccProtocol_h

