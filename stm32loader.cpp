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

int stm32loader::BootLoader::stm32_init() {
	char readBuffer[comPort->getBuffSize()] = { '\0' };
	size_t len = 0;
//	int fd = -1;
//	fd = initSerialPort(portName);
//	if (fd == -1) {
//		fileDescriptorUART = -1;
//		return Communication::STM32_COMM_ERROR;
//	}
//
//	sendCommand(fd, Communication::STM32_CMD_INIT, false);

//	comPort->sendCommand(Communication::STM32_CMD_INIT, false);
//	comPort->receiveData(readBuffer, &len);

//	WaitForAnswer(fd);
//	read(fd, &readBuffer, BUFFSIZE - 1);
//
//	fileDescriptorUART = fd;
//	int res = readBuffer[0];

//	if (!isAcknowdledge(res)) {
//		return Communication::STM32_COMM_ERROR;
//	}
//
//	return STM32_COMM_ACK == res || res == STM32_COMM_NACK ?
//			Communication::STM32_OK : Communication::STM32_INIT_ERROR;
}

int stm32loader::BootLoader::stm32_erase_flash(void) {
	return 1;
}

void stm32loader::BootLoader::stm32_exit() {
//	close(fileDescriptorUART);
//	comPort->close();
}

int stm32loader::BootLoader::stm32_get_commands() {
//	char readBuffer[comPort->getBuffSize()] = { '\0' };
	size_t len = 0;
//	comPort->sendCommand(Communication::STM32_CMD_GET_COMMANDS);
//	comPort->receiveData(readBuffer, &len);

//	sendCommand(fileDescriptorUART, Communication::STM32_CMD_GET_COMMANDS);
//	WaitForAnswer(fileDescriptorUART);
//	read(fileDescriptorUART, &readBuffer, BUFFSIZE - 1);

//	if (!isAcknowdledge(readBuffer[0])) {
//		return Communication::STM32_COMM_ERROR;
//	}

	return Communication::STM32_OK;
}

int stm32loader::BootLoader::stm32_get_bootloader_version(float* version) {
//	char readBuffer[comPort.getBuffSize()] = { '\0' };
//	sendCommand(fileDescriptorUART, Communication::STM32_CMD_GET_BL_VERSION);
//	WaitForAnswer(fileDescriptorUART);
//	read(fileDescriptorUART, &readBuffer, 5);
//
//	if (!isAcknowdledge(readBuffer[0]) && !isAcknowdledge(readBuffer[4])) {
//		return Communication::STM32_COMM_ERROR;
//	}
//
//	*version = (float) ((readBuffer[1] & 0xF0) >> 4);
//	float minor = readBuffer[1] & 0x0F;
//	while (minor >= 1)
//		minor /= 10;
//	*version += minor;
//
	return Communication::STM32_OK;
}
//
int stm32loader::BootLoader::stm32_get_chip_id(int32_t* version) {
//	char readBuffer[comPort.getBuffSize()] = { '\0' };
//	sendCommand(fileDescriptorUART, Communication::STM32_CMD_GET_CHIP_ID);
//	WaitForAnswer(fileDescriptorUART);
//	read(fileDescriptorUART, &readBuffer, BUFFSIZE - 1);
//
//	if (!isAcknowdledge(readBuffer[0]) && !isAcknowdledge(readBuffer[4])) {
//		return Communication::STM32_COMM_ERROR;
//	}
//
//	uint major = readBuffer[2], minor = readBuffer[3];
//	uint c = ((major & 0xF0) >> 4) * 1000;
//	c += (major & 0x0F) * 100;
//	c += ((minor & 0xF0) >> 4) * 10;
//	c += minor & 0x0F;
//	*version = c;
//
	return Communication::STM32_OK;
}

int stm32loader::BootLoader::stm32_disable_writeprotection() {
//	char readBuffer[comPort.getBuffSize()] = { '\0' };
//	sendCommand(fileDescriptorUART, Communication::STM32_CMD_WRITE_UNPROTECT);
//	WaitForAnswer(fileDescriptorUART);
//	read(fileDescriptorUART, &readBuffer, BUFFSIZE - 1);
//
//	return isAcknowdledge(readBuffer[0]) && isAcknowdledge(readBuffer[1]);
//}
//
//int stm32loader::BootLoader::stm32_send_go_command() {
//	char readBuffer[comPort.getBuffSize()] = { '\0' };
//	int32_t adress = STM32_FLASH_START_ADDRESS;
//	char B1, B2, B3, B4, checksum;
//
//	sendCommand(fileDescriptorUART, Communication::STM32_CMD_GO);
//	WaitForAnswer(fileDescriptorUART);
//	read(fileDescriptorUART, &readBuffer, BUFFSIZE - 1);
//
//	if (!isAcknowdledge(readBuffer[0])) {
//		return Communication::STM32_COMM_ERROR;
//	}
//
//	B1 = (adress >> 24) & 0xFF;
//	B2 = (adress >> 16) & 0xFF;
//	B3 = (adress >> 8) & 0xFF;
//	B4 = (adress >> 0) & 0xFF;
//	checksum = B1 ^ B2 ^ B3 ^ B4;
//	sendByte(fileDescriptorUART, B1);
//	sendByte(fileDescriptorUART, B2);
//	sendByte(fileDescriptorUART, B3);
//	sendByte(fileDescriptorUART, B4);
//	sendByte(fileDescriptorUART, checksum);
//
//	WaitForAnswer(fileDescriptorUART);
//	read(fileDescriptorUART, &readBuffer, BUFFSIZE - 1);
//
//	return isAcknowdledge(readBuffer[0]) == true ?
//			Communication::STM32_OK : Communication::STM32_COMM_ERROR;
//
}
