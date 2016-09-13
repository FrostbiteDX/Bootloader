#include "stm32loader.h"

char stm32loader::BootLoader::getACKByte()
{
    return STM32_COMM_ACK;
}

char stm32loader::BootLoader::getNACKByte()
{
    return STM32_COMM_NACK;
}

bool stm32loader::BootLoader::isAcknowdledge(int res)
{
    bool ret = false;
    if (res == getACKByte()) {
        ret = true;
    }
    return ret;
}

int stm32loader::BootLoader::sendCommand(Commands Command, bool sendInverted)
{
    char commandArray[] = {(char)Command, (char)(~Command)};
    if (sendInverted) {
        return comPort->sendData(commandArray, 2) == 2;
    } else {
        return comPort->sendData(commandArray, 1) == 1;
    }
}

int stm32loader::BootLoader::sendAddress(int32_t address)
{
    char data[5] = {'\0' };

    data[0] = (address >> 24) & 0xFF;
    data[1] = (address >> 16) & 0xFF;
    data[2] = (address >> 8) & 0xFF;
    data[3] = (address >> 0) & 0xFF;
    data[4] = data[0] ^ data[1] ^ data[2] ^ data[3]; // Checksum
    return comPort->sendData(data, 5) == 5;
}

int stm32loader::BootLoader::stm32_init()
{
    char readBuffer[comPort->getBuffSize()] = { '\0' };
    size_t len = comPort->getBuffSize();

    sendCommand(stm32loader::STM32_CMD_INIT, false);
    comPort->receiveData(readBuffer, &len);

    int res = readBuffer[0];

    if (!isAcknowdledge(res)) {
        return STM32_COMM_ERROR;
    }

    return STM32_COMM_ACK == res || res == STM32_COMM_NACK ?
    		STM32_OK : STM32_INIT_ERROR;
}

int stm32loader::BootLoader::stm32_erase_flash(void)
{
    size_t buffsize = comPort->getBuffSize();
    char readBuffer[buffsize] = { '\0' };
    char tries = 2;
    bool result = false;

    sendCommand(STM32_CMD_ERASE_FLASH);
    comPort->receiveData(readBuffer, &buffsize);

    if (isAcknowdledge(readBuffer[0])) {
        readBuffer[0] = '\0';
        for (int i = 0; i < tries; i++) {
            sendCommand(STM32_PARAM_COMPLETE_ERASE);
            comPort->receiveData(readBuffer, &buffsize);

            if (isAcknowdledge(readBuffer[0])) {
                i = tries;
                result = true;
                break;
            }
            readBuffer[0] = '\0';
        }
    }

    return result == true ? STM32_OK : STM32_COMM_ERROR;
}

void stm32loader::BootLoader::stm32_exit()
{
    comPort->close();
}

int stm32loader::BootLoader::stm32_get_commands()
{
    size_t buffsize = comPort->getBuffSize();
    char readBuffer[buffsize] = { '\0' };
    sendCommand(STM32_CMD_GET_COMMANDS);
    comPort->receiveData(readBuffer, &buffsize);

    if (!isAcknowdledge(readBuffer[0])) {
        return STM32_COMM_ERROR;
    }

    return STM32_OK;
}

int stm32loader::BootLoader::stm32_get_bootloader_version(float* version)
{
    size_t buffsize = comPort->getBuffSize();
    char readBuffer[buffsize] = { '\0' };

    sendCommand(STM32_CMD_GET_BL_VERSION);
    comPort->receiveData(readBuffer, &buffsize);

    if (!isAcknowdledge(readBuffer[0]) && !isAcknowdledge(readBuffer[4])) {
        return STM32_COMM_ERROR;
    }

    *version = (float)((readBuffer[1] & 0xF0) >> 4);
    float minor = readBuffer[1] & 0x0F;
    while (minor >= 1) {
        minor /= 10;
    }
    *version += minor;

    return STM32_OK;
}

int stm32loader::BootLoader::stm32_get_chip_id(int32_t* version)
{
    size_t buffsize = comPort->getBuffSize();
    char readBuffer[buffsize] = { '\0' };
    sendCommand(STM32_CMD_GET_CHIP_ID);

    comPort->receiveData(readBuffer, &buffsize);

    if (!isAcknowdledge(readBuffer[0]) && !isAcknowdledge(readBuffer[4])) {
        return STM32_COMM_ERROR;
    }

    uint major = readBuffer[2], minor = readBuffer[3];
    uint c = ((major & 0xF0) >> 4) * 1000;
    c += (major & 0x0F) * 100;
    c += ((minor & 0xF0) >> 4) * 10;
    c += minor & 0x0F;
    *version = c;

    return STM32_OK;
}

