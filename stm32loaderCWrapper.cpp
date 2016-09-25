#include <stdio.h>
#include "stm32loader.h"
#include "communication.h"
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
        
        printf("result : %i\n", result);
        
        if(0 != result)
        {
            return ERROR;
        }
        else
        {
            return SUCCESS;
        }
    }

//     uint8_t stm32loaderCWrapper_stm32_Read_Image()
//     {
//         uint8_t result;
//         
//         result = stm32_Read_Image(char* image, int32_t* size, int32_t address);
//         
//         if(0 != result)
//         {
//             return ERROR;
//         }
//         else
//         {
//             return SUCCESS;
//         }
//     }
//         
//     uint8_t stm32loaderCWrapper_stm32_Write_Image()
//     {
//         uint8_t result;
//         
//         result = stm32_Write_Image(char* image, int32_t size, int32_t address, void* updateprogress);
//         
//         if(0 != result)
//         {
//             return ERROR;
//         }
//         else
//         {
//             return SUCCESS;
//         }
//         
//     }
//         
//     uint8_t stm32loaderCWrapper_stm32_get_bootloader_version()
//     {
//         uint8_t result;
//         
//         result = stm32_get_bootloader_version(float* version);
//         
//         if(0 != result)
//         {
//             return ERROR;
//         }
//         else
//         {
//             return SUCCESS;
//         }
//         
//     }
//         
//     uint8_t stm32loaderCWrapper_stm32_get_chip_id()
//     {
//         uint8_t result;
//         
//         result = stm32_get_chip_id(int32_t* version);
//         
//         if(0 != result)
//         {
//             return ERROR;
//         }
//         else
//         {
//             return SUCCESS;
//         }
//         
//     }
//         
//     uint8_t stm32loaderCWrapper_stm32_send_go_command()
//     {
//         uint8_t result;
//         
//         result = stm32_send_go_command();
//         
//         if(0 != result)
//         {
//             return ERROR;
//         }
//         else
//         {
//             return SUCCESS;
//         }
//         
//     }
//         
//     uint8_t stm32loaderCWrapper_stm32_exit()
//     {
//         uint8_t result;
//         
//         result = stm32_exit();
//         
//         if(0 != result)
//         {
//             return ERROR;
//         }
//         else
//         {
//             return SUCCESS;
//         }
//         
//     }
}
