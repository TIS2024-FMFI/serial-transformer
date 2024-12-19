#pragma once

#include <stdint.h>
#include "ProducerDlg.h"
#include "serial.h"

class CProducerDlg;

class Unit1 {

public:

	Unit1(CProducerDlg *dlg);
	unsigned char PriradCisloRychlosti(int Rychlost);
	int PriradRychlostCislu(uint8_t CisloRychlosti);
	void SpracujPrijatyTelegram();
	void VysliTelegram();
	void ZostavPoziadavkuSPrepinanim();
	void ZostavPoziadavkuBezPrepinania();
	void NicNeposielaj();

	bool StlaceneVysli;
	bool portOpen;
	int TypTel = 0;

private:
	CProducerDlg *dialog;


	uint8_t VYSBF[0x30], PRIJBF[0x30], VYS_TEL[0x30], PRIJ_TEL[0x30];
	char Adresa = 0x33;
	uint8_t Poziadavka, STAV;
	unsigned int PocetPrijatych, PocetVyslanych;
	unsigned int Rychl1, Rychl2, PerRychl1, PerRychl2; //rychl a periody s prepinanim	
	bool SResetom;
	SerialPort commport;
	

};
