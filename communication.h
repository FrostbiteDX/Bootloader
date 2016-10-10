#ifndef __BLCOMM_H__
#define __BLCOMM_H__

#include <cstdio>
#include <cstring>
#include <cstdint>

namespace Communication
{
class COMPort

/**  Class Communication is Base Class for every COMPort used in various Environments, e.g. Linux, BLE-Chip...
  *	 Contains most basic functions for sending and receiving Data and is used inside Bootloader for standardized access.  
  *   
  *  @author Alexander Strobl
  *  @date 30.09.16 
  */
{
public:
    virtual uint8_t sendData(const uint8_t* buffer, const uint32_t length) = 0;
    virtual uint8_t receiveData(uint8_t* buffer, uint32_t* length) = 0;
    virtual uint8_t getComPortStatus() = 0;
    virtual uint8_t close() = 0;
    virtual uint16_t getBuffSize() = 0;
};
}

#endif
