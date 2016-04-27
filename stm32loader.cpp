#include "stm32loader.h"

char getACKByte() {
	return STM32_COMM_ACK;
}

char getNACKByte() {
	return STM32_COMM_NACK;
}

bool isAcknowdledge(uint res) {
	bool ret = false;
	if (res == getACKByte())
		ret = true;
	return ret;
}

void WaitForAnswer(int fileDescriptor) {
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 50000;
	select(fileDescriptor, &rfds, NULL, NULL, &timeout);
}

uint sendByte(int fileDescriptor, char data, bool sendInverted = false) {
	uint count = 0;

	char c[] = { (char) data, (char) ~data };

	if (sendInverted) {
		count += write(fileDescriptor, c, 2);
	} else {
		count += write(fileDescriptor, c, 1);
	}

//	tcflush(fileDescriptor, TCOFLUSH);				// clear IO Buffer
	return count > 0;
}

uint sendCommand(int fileDescriptor, ComStrings::Commands Command,
		bool sendInverted = true) {
	return sendByte(fileDescriptor, (char) Command, sendInverted);
}

int stm32loader::BootLoader::stm32_init(char const* portName) {
	char readBuffer[BUFFSIZE] = { '\0' };
	int fd = -1;
	fd = initSerialPort(portName);
	if (fd == -1) {
		fileDescriptorUART = -1;
		return ComStrings::STM32_COMM_ERROR;
	}

	sendCommand(fd, ComStrings::STM32_CMD_INIT, false);
	WaitForAnswer(fd);
	read(fd, &readBuffer, BUFFSIZE - 1);

	fileDescriptorUART = fd;
	int res = readBuffer[0];

	if (!isAcknowdledge(res)) {
		return ComStrings::STM32_COMM_ERROR;
	}

	return STM32_COMM_ACK == res || res == STM32_COMM_NACK ?
			ComStrings::STM32_OK : ComStrings::STM32_INIT_ERROR;
}

int stm32loader::BootLoader::stm32_erase_flash(void) {
	return 1;
}

int stm32loader::BootLoader::initSerialPort(const char* portName) {
	int port = open(portName, O_RDWR);

	if (port < 0) {
		printf("open serial port %s failed: error: %d", portName, errno);
		return port;
	}

	tcgetattr(port, &serPortSettings);

	cfsetispeed(&serPortSettings, BAUDRATE);
	cfsetospeed(&serPortSettings, BAUDRATE);

	// soll: 8, Even par, 1 Stop
	serPortSettings.c_cflag |= PARENB;    	// enable parity for in- and output
	serPortSettings.c_cflag &= ~PARODD;		// even parity
	serPortSettings.c_cflag &= ~CSTOPB;		// only 1 stop bit
	serPortSettings.c_cflag &= ~CSIZE;		// reset data bit size
//	serPortSettings.c_cflag &= ~(CS5 | CS6 | CS7); // deactivate other data packet sizes
	serPortSettings.c_cflag |= CS8;			// force 8 data bits

	// convert break to null byte, no CR to NL translation,
	// no NL to CR translation, don't mark parity errors or breaks
	// don't strip high bit off,
	// no XON/XOFF software flow control
	serPortSettings.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK
			| ISTRIP | IXON | IXOFF);
//
	serPortSettings.c_oflag &= ~OPOST;

	// echo off, echo newline off, canonical mode off,
	// extended input processing off, signal chars off
	serPortSettings.c_lflag &=
			~(ECHO | ECHONL | ECHOE | ICANON | IEXTEN | ISIG);

	// One input byte is enough to return from read()
	// Inter-character timer off
	serPortSettings.c_cc[VMIN] = 0;
	serPortSettings.c_cc[VTIME] = 1;

	tcsetattr(port, TCSAFLUSH, &serPortSettings); // apply the settings to the port
	tcflush(port, TCIOFLUSH);				// clear IO Buffer

	return port;
}

void stm32loader::BootLoader::stm32_exit() {
	close(fileDescriptorUART);
}

