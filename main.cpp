
#include "stdio.h"
#include "stm32loader.h"
#include <cstring>
#include <iostream>
#include "communication.h"
#include "linuxcomport.h"
#include <fstream>

//int loadfile(char* path, char* content, int32_t* size)
//{
//	int success = -1;
//	std::streampos begin, end;
//
//	std::ifstream imageFile(path);
//	begin = imageFile.tellg();
//	imageFile.seekg(0, std::ios::end);
//	end = imageFile.tellg();
//
//	*size = end - begin;
//
//	if(imageFile.is_open())
//	{
//		int readSize = imageFile.read(content, *size);
//		success = (readSize == *size ? -1 : 1);
//	}
//	return success;
//}

int main(int argc, const char** argv)
{
    int errorCode = 0, result = -1;
    uint32_t chipid = 0;
    std::pair<uint8_t, uint8_t> version;

//    uint8_t* filename;
//    uint8_t* content;
    uint32_t filesize;

    uint8_t testarray[7] = {'0', '1', '2', '3', '4', '5', '6' };
    filesize = 7;

    printf("params: <serialport> <hexfile> \n");

//    loadfile(filename, content, &filesize);

    linuxComPort::LinuxComPort LinuxComPort("/dev/ttyUSB0");
    stm32loader::BootLoader* bootloader = new stm32loader::BootLoader(&LinuxComPort);
    result = bootloader->stm32_init();

    if ((result == stm32loader::STM32_INIT_ERROR) || (result == stm32loader::STM32_COMM_ERROR)) {
        printf("error at init: %d \n", result);
    }

    if (result == stm32loader::STM32_INIT_ERROR) {
		errorCode = errno;
		printf("Init error: %d \n", errorCode);
	} else {
		bootloader->stm32_exit();
	}

    bootloader->stm32_get_bootloader_version(&version);
    printf("Bootloader Version: %d.%d \n", version.first, version.second);

    bootloader->stm32_get_chip_id(&version);
    printf("Chip ID: %d.%d  \n", version.first, version.second);

    result = bootloader->stm32_Write_Image(testarray, filesize, bootloader->stm32_get_default_write_address(), NULL);


    if (result != stm32loader::STM32_OK) {
        errorCode = result;
        printf("Error: %d \n", errorCode);
    } else {
        printf("Success writing Image");

		for (uint32_t i = 0; i < filesize; i++)
		{
			testarray[i] = 0;
		}

        result = bootloader->stm32_Read_Image(testarray, &filesize, 0x08000000);

        printf("Gelesen: \"%s\"", testarray);


        errorCode = bootloader->stm32_send_go_command();
        printf("result go: %x \n", errorCode);
    }

	bootloader->stm32_exit();
}
