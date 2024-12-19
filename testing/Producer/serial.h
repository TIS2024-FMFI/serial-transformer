#include <windows.h>

class SerialPort {

	public: 
		wchar_t * ID;
		int BAUDRATE;
		int TIMEOUT;						
		int REPEATS;

		SerialPort();
		~SerialPort();
		int init();

		int sendAndReceive(char * send, int bytesToSend, char * receive);
        int getTelegram(char * telegram);
	
	private:
		bool getChar(char * telegram);
		//int getTelegram(char * telegram);
		void sendTelegram(char * telegram, int bytesToSend);

		DWORD dwOldMask, dwMask;
		COMMTIMEOUTS timeouts;
		HANDLE hComm;
};