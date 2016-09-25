#!/bin/bash
gcc main_test.c -c -o main_test.o
gcc PMD_Uart.c -c -o PMD_Uart.o
g++ stm32loaderCWrapper.cpp -c -o stm32loaderCWrapper.o
g++ nRF51ComPort.cpp -c -o nRF51ComPort.o
g++ stm32loader.cpp -c -o stm32loader.o

gcc nRF51ComPort.o stm32loader.o stm32loaderCWrapper.o PMD_Uart.o main_test.o -lstdc++ -o hallo_welt

./hallo_welt
