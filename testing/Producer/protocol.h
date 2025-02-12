
class Protocol {

	public:
		enum {
			START_BYTE_SLAVE = 0x5a,    
            END_BYTE = 0x0d,
			MAX_TELEGRAM_LENGTH = 100,
            STRTZNM = 0x55,
            STOPZN = 0x0D
             };

        static int MakeTel(char adresa,char dlzka_dat,char* vytel, char const *data);
//zostavi telegram CPU->Slave s adresou adresa,dlzkou dat.pola dlzka_dat
//data zoberie z pola *data , pole telegramu zapise do *vytel
        static int Protocol::StrToDat(char *vst,char *vyst);
        //StrToDat, desifruje data zo vstupneho dat.pola (retazca \0x0 ignoruje
//vystup zapise do pola vyst
//vrati: 0 ak je vsetko OK
//       1 ak nesedi startovaci znak zo Slava 0x5A
//       2 ak desifruje dlzku > ako 20 bytov
//       3 ak nesedi crc
//       4 ak nezachytil 0x0d na konci
		static int getAddress(char * telegram);
		static bool isValidAnswer(int address, char * telegram);


	private:
		Protocol();
		~Protocol();
		static char lsbToAscii(int i);
		static char msbToAscii(int i);
		static int asciiToByte(char high, char low);
};

