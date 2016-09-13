
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
    int errorCode = 0, result = -1, chipid = 0;
    float version = -1;

    char* filename;
    char* content;
    int filesize;

    char testarray[7] = {'0', '1', '2', '3', '4', '5', '6' };
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
    printf("Bootloader Version: %f \n", version);

    bootloader->stm32_get_chip_id(&chipid);
    printf("Chip ID: %d \n", chipid);

//    result = bootloader->stm32_Write_Image(testarray, filesize, 0x08000000, NULL);


    if (result != stm32loader::STM32_OK) {
        errorCode = result;
        printf("Error: %d \n", errorCode);
    } else {
        printf("Success writing Image");

		for (int i = 0; i < filesize; i++)
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
