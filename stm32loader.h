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
enum Commands {
    STM32_CMD_GET_COMMANDS = 0x00,
    STM32_CMD_GET_BL_VERSION = 0x01,
    STM32_CMD_GET_CHIP_ID = 0x02,
    STM32_CMD_READ_FLASH = 0x11,
    STM32_CMD_GO = 0x21,
    STM32_CMD_WRITE_FLASH = 0x31,
    STM32_CMD_ERASE_FLASH = 0x43,
    STM32_CMD_EXTENDED_ERASE_FLASH = 0x44,
    STM32_CMD_WRITE_UNPROTECT = 0x73,
    STM32_CMD_INIT = 0x7F,
    STM32_PARAM_COMPLETE_ERASE = 0xFF
};

enum Results {
    STM32_OK = 0,
    STM32_PORT_OPEN_ERROR,
    STM32_COMM_ERROR,
    STM32_INIT_ERROR,
    STM32_TIMEOUT_ERROR,
    STM32_NOT_INITIALIZED_ERROR
};

class BootLoader
{
private:
    Communication::COMPort* comPort;

    // Communication data
    static constexpr char STM32_COMM_ACK = 0x79;
    static constexpr char STM32_COMM_NACK = 0x1F;
    static constexpr int16_t STM32_MAX_WRITE_SIZE = 256;
//    int32_t STM32_FLASH_START_ADDRESS = 0x08000000;
	int32_t STM32_FLASH_START_ADDRESS = 0x07000000; 	// For testing only!
    //			#define STM32_COMM_TIMEOUT  2000000

    int stm32_disable_writeprotection();
    int stm32_erase_flash();
    int stm32_write_flash();
    int stm32_get_commands();
    int sendCommand(Commands Command, bool sendInverted = true);
    int sendAddress(int32_t address);

    char getACKByte();
    char getNACKByte();
    bool isAcknowdledge(int res);

public:
    BootLoader(Communication::COMPort* serialPort) { comPort = serialPort; };
    int stm32_init();
    int stm32_Read_Image(char* image, int32_t* size);
    int stm32_Write_Image(char* image, int32_t size, int32_t address, void* updateprogress);
    int stm32_get_bootloader_version(float* version);
    int stm32_get_chip_id(int32_t* version);
    int stm32_send_go_command();
    void stm32_exit();
};
}
#endif
