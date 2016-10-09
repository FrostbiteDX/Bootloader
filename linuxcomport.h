#ifndef __LINCOMPORT_H__
#define __LINCOMPORT_H__

#include <cstring>
#include <cstdint>
#include "termios.h"
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include "communication.h"

namespace linuxComPort
{
class LinuxComPort :
    public Communication::COMPort
{
private:
    struct termios serPortSettings;
    int16_t fileDescriptor = -1;
    int8_t comPortStatus = -1;
	static const constexpr int16_t BUFFSIZE = 256;
    static const constexpr speed_t BAUDRATE = B38400;

   uint8_t sendByte(uint8_t data, bool sendInverted = false);
public:
    LinuxComPort(const char* portName);
    const LinuxComPort& operator=(const LinuxComPort& right);

    virtual uint8_t sendData(const uint8_t* buffer, const uint32_t length);
    virtual uint8_t receiveData(uint8_t* buffer, uint32_t* length);
    virtual uint8_t getComPortStatus();
    virtual uint8_t close();
    virtual uint16_t getBuffSize(){ return BUFFSIZE; };
};
}

#endif
