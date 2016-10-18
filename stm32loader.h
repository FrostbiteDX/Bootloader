#ifndef __STM32LD_H__
#define __STM32LD_H__

#include <cstdio>
#include <cstring>
#include <cstdint>
#include "termios.h"
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include <sys/types.h>
#include "communication.h"
#include <utility>

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
	STM32_INIT_ERROR = 1,
    STM32_COMM_ERROR = 2
};

class BootLoader
{
private:
    Communication::COMPort* comPort;

    // Communication data
    static constexpr uint8_t STM32_COMM_ACK = 0x79;		// 121
    static constexpr uint8_t STM32_COMM_NACK = 0x1F;	// 31
    static constexpr uint16_t STM32_MAX_WRITE_SIZE = 256;
    uint32_t STM32_FLASH_START_ADDRESS = 0x08000000;

    uint8_t stm32_get_commands();
    uint8_t stm32_write_flash();
    uint8_t sendCommand(Commands Command, bool sendInverted = true);
    uint8_t sendAddress(int32_t address);

    uint8_t getACKByte();
    uint8_t getNACKByte();
    bool isAcknowdledge(int res);

public:
    BootLoader(Communication::COMPort* serialPort) { comPort = serialPort; };
    uint8_t stm32_init();
    uint8_t stm32_Read_Image(uint8_t* image, uint32_t* size, uint32_t address);
    uint8_t stm32_Write_Image(uint8_t* image, uint32_t size, uint32_t* address, void* updateprogress);
    uint8_t stm32_get_bootloader_version(std::pair<uint8_t, uint8_t>* version);
    uint8_t stm32_get_chip_id(std::pair<uint8_t, uint8_t>* version);
    uint8_t stm32_send_go_command();
    uint32_t stm32_get_default_write_address();

    uint8_t stm32_disable_writeprotection();
    uint8_t stm32_erase_flash();
    uint8_t stm32_extended_erase_flash();

    void stm32_exit();
};
}
#endif
