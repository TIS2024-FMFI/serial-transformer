#include "pch.h"
#include <stdio.h>
#include "serial.h"
#include "timer.h"
#include "protocol.h"

//using namespace std;

SerialPort::SerialPort() {
	ID=L"COM3";
	BAUDRATE=9600;
	TIMEOUT=30;
	REPEATS=5;
}

SerialPort::~SerialPort() {
	SetCommMask(hComm, dwOldMask);
	FlushFileBuffers(hComm);
	CloseHandle(hComm);
  hComm = NULL;
}

int SerialPort::init() {
	hComm = CreateFile(ID, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hComm == INVALID_HANDLE_VALUE) {
		return GetLastError();		
	}
	DCB dcb;
  dcb.DCBlength = sizeof(DCB);
	GetCommState(hComm, &dcb);
	dcb.BaudRate = BAUDRATE;				//baud rate
	dcb.Parity = NOPARITY;					//parity
	dcb.ByteSize = 8;								//data bits
	dcb.StopBits = ONESTOPBIT;			//stopbits
	dcb.fDsrSensitivity = FALSE;
	dcb.fOutxCtsFlow = FALSE;
  dcb.fOutxDsrFlow = FALSE;
  dcb.fOutX = FALSE;
  dcb.fInX = FALSE;
	SetCommState(hComm, &dcb);

	GetCommMask(hComm, &dwOldMask);
	SetCommMask(hComm, EV_TXEMPTY);
	
	GetCommTimeouts(hComm, &timeouts);
     // Specify time-out between charactor for receiving.
	timeouts.ReadIntervalTimeout = MAXDWORD;  
    // Specify value that is multiplied
   // by the requested number of bytes to be read.
   timeouts.ReadTotalTimeoutMultiplier = 0;
   // Specify value is added to the product of the 
   // ReadTotalTimeoutMultiplier member

  timeouts.ReadTotalTimeoutConstant = TIMEOUT;
  SetCommTimeouts(hComm, &timeouts);
  return 0;
}

bool SerialPort::getChar(char * telegram) {
	FILE* f;
	fopen_s(&f, "commlog.txt", "a+");
	DWORD dwBytesRead = 0;
	if (!ReadFile(hComm, telegram, 1, &dwBytesRead, NULL))
		fprintf(f, "reading character failed...\n");
	if (dwBytesRead)
	{
		char c = *telegram;
		int cint = (c > 32) ? c : '.';
		fprintf(f, "%d(%c) ", c, cint);
		fclose(f);
		return(TRUE);
	}
	else
	{
		fclose(f);
		return(FALSE);
	}
}

int SerialPort::getTelegram(char * telegram) {
	int offset = 0;
   	EscapeCommFunction(hComm, SETDTR);
	while(getChar(telegram+offset)) {
		if (*(telegram+offset)==Protocol::END_BYTE) {
			return(offset+1);
		}
		else {
			offset++;
		}
	}
	PurgeComm(hComm, PURGE_RXABORT);
	return(0);
}

void SerialPort::sendTelegram(char * telegram, int bytesTosend) {
 //DWORD dwWritten;

  	EscapeCommFunction(hComm, CLRDTR);
	FILE* f;
	fopen_s(&f, "commlog.txt", "a+");
	fprintf(f, "\n>      ");
	for(int i=0; i<bytesTosend;i++) {
		char c = telegram[i];
		int cint = (c > 32) ? c : '.';
		fprintf(f, "%d(%c) ", c, cint);		
		TransmitCommChar(hComm, telegram[i]);
 //   WriteFile(hComm,telegram,bytesTosend,&dwWritten,NULL);

		//WaitCommEvent(hComm, &dwMask, NULL); 
	}
	fprintf(f, "\n< ");
	fclose(f);
}

int SerialPort::sendAndReceive(char * send, int bytesToSend, char * receive) {
	int bytesReceived = 0;
	for(int i=0;i<REPEATS;i++) {
		sendTelegram(send, bytesToSend);
	   	timer.wait(1);
		if ((bytesReceived = getTelegram(receive))>0 && Protocol::isValidAnswer(Protocol::getAddress(send),receive))
			return(bytesReceived);
	}
	*receive = Protocol::END_BYTE;
	return(1);
} 
