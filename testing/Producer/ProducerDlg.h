
// ProducerDlg.h : header file
//

#pragma once

#include "Unit1.h"

class Unit1;

// CProducerDlg dialog
class CProducerDlg : public CDialog
{
// Construction
public:
	CProducerDlg(CWnd* pParent = nullptr);	// standard constructor

	void setImpulzy(int snimacID, long pocetImpulzov);
	void setRychlost(uint8_t rychlost);
	void setStatus(wchar_t* status);
	void setPocetPrijatych(unsigned int pocet);
	void setPocetVyslanych(unsigned int pocet);
	int getVybranaRychlostBezPrepinania();
	int getVybranaRychlost1SPrepinanim();
	int getVybranaRychlost2SPrepinanim();
	bool isResetChecked();
	void setResetChecked(bool newState);
	int getPerioda1();
	int getPerioda2();
	bool isSPrepinanim();

	void OnTimer(UINT_PTR nIDEvent);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRODUCER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CFont m_font;
	CStatic m_staticPoziadavka;
	CStatic m_staticStav;
	CStatic m_staticKomunikacia;
	CStatic m_pocetImpulzov0;
	CStatic m_pocetImpulzov1;
	CStatic m_aktRychlost;
	CStatic m_vyslanych;
	CStatic m_prijatych;
	CStatic m_status;
	CListBox m_listbox_rychlost_bez;
	CListBox m_listbox_rychlost1_s;
	CListBox m_listbox_rychlost2_s;
	CButton m_resetchecked;
	CEdit m_perioda1;
	CEdit m_perioda2;
	CButton m_radio_s_prepinanim;

	Unit1 unit1;
public:
	afx_msg void OnClickedButtonVysli();
	afx_msg void OnBnClickedRadioSPrepinanim();
	afx_msg void OnBnClickedRadioBezPrepinania();
};
