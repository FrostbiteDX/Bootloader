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
#ifdef DEBUG
    uint8_t stm32loaderCWrapper_stm32_test()
    {
        uint8_t result;
        
        result = bootloader.stm32_test();
        
        return result;
    }
#endif /* DEBUG */

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
