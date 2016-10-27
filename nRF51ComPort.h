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
    /**  Class implements the nRF51 com port
    *	 This class provides a API for the nRF51 UART module.
    *
    *  @author Daniel Tatzel
    *  @date 14.09.16
    */
    {
    private:
        //! Current com port status.
        uint32_t comPortStatus = 0xFFFFFFFF;
        //! Current set buffer size.
        uint8_t bufferSize = 128;

    public:
        //! nRF51 com port constructor which initialises the UART module.
        NRF51ComPort();
        const NRF51ComPort& operator=(const NRF51ComPort& right);

        //! Send data function.
        /*!
        \param buffer Buffer to be sent.
        \param length Length of the buffer.
        \return Error code (true or false).
        */
        virtual uint8_t sendData(const uint8_t* buffer, const uint32_t length);
        
        //! Receive data function.
        /*!
        \param buffer Buffer for received data.
        \param length Length of the buffer.
        \return Error code (true or false).
        */
        virtual uint8_t receiveData(uint8_t* buffer, uint32_t* length);
        
        //! Get the current status of the com port.
        virtual uint8_t getComPortStatus();
        
        //! Close the com port.
        virtual uint8_t close();
        
        //! Get the current set size of the buffer.
        virtual uint16_t getBuffSize();
        
        //! Set a new buffer size.
        /*!
        \param size New buffer size.
        \return Error code (true or false).
        */
        virtual uint8_t setBuffSize(uint16_t size);
    };
}

#endif /* __NRF51COMPORT_H__ */
