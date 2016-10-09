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

#include "stm32loaderCWrapper.h"
#include "communication.h"
#include "stm32loader.h"
#include "nRF51ComPort.h"

#define SUCCESS ((uint8_t) 1)
#define ERROR ((uint8_t) 0)

nRF51ComPort::NRF51ComPort NRF51ComPort;
stm32loader::BootLoader bootloader(&NRF51ComPort);
 
extern "C"
{
    uint8_t stm32loaderCWrapper_stm32_init()
    {
        uint8_t result;
        
        result = bootloader.stm32_init();
        
        if(0 != result)
        {
            return ERROR;
        }
        else
        {
            return SUCCESS;
        }
    }

    uint8_t stm32loaderCWrapper_stm32_Read_Image(uint8_t* image, uint32_t* size, uint32_t address)
    {
        uint8_t result;
        
        result = bootloader.stm32_Read_Image(image, size, address);
        
        if(0 != result)
        {
            return ERROR;
        }
        else
        {
            return SUCCESS;
        }
    }
        
    uint8_t stm32loaderCWrapper_stm32_Write_Image(uint8_t* image, uint32_t size, uint32_t address, void* updateprogress)
    {
        uint8_t result;
        
        result = bootloader.stm32_Write_Image(image, size, address, updateprogress);
        
        if(0 != result)
        {
            return ERROR;
        }
        else
        {
            return SUCCESS;
        }
    }
        
    uint8_t stm32loaderCWrapper_stm32_get_bootloader_version(uint8_t* version)
    {
        uint8_t result;
        std::pair<uint8_t, uint8_t> tmpVersion;
        
        result = bootloader.stm32_get_bootloader_version(&tmpVersion);
        
        *version = (tmpVersion.first & 0x0F) << 4;
        *version = *version | (tmpVersion.second & 0x0F);
        
        if(0 != result)
        {
            return ERROR;
        }
        else
        {
            return SUCCESS;
        }
    }
        
    uint8_t stm32loaderCWrapper_stm32_get_chip_id(uint16_t* version)
    {
        uint8_t result;
        std::pair<uint8_t, uint8_t> tmpVersion;
        
        result = bootloader.stm32_get_chip_id(&tmpVersion);

        *version = tmpVersion.first;
        *version = *version << 8;
        *version = *version | (tmpVersion.second & 0x00FF);
    
        if(0 != result)
        {
            return ERROR;
        }
        else
        {
            return SUCCESS;
        }
    }
        
    uint8_t stm32loaderCWrapper_stm32_send_go_command()
    {
        uint8_t result;
        
        result = bootloader.stm32_send_go_command();
        
        if(0 != result)
        {
            return ERROR;
        }
        else
        {
            return SUCCESS;
        }
    }
        
    void stm32loaderCWrapper_stm32_exit()
    {   
        bootloader.stm32_exit();
    }
}
