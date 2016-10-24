
#include "stdio.h"
#include "stm32loader.h"
#include <cstring>
#include <iostream>
#include "communication.h"
#include "linuxcomport.h"

int main(int argc, const char** argv)
{
    int result = -1;
    std::pair<uint8_t, uint8_t> version;
    uint32_t filesize;

    filesize = 128;
    uint8_t testarray[filesize];

    for (uint32_t i = 0; i < filesize; i++)
    {
    	testarray[i] = i + 1;
    }
	printf("Image: string \"%s\" \n", testarray);

    linuxComPort::LinuxComPort LinuxComPort("/dev/ttyUSB0");
    stm32loader::BootLoader* bootloader = new stm32loader::BootLoader(&LinuxComPort);

    result = bootloader->stm32_prepare_flashing();
    if(result == stm32loader::STM32_OK) {
    	uint32_t startAddress = bootloader->stm32_get_default_write_address();
    	bootloader->stm32_Write_Image(testarray, filesize, &startAddress);
    }
    else
    {
    	printf("Error preparing: %d \n", result);
    }
    bootloader->stm32_finish_flashing();
}
