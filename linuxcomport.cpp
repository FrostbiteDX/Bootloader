/*
 * Communication.cpp
 *
 *  Created on: 27.04.2016
 *      Author: Alexander Strobl
 */

#include "linuxcomport.h"

size_t linuxComPort::LinuxComPort::sendData(const char* buffer, const size_t length) {
	//TODO
	return 1;

}

size_t linuxComPort::LinuxComPort::sendByte(char data, bool sendInverted) {
	uint count = 0;

	char c[] = { (char) data, (char) ~data };

	if (sendInverted) {
		count += write(fileDescriptor, c, 2);
	} else {
		count += write(fileDescriptor, c, 1);
	}

//	tcflush(fileDescriptor, TCOFLUSH);				// clear IO Buffer
	return count > 0;
}

size_t linuxComPort::LinuxComPort::sendCommand(Communication::Commands Command, bool sendInverted) {
	return sendByte((char) Command, sendInverted);
}

linuxComPort::LinuxComPort::LinuxComPort(const char* portName) {
	int port = open(portName, O_RDWR);
	if (port < 0) {
		printf("open serial port %s failed: error: %d", portName, errno);
		comPortStatus = port;
	}

	tcgetattr(port, &serPortSettings);
	cfsetispeed(&serPortSettings, BAUDRATE);
	cfsetospeed(&serPortSettings, BAUDRATE);

	// soll: 8, Even par, 1 Stop
	serPortSettings.c_cflag |= PARENB;    	// enable parity for in- and output
	serPortSettings.c_cflag &= ~PARODD;		// even parity
	serPortSettings.c_cflag &= ~CSTOPB;		// only 1 stop bit
	serPortSettings.c_cflag &= ~CSIZE;		// reset data bit size
	serPortSettings.c_cflag |= CS8;			// force 8 data bits

	// convert break to null byte, no CR to NL translation,
	// no NL to CR translation, don't mark parity errors or breaks
	// don't strip high bit off,
	// no XON/XOFF software flow control
	serPortSettings.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK
			| ISTRIP | IXON | IXOFF);

	serPortSettings.c_oflag &= ~OPOST;

	// echo off, echo newline off, canonical mode off,
	// extended input processing off, signal chars off
	serPortSettings.c_lflag &= ~(ECHO | ECHONL | ECHOE | ICANON | IEXTEN | ISIG);

	// One input byte is enough to return from read()
	// Inter-character timer off
	serPortSettings.c_cc[VMIN] = 0;
	serPortSettings.c_cc[VTIME] = 1;

	tcsetattr(port, TCSAFLUSH, &serPortSettings); // apply the settings to the port
	tcflush(port, TCIOFLUSH);				// clear IO Buffer

	comPortStatus = true;
	fileDescriptor = port;
}
