#include "pch.h"
#include "protocol.h"


Protocol::Protocol() {
}


Protocol::~Protocol() {
}

int Protocol::getAddress(char * telegram) {
	return(asciiToByte(telegram[1], telegram[2]));
}

bool Protocol::isValidAnswer(int address, char * telegram) {
	
	int rlength;
	for(rlength=0; rlength<MAX_TELEGRAM_LENGTH && telegram[rlength]!=END_BYTE; rlength++) {
		;
	}
	rlength++;
	
	//ci sedi dlzka telegramu
	if(telegram[0]==START_BYTE_SLAVE && asciiToByte(telegram[3], telegram[4])==(rlength-8)/2 && telegram[rlength-1]==END_BYTE) {
		//ci sedi cislo modulu
		int addressReceived=getAddress(telegram);
		if(address==addressReceived) {
			
			//ci sedi kontrolna suma
			int crc=0;
			for(int i=0; i<rlength-3; i++) {
				crc+=telegram[i];
			}
			crc%=256;
			if(msbToAscii(crc)==telegram[rlength-3] && lsbToAscii(crc)==telegram[rlength-2]) {
				return (true);
			}
			else {
				return (false);
			}
		}
		else {
			return (false);
		}
		
	}
	else {
		return (false);
	}
}

int Protocol::asciiToByte(char high, char low) {
	high-=(char)48;
	low-=(char)48;

	if(high>9) {
		high-=(char)7;
	}
	if(low>(char)9) {
		low-=(char)7;

	}
	int result=(high << 4)+low;

	return (result);
}

char Protocol::lsbToAscii(int i) {
	char low=(char) ((i & 15)+48);
	if(low>57) {
		low+=(char)7;
	}
	return (low);
}

char Protocol::msbToAscii(int i) {
	char high=(char) (((i & 240) >> 4)+48);//unsigned right shift??
	if(high>57) {
		high+=(char)7;
	}
	return (high);
}

int Protocol::MakeTel(char adresa,char dlzka_dat,char* vytel, char const *data)
//zostavi telegram CPU->Slave s adresou adresa,dlzkou dat.pola dlzka_dat
//data zoberie z pola *data , pole telegramu zapise do *vytel
{
	char crc=0;
	int i;
	vytel[0]=STRTZNM;
	vytel[1]=msbToAscii(adresa);
	vytel[2]=lsbToAscii(adresa);
	vytel[3]=msbToAscii(dlzka_dat); //dlzka datoveho pola
	vytel[4]=lsbToAscii(dlzka_dat);
	for(i=0;i<=dlzka_dat;i++)
	{
	  vytel[5+(2*i)]=msbToAscii(data[i]);
	  vytel[6+(2*i)]=lsbToAscii(data[i]);
	}
	for(i=0;i<=(4+2*dlzka_dat);i++)
		crc+=vytel[i];
	i=(5+2*dlzka_dat);
	vytel[i]=(char)msbToAscii(crc);
	i++;
	vytel[i]=lsbToAscii(crc);
	i++;
	vytel[i]=STOPZN;
	i++;
	return(i);
}



//StrToDat, desifruje data zo vstupneho dat.pola (retazca \0x0 ignoruje
//vystup zapise do pola vyst
//vrati: 0 ak je vsetko OK
//       1 ak nesedi startovaci znak zo Slava 0x5A
//       2 ak desifruje dlzku > ako 20 bytov
//       3 ak nesedi crc
//       4 ak nezachytil 0x0d na konci
int Protocol::StrToDat(char *vst,char *vyst)
{
	char m,l,crc=0;
	char CC;
	int ii,io;
	m=vst[0];
	crc+=m;
	if(m!=START_BYTE_SLAVE)
		 return 1;
	m=vst[1];
	crc+=m;
	l=vst[2];
	crc+=l;
	vyst[0]=(char)asciiToByte(m,l);
	m=vst[3];
	crc+=m;
	l=vst[4];
	crc+=l;
	vyst[1]=(char)asciiToByte(m,l);
	if(vyst[1] > 20)
		  return 1;
	for(ii=5,io=2;io<=(vyst[1]+1);io++)
		{
		   m=vst[ii];
		   crc+=m;
		   ii++;
		   l=vst[ii];
		   crc+=l;
		   ii++;
		   vyst[io]=(char)asciiToByte(m,l);
		}
	m=vst[ii];
	ii++;
	l=vst[ii];
	CC=(char)asciiToByte(m,l);
	if(crc!=CC)
		 return 3;
	ii++;
	  m=vst[ii];
	if(m!=0x0D)
		 return 4;
	return 0;       //telegram bol OK
}