#ifndef __BLCOMM_H__
#define __BLCOMM_H__ 1

#include <cstdio>
#include <cstring>
#include "termios.h"
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include <sys/types.h>

namespace Communication
{
enum Commands {
    STM32_CMD_GET_COMMANDS = 0x00,
    STM32_CMD_GET_BL_VERSION = 0x01,
    STM32_CMD_GET_CHIP_ID = 0x02,
    STM32_CMD_READ_FLASH = 0x11,
    STM32_CMD_GO = 0x21,
    STM32_CMD_WRITE_FLASH = 0x31,
    STM32_CMD_ERASE_FLASH = 0x43,
    STM32_CMD_EXTENDED_ERASE_FLASH = 0x44,
    STM32_CMD_WRITE_UNPROTECT = 0x73,
    STM32_CMD_INIT = 0x7F,
    STM32_PARAM_COMPLETE_ERASE = 0xFF
};

enum Results {
    STM32_OK = 0,
    STM32_PORT_OPEN_ERROR,
    STM32_COMM_ERROR,
    STM32_INIT_ERROR,
    STM32_TIMEOUT_ERROR,
    STM32_NOT_INITIALIZED_ERROR
};

class COMPort
{
protected:
    const size_t BUFFSIZE = 256;
public:
    const COMPort& operator=(const COMPort& right);

    virtual size_t sendData(const char* buffer, const size_t length) = 0;
    virtual size_t receiveData(char* buffer, size_t* length) = 0;
    virtual size_t getComPortStatus() = 0;
    virtual size_t close() = 0;
    virtual size_t getBuffSize() = 0;
};
}

#endif
