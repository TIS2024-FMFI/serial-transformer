#include "pch.h"
#include "ProducerDlg.h"
#include "Unit1.h"
#include "protocol.h"

Unit1::Unit1(CProducerDlg *dlg)
{
	dialog = dlg;
	StlaceneVysli = false;
	if (commport.init() == 0)
		portOpen = 1;
	else portOpen = 0;
	TypTel = 0;
	PocetPrijatych = 0;
	PocetVyslanych = 0;
}

unsigned char Unit1::PriradCisloRychlosti(int Rychlost)
{
	unsigned char r;
	switch (Rychlost)
	{
	case -4000: r = 0; break;
	case -2000: r = 1; break;
	case -300: r = 2; break;
	case 37: r = 3; break;
	case 38: r = 4; break;
	case 39: r = 5; break;
	case 40: r = 6; break;
	case 41: r = 7; break;
	case 42: r = 8; break;
	case 43: r = 9; break;
	case 44: r = 0x0A; break;
	case 80: r = 0x0B; break;
	case 300: r = 0x0C; break;
	case 2000: r = 0x0D; break;
	case 4000: r = 0x0E; break;
	default: r = 0x0F; break;
	}
	return r;
}

int Unit1::PriradRychlostCislu(uint8_t CisloRychlosti)
{
	int Rychlost;
	switch (CisloRychlosti)
	{
	case 0x0: Rychlost = -4000; break;
	case 0x1: Rychlost = -2000; break;
	case 0x2: Rychlost = -300; break;
	case 0x3: Rychlost = 37; break;
	case 0x4: Rychlost = 38; break;
	case 0x5: Rychlost = 39; break;
	case 0x6: Rychlost = 40; break;
	case 0x7: Rychlost = 41; break;
	case 0x8: Rychlost = 42; break;
	case 0x9: Rychlost = 43; break;
	case 0xA: Rychlost = 44; break;
	case 0xB: Rychlost = 80; break;
	case 0xC: Rychlost = 300; break;
	case 0xD: Rychlost = 2000; break;
	case 0xE: Rychlost = 4000; break;
	case 0xF: Rychlost = 0; break;
	}
	return Rychlost;
}

void Unit1::SpracujPrijatyTelegram() {	
	long PrecSn; //udej z presneho snimaca
	//short int BeznySn; //udej z bezneho snimaca
	long BeznySn;
	//int Q=0; // Poc.imp.prec.snim./Poc.kr.motora
	uint8_t AktRychlost;
	if (PRIJBF[1] == 0x0)
		dialog->setStatus(L"NULOVY TELEGRAM");
	if (PRIJBF[1] == 9)
	{
		memcpy(&AktRychlost, PRIJBF + 2, 1);
		//memcpy(&Akt1s,PRIJBF+3,1);
		memcpy(&BeznySn, PRIJBF + 3, 4);
		memcpy(&PrecSn, PRIJBF + 7, 4);
		//memcpy(&Q,PRIJBF+12,4);

		dialog->setImpulzy(0, PrecSn);
		dialog->setImpulzy(1, BeznySn);
		dialog->setRychlost(AktRychlost);

		//sprintf(str1,"%05ld",Q);
		//Form1->Label9->Caption="Hodnota Q:"+AnsiString(str1);
		//sprintf(str1,"%03u",Akt1s);
		//Form1->Label10->Caption="Aktual.sekunda:"+AnsiString(str1);
	}
}

void Unit1::VysliTelegram() {
	wchar_t str1[40];
	int brcv;
	char dlzka;
	switch (TypTel) {
		case 0: dlzka = 0; break;
		case 1: dlzka = 1; break;
		case 2: dlzka = 6; break;
		default: dlzka = 0; break;
	}

	Protocol::MakeTel(Adresa, dlzka, (char *)VYS_TEL, (char *)VYSBF);
	brcv = commport.sendAndReceive((char *)VYS_TEL, (2 * dlzka) + 8, (char *)PRIJ_TEL);
	PocetVyslanych++;
	if (Protocol::StrToDat((char *)PRIJ_TEL, (char *)PRIJBF) == 0)
	{
		PocetPrijatych++;
		SpracujPrijatyTelegram();
		memset(PRIJ_TEL, 0x0, 0x20);
	}
	else
		dialog->setStatus(L"BEZ ODPOVEDE");

	dialog->setPocetVyslanych(PocetVyslanych);
	dialog->setPocetPrijatych(PocetPrijatych);
	dialog->setStatus(L"                                ");	
	if (TypTel == 2) {
		wsprintf(str1, L"%02X %02X %02X %02X %02X %02X", VYSBF[0], VYSBF[1],
			VYSBF[2], VYSBF[3], VYSBF[4], VYSBF[5]);
		dialog->setStatus(str1);
	}
	if (TypTel == 1) {
		wsprintf(str1, L"%02X", VYSBF[0]);
		dialog->setStatus(str1);
	}
}

void Unit1::ZostavPoziadavkuSPrepinanim() {
	
	unsigned char Rychlost1, Rychlost2;
	
	/* takto sa nacita do retazca aktualne vyznacena polozka ListBoxu>
	sprintf(str1,"%s",Form1->ListBox1->Items
		   ->Strings[Form1->ListBox1->ItemIndex]);  */

	int r1 = 3 + dialog->getVybranaRychlost1SPrepinanim();
	//Rychlost1 = PriradCisloRychlosti(r1);

	int r2 = 3 + dialog->getVybranaRychlost2SPrepinanim();
	//Rychlost2 = PriradCisloRychlosti(r2);

	VYSBF[0] = 0x0;
	if (dialog->isResetChecked())
	{
		dialog->setResetChecked(false);		
		VYSBF[0] |= 0x80;
	}
	VYSBF[1] = r1; //Rychlost1;
	VYSBF[1] <<= 4;
	VYSBF[1] |= r2; // Rychlost2;
	VYSBF[2] = (uint8_t)(dialog->getPerioda1() / 0x100);  //MSB Perioda 1
	VYSBF[3] = (uint8_t)(dialog->getPerioda1() % 0x100);  //LSB Perioda 1
	VYSBF[4] = (uint8_t)(dialog->getPerioda2() / 0x100);  //MSB Perioda 2
	VYSBF[5] = (uint8_t)(dialog->getPerioda2() % 0x100);  //LSB Perioda 2

	TypTel = 2;
}

void Unit1::ZostavPoziadavkuBezPrepinania() {
	
	unsigned char Rychlost;
	
	VYSBF[0] = 0x80;
	if (dialog->isResetChecked())
	{
		dialog->setResetChecked(false);
		VYSBF[0] |= 0x40;
	}
	int r = dialog->getVybranaRychlostBezPrepinania();
	// Rychlost = PriradCisloRychlosti(r);
	VYSBF[0] |= r; // Rychlost;

	TypTel = 1;
}

void Unit1::NicNeposielaj()
{
	VYSBF[0] = 0x0;
	TypTel = 0;
}