#ifndef __BLCOMM_H__
#define __BLCOMM_H__ 1

#include <cstdio>
#include <cstring>
#include "termios.h"
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include <sys/types.h>

namespace Communication {

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

	class COMPort {
//	protected:
//		size_t comPortStatus = -1;
//		const speed_t BAUDRATE = B38400;
//		const size_t BUFFSIZE = 64;
	public:
		COMPort();
		COMPort( const COMPort &right);
		virtual ~COMPort();
		const COMPort& operator=(const COMPort &right);

		virtual size_t sendCommand(Communication::Commands Command, bool sendInverted = true);
		virtual size_t sendData(const char* buffer, const size_t length);
		virtual size_t receiveData(char* buffer, size_t* length);
//		virtual size_t getComPortStatus();
//		virtual size_t close();
		virtual size_t getBuffSize();
	};


//	class LinuxComPort : public COMPort {
//	private:
//		struct termios serPortSettings;
//		uint fileDescriptor;
//		size_t sendByte(char data, bool sendInverted = false);
//	public:
//		const LinuxComPort& operator=(const LinuxComPort &right);
//
////		LinuxComPort() = delete;
//		LinuxComPort(const char* portName);
//		size_t sendCommand(Communication::Commands Command, bool sendInverted = true);
//		size_t sendData(const char* buffer, const size_t length);
//		size_t receiveData(char* buffer, size_t* length);
//		size_t getComPortStatus();
//		size_t close();
//		size_t getBuffSize(){ return BUFFSIZE; };
//	};

//	class NRFComPort : COMPort {
//	...
//	};
//
//	class WinComPort : COMPort {
//	...
//	};
//
//	...
//
}

#endif