int stm32loader::BootLoader::stm32_disable_writeprotection()
{
    size_t buffsize = comPort->getBuffSize();
    char readBuffer[buffsize] = { '\0' };
    sendCommand(STM32_CMD_WRITE_UNPROTECT);

    comPort->receiveData(readBuffer, &buffsize);

    return isAcknowdledge(readBuffer[0]) && isAcknowdledge(readBuffer[1]);
}

int stm32loader::BootLoader::stm32_send_go_command()
{
    size_t buffsize = comPort->getBuffSize();
    char readBuffer[buffsize] = { '\0' };
    int32_t adress = STM32_FLASH_START_ADDRESS;

    sendCommand(STM32_CMD_GO);
    comPort->receiveData(readBuffer, &buffsize);

    if (!isAcknowdledge(readBuffer[0])) {
        return STM32_COMM_ERROR;
    }
    readBuffer[0] = '\0';

    sendAddress(adress);

    comPort->receiveData(readBuffer, &buffsize);

    return isAcknowdledge(readBuffer[0]) == true ?
           STM32_OK : STM32_COMM_ERROR;
}

int stm32loader::BootLoader::stm32_Write_Image(char* image, int32_t size, int32_t address, void* updateprogress)
{
    size_t buffsize = comPort->getBuffSize();
    char readBuffer[buffsize] = { '\0' };

    unsigned long int currAddress = address;
    int16_t stepWriteSize = STM32_MAX_WRITE_SIZE;
    char checkSum = 0;

	for(int i = 0; i < size; i += stepWriteSize)
	{
		if ( (size - i) < stepWriteSize)			// If image size is not an exact multiple of maxWriteSize
		{
			stepWriteSize = size - i;
		}

		// Send Write Memory Command - prepare programming
		sendCommand(Commands::STM32_CMD_WRITE_FLASH);
		comPort->receiveData(readBuffer, &buffsize);
		if (!isAcknowdledge(readBuffer[0])) {
			return STM32_COMM_ERROR;
		}
	    readBuffer[0] = '\0';


	    // Send Memory Address to program
	    sendAddress(currAddress);
		comPort->receiveData(readBuffer, &buffsize);
		if (!isAcknowdledge(readBuffer[0])) {
			return STM32_COMM_ERROR;
		}
	    readBuffer[0] = '\0';


	    // Calc XOR-Checksum for N (amount of bytes) and N + 1 data bytes
	    char stm32_N = stepWriteSize - 1;
	    checkSum = stm32_N;
	    for (int j = 0; j < stepWriteSize; j++) {
	    	checkSum ^= image[j + i];			// i for offset
	    }

	    comPort->sendData(&stm32_N, 1);
	    comPort->sendData(&(image[i]), stepWriteSize);
	    comPort->sendData(&checkSum, 1);

		comPort->receiveData(readBuffer, &buffsize);
		char help = readBuffer[0];
		if (!isAcknowdledge(readBuffer[0])) {
			return STM32_COMM_ERROR;
		}
	    readBuffer[0] = '\0';
	}

	return STM32_OK;
}

//static int stm32h_send_packet_with_checksum( u8 *packet, u32 len )
//{
//  u8 chksum = 0;
//  u32 i;
//
//  for( i = 0; i < len; i ++ )
//    chksum ^= packet[ i ];
//  ser_write( stm32_ser_id, packet, len );
//  ser_write_byte( stm32_ser_id, chksum );
//  return STM32_OK;
//}


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



int stm32loader::BootLoader::stm32_Read_Image(char* image, int32_t* size, int32_t address)
{
    size_t buffsize = comPort->getBuffSize();
    char readBuffer[buffsize] = { '\0' };

	if (size <= 0)
	{
		image[0] = '\0';
		return -1;
	}

    sendCommand(Commands::STM32_CMD_READ_FLASH);
	comPort->receiveData(readBuffer, &buffsize);
	if (!isAcknowdledge(readBuffer[0])) {
		return STM32_COMM_ERROR;
	}
	readBuffer[0] = '\0';

	// Send Memory Address to read
	sendAddress(address);
	comPort->receiveData(readBuffer, &buffsize);
	if (!isAcknowdledge(readBuffer[0])) {
		return STM32_COMM_ERROR;
	}
	readBuffer[0] = '\0';

	// Send Memory size to receive
	sendCommand((stm32loader::Commands)(*size - 1));
	comPort->receiveData(readBuffer, &buffsize);
	if (!isAcknowdledge(readBuffer[0])) {
		return STM32_COMM_ERROR;
	}
	readBuffer[0] = '\0';

	// Receive data
	comPort->receiveData(readBuffer, &buffsize);

	// copy into image
	memcpy(image, readBuffer, *size);

	return STM32_OK;

}


