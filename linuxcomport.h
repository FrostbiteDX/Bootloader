#include <cstdio>
#include <cstring>
#include "termios.h"
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include <sys/types.h>
#include "communication.h"


namespace linuxComPort {

	class LinuxComPort : public Communication::COMPort {
		private:
			struct termios serPortSettings;
			uint fileDescriptor;
			size_t comPortStatus = -1;
			const speed_t BAUDRATE = B38400;
			const size_t BUFFSIZE = 64;

			size_t sendByte(char data, bool sendInverted = false);
		public:
			LinuxComPort(const char* portName);
			LinuxComPort() = delete;
			virtual ~LinuxComPort();
			LinuxComPort( const LinuxComPort &right);
			const LinuxComPort& operator=(const LinuxComPort &right);

			size_t sendCommand(Communication::Commands Command, bool sendInverted = true);
			size_t sendData(const char* buffer, const size_t length);
			size_t receiveData(char* buffer, size_t* length);
			size_t getComPortStatus();
			size_t close();
			size_t getBuffSize(){ return BUFFSIZE; };
		};
}
