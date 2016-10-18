
#include "stdio.h"
#include "stm32loader.h"
#include <cstring>
#include <iostream>
#include "communication.h"
#include "linuxcomport.h"
#include <fstream>
#include <ostream>

int main(int argc, const char** argv)
{
    int errorCode = 0, result = -1;
    std::pair<uint8_t, uint8_t> version;
    uint32_t filesize;

    filesize = 128;
    uint8_t testarray[filesize];

    for (uint32_t i = 0; i < filesize; i++)
    {
    	testarray[i] = i;
//    	testarray[i] = 65 + (i % 26);
//    	testarray[i] = 65;
    }
	printf("Image: string \"%s\" \n", testarray);

    printf("params: <serialport> <hexfile> \n");

    linuxComPort::LinuxComPort LinuxComPort("/dev/ttyUSB0");
    stm32loader::BootLoader* bootloader = new stm32loader::BootLoader(&LinuxComPort);
    result = bootloader->stm32_init();

    if ((result == stm32loader::STM32_INIT_ERROR) || (result == stm32loader::STM32_COMM_ERROR)) {
        printf("error at init: %d \n", result);
        return 0;
    }

    if (result == stm32loader::STM32_INIT_ERROR) {
		errorCode = errno;
		printf("Init error: %d \n", errorCode);
        return 0;
	}
    else {
		result = bootloader->stm32_disable_writeprotection();
		printf("Disable Writeprotection: %d \n", result);

	    result = bootloader->stm32_init();

		bootloader->stm32_get_chip_id(&version);
		printf("Chip ID: %d.%d  \n", version.first, version.second);

		bootloader->stm32_get_bootloader_version(&version);
		printf("Bootloader Version: %d.%d \n", version.first, version.second);


		if(version.first == 3) {
			result = bootloader->stm32_extended_erase_flash();
		}
		else {
			result = bootloader->stm32_erase_flash();
		}
		printf("Erase Flash: %d \n", result);

		uint32_t startAddress = bootloader->stm32_get_default_write_address();

		result = bootloader->stm32_Write_Image(testarray, filesize, &startAddress, NULL);
		result |= bootloader->stm32_Write_Image(testarray, filesize, &startAddress, NULL);


		if (result != stm32loader::STM32_OK) {
			errorCode = result;
			printf("Error Writing Image: %d \n", errorCode);
		} else {
			printf("Success writing Image \n");

			for (uint32_t i = 0; i < filesize; i++)
			{
				testarray[i] = 0;
			}

			for (int i = 0; i < 2; i++) {
				result = bootloader->stm32_Read_Image(testarray, &filesize, bootloader->stm32_get_default_write_address() + i * filesize);

				for (uint32_t j = 0; j < filesize; j++)
				{
					printf("Gelesen index %d: number \"%d\" \n", j + i * filesize, testarray[j + 1]);
				}

				printf("next read \n");
			}

			errorCode = bootloader->stm32_send_go_command();
			printf("result go: %x \n", errorCode);
		}
	}

	bootloader->stm32_exit();
}
