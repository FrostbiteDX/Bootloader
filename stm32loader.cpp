#include "stm32loader.h"

char getACKByte() {
	return STM32_COMM_ACK;
}

char getNACKByte() {
	return STM32_COMM_NACK;
}

bool isAcknowdledge(int res) {
	bool ret = false;
	if (res == getACKByte())
		ret = true;
	return ret;
}


int stm32loader::BootLoader::sendCommand(Communication::Commands Command, bool sendInverted) {
	char commandArray[] = {(char)Command, (char)(~Command)};
	if (sendInverted)
		return comPort->sendData(commandArray, 2);
	else
		return comPort->sendData(commandArray, 1);
}

int stm32loader::BootLoader::stm32_init() {
	char readBuffer[comPort->getBuffSize()] = { '\0' };
	size_t len = comPort->getBuffSize();

	sendCommand(Communication::STM32_CMD_INIT, false);
	comPort->receiveData(readBuffer, &len);

	int res = readBuffer[0];

	if (!isAcknowdledge(res)) {
		return Communication::STM32_COMM_ERROR;
	}

	return STM32_COMM_ACK == res || res == STM32_COMM_NACK ?
			Communication::STM32_OK : Communication::STM32_INIT_ERROR;
}

int stm32loader::BootLoader::stm32_erase_flash(void) {
	size_t buffsize = comPort->getBuffSize();
	char readBuffer[buffsize] = { '\0' };
	char tries = 2;
	bool result = false;


	sendCommand(Communication::STM32_CMD_ERASE_FLASH);
	comPort->receiveData(readBuffer, &buffsize);

	if(isAcknowdledge(readBuffer[0]))
	{
		readBuffer[0] = '\0';
		for(int i = 0; i < tries; i++) {
			sendCommand(Communication::STM32_PARAM_COMPLETE_ERASE);
			comPort->receiveData(readBuffer, &buffsize);

			if (isAcknowdledge(readBuffer[0])) {
				i = tries;
				result = true;
				break;
			}
		}
	}

	return result == true ? Communication::STM32_OK : Communication::STM32_COMM_ERROR;
}

void stm32loader::BootLoader::stm32_exit() {
	comPort->close();
}

int stm32loader::BootLoader::stm32_get_commands() {
	size_t buffsize = comPort->getBuffSize();
	char readBuffer[buffsize] = { '\0' };
	sendCommand(Communication::STM32_CMD_GET_COMMANDS);
	comPort->receiveData(readBuffer, &buffsize);

	if (!isAcknowdledge(readBuffer[0])) {
		return Communication::STM32_COMM_ERROR;
	}

	return Communication::STM32_OK;
}

int stm32loader::BootLoader::stm32_get_bootloader_version(float* version) {
	size_t buffsize = comPort->getBuffSize();
	char readBuffer[buffsize] = { '\0' };

	sendCommand(Communication::STM32_CMD_GET_BL_VERSION);
	comPort->receiveData(readBuffer, &buffsize);

	if (!isAcknowdledge(readBuffer[0]) && !isAcknowdledge(readBuffer[4])) {
		return Communication::STM32_COMM_ERROR;
	}

	*version = (float) ((readBuffer[1] & 0xF0) >> 4);
	float minor = readBuffer[1] & 0x0F;
	while (minor >= 1)
		minor /= 10;
	*version += minor;

	return Communication::STM32_OK;
}

int stm32loader::BootLoader::stm32_get_chip_id(int32_t* version) {
	size_t buffsize = comPort->getBuffSize();
	char readBuffer[buffsize] = { '\0' };
	sendCommand(Communication::STM32_CMD_GET_CHIP_ID);

	comPort->receiveData(readBuffer, &buffsize);

	if (!isAcknowdledge(readBuffer[0]) && !isAcknowdledge(readBuffer[4])) {
		return Communication::STM32_COMM_ERROR;
	}

	uint major = readBuffer[2], minor = readBuffer[3];
	uint c = ((major & 0xF0) >> 4) * 1000;
	c += (major & 0x0F) * 100;
	c += ((minor & 0xF0) >> 4) * 10;
	c += minor & 0x0F;
	*version = c;

	return Communication::STM32_OK;
}

int stm32loader::BootLoader::stm32_disable_writeprotection() {
	size_t buffsize = comPort->getBuffSize();
	char readBuffer[buffsize] = { '\0' };
	sendCommand(Communication::STM32_CMD_WRITE_UNPROTECT);

	comPort->receiveData(readBuffer, &buffsize);

	return isAcknowdledge(readBuffer[0]) && isAcknowdledge(readBuffer[1]);
}

int stm32loader::BootLoader::stm32_send_go_command() {
	size_t buffsize = comPort->getBuffSize();
	char readBuffer[buffsize] = { '\0' };
	char data[5];
	int32_t adress = STM32_FLASH_START_ADDRESS;

	sendCommand(Communication::STM32_CMD_GO);
	comPort->receiveData(readBuffer, &buffsize);

	if (!isAcknowdledge(readBuffer[0])) {
		return Communication::STM32_COMM_ERROR;
	}

	data[1] = (adress >> 24) & 0xFF;
	data[2] = (adress >> 16) & 0xFF;
	data[3] = (adress >> 8) & 0xFF;
	data[4] = (adress >> 0) & 0xFF;
	data[5] = data[1] ^ data[2] ^ data[3] ^ data[4]; // Checksum
	comPort->sendData(data, 5);

//	sendByte(fileDescriptorUART, B2);
//	sendByte(fileDescriptorUART, B3);
//	sendByte(fileDescriptorUART, B4);
//	sendByte(fileDescriptorUART, checksum);
//	WaitForAnswer(fileDescriptorUART);
//	read(fileDescriptorUART, &readBuffer, BUFFSIZE - 1);

	comPort->receiveData(readBuffer, &buffsize);

	return isAcknowdledge(readBuffer[0]) == true ?
			Communication::STM32_OK : Communication::STM32_COMM_ERROR;
}

int stm32loader::BootLoader::stm32_Write_Image(char* image, int16_t size){


}

//int stm32_write_flash( p_read_data read_data_func, p_progress progress_func )
//{
//  u32 wrote = 0;
//  u8 data[ STM32_WRITE_BUFSIZE + 1 ];
//  u32 datalen, address = STM32_FLASH_START_ADDRESS;
//
//  STM32_CHECK_INIT;
//  while( 1 )
//  {
//    // Read data to program
//    if( ( datalen = read_data_func( data + 1, STM32_WRITE_BUFSIZE ) ) == 0 )
//      break;
//    data[ 0 ] = ( u8 )( datalen - 1 );
//
//    // Send write request
//    stm32h_send_command( STM32_CMD_WRITE_FLASH );
//    STM32_EXPECT( STM32_COMM_ACK );
//
//    // Send address
//    stm32h_send_address( address );
//    STM32_EXPECT( STM32_COMM_ACK );
//
//    // Send data
//    stm32h_send_packet_with_checksum( data, datalen + 1 );
//    STM32_EXPECT( STM32_COMM_ACK );
//
//    // Call progress function (if provided)
//    wrote += datalen;
//    if( progress_func )
//      progress_func( wrote );
//
//    // Advance to next data
//    address += datalen;
//  }
//  return STM32_OK;
//}
