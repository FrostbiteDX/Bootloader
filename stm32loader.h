#ifndef __STM32LD_H__
#define __STM32LD_H__

#include <cstdio>
#include <cstring>
#include "termios.h"
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include <sys/types.h>


namespace ComStrings {
//	enum Commands { STM32_CMD_INIT = 0, STM32_CMD_GET_COMMANDS,
//		  STM32_CMD_ERASE_FLASH, STM32_CMD_EXTENDED_ERASE_FLASH,
//		  STM32_CMD_GET_ID, STM32_CMD_WRITE_FLASH,
//		  STM32_CMD_WRITE_UNPROTECT, STM32_CMD_READ_FLASH,
//		  STM32_CMD_GO, STM32_CMD_GET_VERSION, SIZE_OF_ENUM};
//	static const char CommandNames[] = { 0x7F, 0x00, 0x43, 0x44, 0x02, 0x31, 0x73, 0x11, 0x21, 0x01 };

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
		STM32_CMD_INIT = 0x7F
	};

	enum Results
	{
	  STM32_OK = 0,
	  STM32_PORT_OPEN_ERROR,
	  STM32_COMM_ERROR,
	  STM32_INIT_ERROR,
	  STM32_TIMEOUT_ERROR,
	  STM32_NOT_INITIALIZED_ERROR
	};
}

namespace stm32loader {
	class BootLoader {
		private:
			struct termios serPortSettings;

			int fileDescriptorUART = -1;

			// Communication data
			#define STM32_COMM_ACK      0x79
			#define STM32_COMM_NACK     0x1F
			#define STM32_COMM_TIMEOUT  2000000
			#define STM32_WRITE_BUFSIZE 256
			#define STM32_FLASH_START_ADDRESS 0x08000000

			const speed_t BAUDRATE = B38400;
			const int BUFFSIZE = 64;

			int initSerialPort(char const* portName);
			int stm32_disable_writeprotection();
			int stm32_erase_flash();
			int stm32_write_flash();
			int stm32_get_commands();

		public:
			int stm32_init(const char* portName);
			int stm32_Read_Image(char* image, int16_t size);
			int stm32_Write_Image(char* image, int16_t size);
			int stm32_get_bootloader_version( float* version );
			int stm32_get_chip_id( int32_t* version );
			int stm32_send_go_command();
			void stm32_exit();
	};
}
#endif
