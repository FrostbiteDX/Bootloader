#ifndef __BLCOMM_H__
#define __BLCOMM_H__

#include <cstdio>
#include <cstring>
#include <sys/types.h>

namespace Communication
{
class COMPort
{
public:
    virtual int sendData(const char* buffer, const size_t length) = 0;
    virtual int receiveData(char* buffer, size_t* length) = 0;
    virtual int getComPortStatus() = 0;
    virtual int close() = 0;
    virtual int getBuffSize() = 0;
};
}

#endif
