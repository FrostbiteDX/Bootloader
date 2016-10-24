/*
 *
 *  Created on: 27.04.2016
 *      Author: Alexander Strobl
 */
 
#include "stm32loader.h"

uint8_t stm32loader::BootLoader::getACKByte()
{
    return STM32_COMM_ACK;
}

uint8_t stm32loader::BootLoader::getNACKByte()
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

bool stm32loader::BootLoader::isSTMOK(int res)
{
    bool ret = false;
    if (res == STM32_OK) {
        ret = true;
    }
    return ret;
}

uint8_t stm32loader::BootLoader::sendCommand(Commands Command, bool sendInverted)
{
    uint8_t commandArray[] = {(uint8_t)Command, (uint8_t)(~Command)};
    if (sendInverted) {
        return comPort->sendData(commandArray, 2) == 2 ? STM32_OK : STM32_COMM_ERROR;
    } else {
        return comPort->sendData(commandArray, 1) == 1 ? STM32_OK : STM32_COMM_ERROR;
    }
}

uint8_t stm32loader::BootLoader::sendAddress(int32_t address)
{
    uint8_t data[5] = { '\0' };

    data[0] = (address >> 24) & 0xFF;
    data[1] = (address >> 16) & 0xFF;
    data[2] = (address >> 8) & 0xFF;
    data[3] = (address >> 0) & 0xFF;
    data[4] = data[0] ^ data[1] ^ data[2] ^ data[3]; // Checksum
    return comPort->sendData(data, 5) == 5 ? STM32_OK : STM32_COMM_ERROR;
}

uint8_t stm32loader::BootLoader::stm32_init()
{
    uint8_t readBuffer[comPort->getBuffSize()] = { '\0' };
    uint32_t len = comPort->getBuffSize();

    sendCommand(stm32loader::STM32_CMD_INIT, false);
    comPort->receiveData(readBuffer, &len);

    int res = readBuffer[0];

    if (!isAcknowdledge(res)) {
        return STM32_COMM_ERROR;
    }

    return STM32_COMM_ACK == res || res == STM32_COMM_NACK ?
    		STM32_OK : STM32_INIT_ERROR;
}

