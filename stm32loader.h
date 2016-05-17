#ifndef __STM32LD_H__
#define __STM32LD_H__

#include <cstdio>
#include <cstring>
#include "termios.h"
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include <sys/types.h>
#include "communication.h"

namespace stm32loader
{
class BootLoader
{
private:
    Communication::COMPort* comPort;

    // Communication data
#define STM32_COMM_ACK      0x79
#define STM32_COMM_NACK     0x1F
//			#define STM32_COMM_TIMEOUT  2000000
#define STM32_FLASH_START_ADDRESS 0x08000000

    int stm32_disable_writeprotection();
    int stm32_erase_flash();
    int stm32_write_flash();
    int stm32_get_commands();
    int sendCommand(Communication::Commands Command, bool sendInverted = true);

public:
    BootLoader(Communication::COMPort* serialPort) { comPort = serialPort; };
    int stm32_init();
    int stm32_Read_Image(char* image, int16_t size);
    int stm32_Write_Image(char* image, int16_t size);
    int stm32_get_bootloader_version(float* version);
    int stm32_get_chip_id(int32_t* version);
    int stm32_send_go_command();
    void stm32_exit();
};
}
#endif
