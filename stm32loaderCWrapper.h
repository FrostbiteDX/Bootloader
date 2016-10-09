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

    uint8_t stm32loaderCWrapper_stm32_init();
    uint8_t stm32loaderCWrapper_stm32_Read_Image(uint8_t* image, uint32_t* size, uint32_t address);
    uint8_t stm32loaderCWrapper_stm32_Write_Image(uint8_t* image, uint32_t size, uint32_t address, void* updateprogress);
    uint8_t stm32loaderCWrapper_stm32_get_bootloader_version(uint8_t* version);
    uint8_t stm32loaderCWrapper_stm32_get_chip_id(uint16_t* version);
    uint8_t stm32loaderCWrapper_stm32_send_go_command();
    void stm32loaderCWrapper_stm32_exit();
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __stm32loaderCWrapper_H__ */
