#ifndef __LINCOMPORT_H__
#define __LINCOMPORT_H__

#include <cstdio>
#include <cstring>
#include "termios.h"
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include <sys/types.h>
#include "communication.h"

namespace linuxComPort
{
class LinuxComPort :
    public Communication::COMPort
{
private:
    struct termios serPortSettings;
    int fileDescriptor = -1;
    int comPortStatus = -1;
	static const constexpr int16_t BUFFSIZE = 256;
    static const constexpr speed_t BAUDRATE = B38400;

    int sendByte(char data, bool sendInverted = false);
public:
    LinuxComPort(const char* portName);
    const LinuxComPort& operator=(const LinuxComPort& right);

    virtual int sendData(const char* buffer, const size_t length);
    virtual int receiveData(char* buffer, size_t* length);
    virtual int getComPortStatus();
    virtual int close();
    virtual int getBuffSize(){ return BUFFSIZE; };
};
}

#endif
