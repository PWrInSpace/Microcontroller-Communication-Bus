#ifndef STRUCT_TO_RECEIVE_H
#define STRUCT_TO_RECEIVE_H

#include <stdint.h>

// Struct of data (eg. readings from sensors) sended from slave to this master (the same struct must be in slave device):

struct StructToReceive {
    
    bool ledState;
    uint8_t lastDoneCommandNum;
    float adcVoltage;
    int16_t adcValue;
};

#endif