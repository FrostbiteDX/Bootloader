/* Copyright (C) 2016 Daniel Tatzel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef __NRF51COMPORT_H__
#define __NRF51COMPORT_H__

// #include <cstdio>
// #include <cstring>
// #include <unistd.h> // UNIX standard function definitions
// #include <fcntl.h> // File control definitions
// #include <stdint.h>
#include <cstdint>
#include <errno.h> // Error number definitions
#include "communication.h"

extern "C"
{
#include "PMD_Uart.h"
}


namespace nRF51ComPort
{
    class NRF51ComPort :
        public Communication::COMPort
    {
    private:
        uint32_t comPortStatus = 0xFFFFFFFF;
        uint8_t bufferSize = 128;

    public:
        NRF51ComPort();
        const NRF51ComPort& operator=(const NRF51ComPort& right);

        virtual uint8_t sendData(const uint8_t* buffer, const uint32_t length);
        virtual uint8_t receiveData(uint8_t* buffer, uint32_t* length);
        virtual uint8_t getComPortStatus();
        virtual uint8_t close();
        virtual uint16_t getBuffSize();
        virtual uint8_t setBuffSize(uint16_t size);
    };
}

#endif /* __NRF51COMPORT_H__ */
