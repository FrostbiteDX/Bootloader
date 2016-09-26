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

#include "nRF51ComPort.h"

uint8_t nRF51ComPort::NRF51ComPort::sendData(const uint8_t* buffer, const uint32_t length)
{
    uint32_t errCode = uartTransfer(buffer, length);
    
    printf("nrf51 sendData\n");
    
    return errCode;
    
    if(0 == errCode)
    {
        return true;
    }
    else
    {
        return false;
    }
}

uint8_t nRF51ComPort::NRF51ComPort::receiveData(uint8_t* buffer, uint32_t* length)
{
    uint32_t errCode = uartReceive(buffer, *length);
    
    printf("nrf51 recData\n");
    
    if(0 == errCode)
    {
        return true;
    }
    else
    {
        return false;
    }
}

uint8_t nRF51ComPort::NRF51ComPort::getComPortStatus()
{
    if(0 == comPortStatus)
    {
        return true;
    }
    else
    {
        return false;
    }
}

uint8_t nRF51ComPort::NRF51ComPort::close()
{
    uint32_t errCode = 0;
//     = app_uart_close();
    
    if(0 == errCode)
    {
        return true;
    }
    else
    {
        return false;
    }
}

uint16_t nRF51ComPort::NRF51ComPort::getBuffSize()
{
    return bufferSize;
}

uint8_t nRF51ComPort::NRF51ComPort::setBuffSize(uint16_t size)
{
    bufferSize = size;
    
    return true;
}

nRF51ComPort::NRF51ComPort::NRF51ComPort()
{
    comPortStatus = initUart();
}