int stm32loader::BootLoader::stm32_get_commands() {
	char readBuffer[BUFFSIZE] = { '\0' };
	sendCommand(fileDescriptorUART, ComStrings::STM32_CMD_GET_COMMANDS);
	WaitForAnswer(fileDescriptorUART);
	read(fileDescriptorUART, &readBuffer, BUFFSIZE - 1);

	if (!isAcknowdledge(readBuffer[0])) {
		return ComStrings::STM32_COMM_ERROR;
	}

	return ComStrings::STM32_OK;
}

int stm32loader::BootLoader::stm32_get_bootloader_version(float* version) {
	char readBuffer[BUFFSIZE] = { '\0' };
	sendCommand(fileDescriptorUART, ComStrings::STM32_CMD_GET_BL_VERSION);
	WaitForAnswer(fileDescriptorUART);
	read(fileDescriptorUART, &readBuffer, 5);

	if (!isAcknowdledge(readBuffer[0]) && !isAcknowdledge(readBuffer[4])) {
		return ComStrings::STM32_COMM_ERROR;
	}

	*version = (float) ((readBuffer[1] & 0xF0) >> 4);
	float minor = readBuffer[1] & 0x0F;
	while (minor >= 1)
		minor /= 10;
	*version += minor;

	return ComStrings::STM32_OK;
}

int stm32loader::BootLoader::stm32_get_chip_id(int32_t* version) {
	char readBuffer[BUFFSIZE] = { '\0' };
	sendCommand(fileDescriptorUART, ComStrings::STM32_CMD_GET_CHIP_ID);
	WaitForAnswer(fileDescriptorUART);
	read(fileDescriptorUART, &readBuffer, BUFFSIZE - 1);

	if (!isAcknowdledge(readBuffer[0]) && !isAcknowdledge(readBuffer[4])) {
		return ComStrings::STM32_COMM_ERROR;
	}

	uint major = readBuffer[2], minor = readBuffer[3];
	uint c = ((major & 0xF0) >> 4) * 1000;
	c += (major & 0x0F) * 100;
	c += ((minor & 0xF0) >> 4) * 10;
	c += minor & 0x0F;
	*version = c;

	return ComStrings::STM32_OK;
}

int stm32loader::BootLoader::stm32_disable_writeprotection() {
	char readBuffer[BUFFSIZE] = { '\0' };
	sendCommand(fileDescriptorUART, ComStrings::STM32_CMD_WRITE_UNPROTECT);
	WaitForAnswer(fileDescriptorUART);
	read(fileDescriptorUART, &readBuffer, BUFFSIZE - 1);

	return isAcknowdledge(readBuffer[0]) && isAcknowdledge(readBuffer[1]);
}

int stm32loader::BootLoader::stm32_send_go_command() {
	char readBuffer[BUFFSIZE] = { '\0' };
	int32_t adress = STM32_FLASH_START_ADDRESS;
	char B1, B2, B3, B4, checksum;

	sendCommand(fileDescriptorUART, ComStrings::STM32_CMD_GO);
	WaitForAnswer(fileDescriptorUART);
	read(fileDescriptorUART, &readBuffer, BUFFSIZE - 1);

	if (!isAcknowdledge(readBuffer[0])) {
		return ComStrings::STM32_COMM_ERROR;
	}

	B1 = (adress >> 24) & 0xFF;
	B2 = (adress >> 16) & 0xFF;
	B3 = (adress >> 8) & 0xFF;
	B4 = (adress >> 0) & 0xFF;
	checksum = B1 ^ B2 ^ B3 ^ B4;
	sendByte(fileDescriptorUART, B1);
	sendByte(fileDescriptorUART, B2);
	sendByte(fileDescriptorUART, B3);
	sendByte(fileDescriptorUART, B4);
	sendByte(fileDescriptorUART, checksum);

	WaitForAnswer(fileDescriptorUART);
	read(fileDescriptorUART, &readBuffer, BUFFSIZE - 1);

	return isAcknowdledge(readBuffer[0]) == true ?
			ComStrings::STM32_OK : ComStrings::STM32_COMM_ERROR;

}
