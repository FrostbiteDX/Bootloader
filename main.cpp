
#include "stdio.h"
#include "stm32loader.h"
#include <cstring>
#include <iostream>
#include "communication.h"
#include "linuxcomport.h"

int main(int argc, const char** argv)
{
    int errorCode = 0, result = -1, chipid = 0;
    float version = -1;

    printf("params: <serialport> <hexfile> \n");

    linuxComPort::LinuxComPort LinuxComPort("/dev/ttyUSB0");
    stm32loader::BootLoader* bootloader = new stm32loader::BootLoader(&LinuxComPort);
    result = bootloader->stm32_init();

    if ((result == stm32loader::STM32_INIT_ERROR) || (result == stm32loader::STM32_COMM_ERROR)) {
        printf("error at init: %d \n", result);
    }

    bootloader->stm32_get_bootloader_version(&version);
    printf("Bootloader Version: %f \n", version);

    bootloader->stm32_get_chip_id(&chipid);
    printf("Chip ID: %d \n", chipid);

    errorCode = bootloader->stm32_send_go_command();
    printf("result go: %x \n", errorCode);

    if (result == stm32loader::STM32_INIT_ERROR) {
        errorCode = errno;
        printf("Init error: %d \n", errorCode);
    } else {
        bootloader->stm32_exit();
    }
}