uint8_t stm32loader::BootLoader::stm32_erase_flash(void)
{
    uint32_t buffsize = comPort->getBuffSize();
    uint8_t readBuffer[buffsize] = { '\0' };
    uint8_t tries = 2;
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

uint8_t stm32loader::BootLoader::stm32_extended_erase_flash(void)
{
    size_t buffsize = comPort->getBuffSize();
    uint8_t readBuffer[buffsize] = { '\0' };
    uint8_t tries = 2;
    bool result = false;

    sendCommand(STM32_CMD_EXTENDED_ERASE_FLASH);
    comPort->receiveData(readBuffer, &buffsize);

    if (isAcknowdledge(readBuffer[0])) {
        readBuffer[0] = '\0';

    	//send complete erase
    	sendCommand(STM32_PARAM_COMPLETE_ERASE, false);
    	sendCommand(STM32_PARAM_COMPLETE_ERASE);

    	for (int i = 0; i < tries; i++) {
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

uint8_t stm32loader::BootLoader::stm32_get_commands()
{
    uint32_t buffsize = comPort->getBuffSize();
    uint8_t readBuffer[buffsize] = { '\0' };

    sendCommand(STM32_CMD_GET_COMMANDS);
    comPort->receiveData(readBuffer, &buffsize);

    if (!isAcknowdledge(readBuffer[0])) {
        return STM32_COMM_ERROR;
    }

    buffsize = comPort->getBuffSize();
    comPort->receiveData(readBuffer, &buffsize);

    return STM32_OK;
}

uint8_t stm32loader::BootLoader::stm32_get_bootloader_version(std::pair<uint8_t, uint8_t>* version)
{
    uint32_t buffsize = comPort->getBuffSize();
    uint8_t readBuffer[buffsize] = { '\0' };

    sendCommand(STM32_CMD_GET_BL_VERSION);
    comPort->receiveData(readBuffer, &buffsize);

    if (!isAcknowdledge(readBuffer[0]) && !isAcknowdledge(readBuffer[4])) {
        return STM32_COMM_ERROR;
    }

    version->first = (readBuffer[1] & 0xF0) >> 4;
    version->second = (readBuffer[1] & 0x0F);

    return STM32_OK;
}

uint8_t stm32loader::BootLoader::stm32_get_chip_id(std::pair<uint8_t, uint8_t>* version)
{
    uint32_t buffsize = comPort->getBuffSize();
    uint8_t readBuffer[buffsize] = { '\0' };
    sendCommand(STM32_CMD_GET_CHIP_ID);

    comPort->receiveData(readBuffer, &buffsize);

    if (!isAcknowdledge(readBuffer[0]) && !isAcknowdledge(readBuffer[4])) {
        return STM32_COMM_ERROR;
    }

    version->first = readBuffer[2];
    version->second = readBuffer[3];

    return STM32_OK;
}

uint8_t stm32loader::BootLoader::stm32_disable_writeprotection()
{
    uint32_t buffsize = comPort->getBuffSize();
    uint8_t readBuffer[buffsize] = { '\0' };

    sendCommand(STM32_CMD_WRITE_UNPROTECT);
	comPort->receiveData(readBuffer, &buffsize);

	if (!isAcknowdledge(readBuffer[0])) {
		return STM32_COMM_ERROR;
	}
    readBuffer[0] = '\0';

    comPort->receiveData(readBuffer, &buffsize);
	if (!(isAcknowdledge(readBuffer[0]) || isAcknowdledge(readBuffer[1]))) {		// depends on HW timings...
		return STM32_COMM_ERROR;
	}
    comPort->receiveData(readBuffer, &buffsize);		// wait for restart after write unprotect

	return STM32_OK;

//    comPort->receiveData(readBuffer, &buffsize);
//    return isAcknowdledge(readBuffer[0]) && isAcknowdledge(readBuffer[1]) ? STM32_OK : STM32_COMM_ERROR;
}

uint8_t stm32loader::BootLoader::stm32_send_go_command()
{
    uint32_t buffsize = comPort->getBuffSize();
    uint8_t readBuffer[buffsize] = { '\0' };
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

uint8_t stm32loader::BootLoader::stm32_Write_Image(uint8_t* image, uint32_t size, uint32_t* address, void* updateprogress)
/**  Most important function of Bootloader, image size can be abitrarily.
  *	 Counts up the written memory addresses, so just supply the returned
  *	 address for further write operations of the same image.
  *
  *  @author Alexander Strobl
  *  @date 24.10.16
  */
{
    uint32_t buffsize = comPort->getBuffSize();
    uint8_t readBuffer[buffsize] = { '\0' };

    unsigned long int currAddress = *address;
    uint16_t stepWriteSize = STM32_MAX_WRITE_SIZE;
    uint8_t checkSum = 0;

    stepWriteSize = 6;

	for(uint32_t i = 0; i < size; i += stepWriteSize, currAddress+=stepWriteSize)
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
	    uint8_t stm32_N = stepWriteSize - 1;
	    checkSum = stm32_N;
	    for (int j = 0; j < stepWriteSize; j++) {
	    	checkSum ^= image[j + i];			// i for offset
	    }

	    comPort->sendData(&stm32_N, 1);
	    comPort->sendData(&(image[i]), stepWriteSize);
	    comPort->sendData(&checkSum, 1);

		comPort->receiveData(readBuffer, &buffsize);
		if (!isAcknowdledge(readBuffer[0])) {
			return STM32_COMM_ERROR;
		}
	    readBuffer[0] = '\0';
	}

	*address = currAddress;

	return STM32_OK;
}

uint8_t stm32loader::BootLoader::stm32_Read_Image(uint8_t* image, uint32_t* size, uint32_t address)
/**  Class Communication is Base Class for every COMPort used in various Environments, e.g. Linux, BLE-Chip...
  *	 Contains most basic functions for sending and receiving Data and is used inside Bootloader for standardized access.
  *
  *  @author Alexander Strobl
  *  @date 30.09.16
  */
{
    uint32_t buffsize = comPort->getBuffSize();
    uint8_t readBuffer[buffsize] = { '\0' };

	if (*size <= 0 || *size > buffsize)
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
	buffsize = comPort->getBuffSize();

	// Send Memory Address to read
	sendAddress(address);
	comPort->receiveData(readBuffer, &buffsize);
	if (!isAcknowdledge(readBuffer[0])) {
		return STM32_COMM_ERROR;
	}
	readBuffer[0] = '\0';
	buffsize = comPort->getBuffSize();

	// Send Memory size to receive
	sendCommand((stm32loader::Commands)(*size - 1));

	comPort->receiveData(readBuffer, &buffsize);
	if (!isAcknowdledge(readBuffer[0])) {
		return STM32_COMM_ERROR;
	}
	readBuffer[0] = '\0';

	buffsize = *size;
	// Receive data
	comPort->receiveData(readBuffer, &buffsize);

	// copy into image
	memcpy(image, readBuffer, (*size + 1) * sizeof(readBuffer[0]));

	return STM32_OK;

}

uint32_t stm32loader::BootLoader::stm32_get_default_write_address()
{
	return STM32_FLASH_START_ADDRESS;
}

uint8_t stm32loader::BootLoader::stm32_prepare_flashing()
/**	This function prepares everything for the Write Memory Command, this includes, disabling writeprotection
 *  and erasing flash
 *
 * 	@author: Alexander Strobl
 * 	@date: 24.10.16
 */
{
    int result = -1;
    std::pair<uint8_t, uint8_t> version;

	result = stm32_init();

	if ((result == stm32loader::STM32_INIT_ERROR) || (result == stm32loader::STM32_COMM_ERROR)) {
		return result;
	}
	else {
		result = stm32_disable_writeprotection();
		if(!isSTMOK(result))
		{
			return STM32_DISABLE_WRITEPROTECTION_FAILED;
		}

		result = stm32_init();
		if(!isSTMOK(result))
		{
			return STM32_INIT_ERROR;
		}

		result = stm32_get_bootloader_version(&version);
		if(!isSTMOK(result))
		{
			return STM32_GET_BOOTLOADERVERSION_FAILED;
		}


		if(version.first >= 3) {
			result = stm32_extended_erase_flash();
			if(!isSTMOK(result))
			{
				return STM32_EXTENDED_ERASE_FAILED;
			}
		}
		else {
			result = stm32_erase_flash();
			if(!isSTMOK(result))
			{
				return STM32_ERASE_FAILED;
			}
		}
	}
//		uint32_t startAddress = bootloader->stm32_get_default_write_address();
//
//		result = bootloader->stm32_Write_Image(testarray, filesize, &startAddress);
//		result |= bootloader->stm32_Write_Image(testarray, filesize, &startAddress);
//
//
//		if (result != stm32loader::STM32_OK) {
//			errorCode = result;
//			printf("Error Writing Image: %d \n", errorCode);
//		} else {
//			printf("Success writing Image \n");
//
//			for (uint32_t i = 0; i < filesize; i++)
//			{
//				testarray[i] = 0;
//			}
//
//			for (int i = 0; i < 2; i++) {
//				result = bootloader->stm32_Read_Image(testarray, &filesize, bootloader->stm32_get_default_write_address() + i * filesize);
//
//				for (uint32_t j = 0; j < filesize; j++)
//				{
//					printf("Gelesen index %d: number \"%d\" \n", j + i * filesize, testarray[j + 1]);
//				}
//
//				printf("next read \n");
//			}
//
//			errorCode = bootloader->stm32_send_go_command();
//			printf("result go: %x \n", errorCode);
//		}
//	}
//
//	bootloader->stm32_exit();

	return STM32_OK;
}

uint8_t stm32loader::BootLoader::stm32_finish_flashing()
/**	This function ends the process of flashing by sending Go command and disabling the COMPort.
 *
 * 	@author: Alexander Strobl
 * 	@date: 24.10.16
 */
{
	uint8_t res = stm32_send_go_command();
	stm32_exit();

	return res;
}
