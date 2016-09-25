#ifndef __BLCOMM_H__
#define __BLCOMM_H__

#include <cstdio>
#include <cstring>
#include <cstdint>
#include <sys/types.h>

namespace Communication
{
class COMPort
{
public:
    virtual uint8_t sendData(const uint8_t* buffer, const size_t length) = 0;
    virtual uint8_t receiveData(uint8_t* buffer, size_t* length) = 0;
    virtual uint8_t getComPortStatus() = 0;
    virtual uint8_t close() = 0;
    virtual uint16_t getBuffSize() = 0;
};
}

#endif
