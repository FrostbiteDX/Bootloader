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

#ifndef __stm32loaderCWrapper_H__
#define __stm32loaderCWrapper_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /**
    * @brief  Wrapper for the init function of the stm32loader. 
    */
    uint8_t stm32loaderCWrapper_stm32_init();
    
    /**
    * @brief  Wrapper for the read image function of the stm32loader. 

    * @param[in] image Buffer in which the image is stored.
    * @param[in] size  Size of the image.
    * @param[in] address Address pointer for the destination of the image.
    * @retval Error code (true or false).
    */
    uint8_t stm32loaderCWrapper_stm32_Read_Image(uint8_t* image, uint32_t* size, uint32_t address);
    
    /**
    * @brief  Wrapper for the write image function of the stm32loader. 
    * @param[in] buffer Buffer to be sent.
    * @param[in] updateprogress Current progress of the update.
    * @retval Error code (true or false).
    */
    uint8_t stm32loaderCWrapper_stm32_Write_Image(uint8_t* image, uint32_t size, uint32_t address, void* updateprogress);
    
    /**
    * @brief  Wrapper function of the stm32loader for reading the current bootloader version. 

    * @param[in] version Variable in which the version shall be stored.
    * @retval Error code (true or false).
    */
    uint8_t stm32loaderCWrapper_stm32_get_bootloader_version(uint8_t* version);
    
    /**
    * @brief  Wrapper function of the stm32loader for getting the chip ID. 

    * @param[in] id Chip ID.
    * @retval Error code (true or false).
    */
    uint8_t stm32loaderCWrapper_stm32_get_chip_id(uint16_t* id);
    
    /**
    * @brief  Wrapper function for sending the go command to the target microcontroller after flashing. 

    * @retval Error code (true or false).
    */
    uint8_t stm32loaderCWrapper_stm32_send_go_command();
    
    /**
    * @brief  Wrapper function for closing the com port. 
    */
    void stm32loaderCWrapper_stm32_exit();
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __stm32loaderCWrapper_H__ */
