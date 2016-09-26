#ifndef __stm32loaderCWrapper_H__
#define __stm32loaderCWrapper_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
    
#ifdef DEBUG
    uint8_t stm32loaderCWrapper_stm32_test();  
#endif /* DEBUG */

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
