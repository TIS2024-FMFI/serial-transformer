
// ProducerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Producer.h"
#include "ProducerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CProducerDlg dialog



CProducerDlg::CProducerDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PRODUCER_DIALOG, pParent),
	unit1(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProducerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_POZIADAVKA, m_staticPoziadavka);
	DDX_Control(pDX, IDC_STATIC_STAV, m_staticStav);
	DDX_Control(pDX, IDC_STATIC_KOMUNIKACIA, m_staticKomunikacia);
	DDX_Control(pDX, IDC_STATIC_IMPULZOV_0, m_pocetImpulzov0);
	DDX_Control(pDX, IDC_STATIC_IMPULZOV_1, m_pocetImpulzov1);
	DDX_Control(pDX, IDC_STATIC_RYCHLOST, m_aktRychlost);
	DDX_Control(pDX, IDC_STATIC_VYSLANYCH, m_vyslanych);
	DDX_Control(pDX, IDC_STATIC_PRIJATYCH, m_prijatych);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_status);
	DDX_Control(pDX, IDC_LIST_BEZ_PREP_RYCHLOSTI, m_listbox_rychlost_bez);
	DDX_Control(pDX, IDC_LIST_S_PREP_RYCHLOSTI1, m_listbox_rychlost1_s);
	DDX_Control(pDX, IDC_LIST_S_PREP_RYCHLOSTI2, m_listbox_rychlost2_s);
	DDX_Control(pDX, IDC_CHECK_SRESETOM, m_resetchecked);
	DDX_Control(pDX, IDC_EDIT_T1, m_perioda1);
	DDX_Control(pDX, IDC_EDIT_T2, m_perioda2);
	DDX_Control(pDX, IDC_RADIO_S_PREPINANIM, m_radio_s_prepinanim);
}

BEGIN_MESSAGE_MAP(CProducerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_VYSLI, &CProducerDlg::OnClickedButtonVysli)
	ON_BN_CLICKED(IDC_RADIO_S_PREPINANIM, &CProducerDlg::OnBnClickedRadioSPrepinanim)
	ON_BN_CLICKED(IDC_RADIO_BEZ_PREPINANIA, &CProducerDlg::OnBnClickedRadioBezPrepinania)
	ON_WM_TIMER() 
END_MESSAGE_MAP()

void CProducerDlg::setImpulzy(int snimacID, long pocetImpulzov)
{
	CString str;
	str.Format(_T("Počet impulzov snímača %d: %05ld"), snimacID, pocetImpulzov);
	CStatic* pimp = &m_pocetImpulzov0;
	if (snimacID == 1)
		pimp = &m_pocetImpulzov1;
	pimp->SetWindowText(str);
}

void CProducerDlg::setRychlost(uint8_t rychlost)
{
	CString str;
	str.Format(_T("Aktuálna rýchlosť: %d krokov/s"), unit1.PriradRychlostCislu(rychlost));
	m_aktRychlost.SetWindowText(str);
}

void CProducerDlg::setStatus(wchar_t *status)
{
	m_status.SetWindowText(status);
}

void CProducerDlg::setPocetPrijatych(unsigned int pocet)
{
	CString str;
	str.Format(_T("Počet prijatých telegramov: %05u"), pocet);
	m_prijatych.SetWindowText(str);
}

void CProducerDlg::setPocetVyslanych(unsigned int pocet)
{
	CString str;
	str.Format(_T("Počet vyslaných telegramov: %05u"), pocet);
	m_vyslanych.SetWindowText(str);
}

int CProducerDlg::getVybranaRychlostBezPrepinania()
{
	return m_listbox_rychlost_bez.GetCurSel();
}

int CProducerDlg::getVybranaRychlost1SPrepinanim()
{
	return m_listbox_rychlost1_s.GetCurSel();
}

int CProducerDlg::getVybranaRychlost2SPrepinanim()
{
	return m_listbox_rychlost2_s.GetCurSel();
}

bool CProducerDlg::isResetChecked()
{
	return m_resetchecked.GetCheck();
}

void CProducerDlg::setResetChecked(bool newState)
{
	m_resetchecked.SetCheck(newState);
}

int CProducerDlg::getPerioda1()
{
	CString text;
	m_perioda1.GetWindowText(text);
	return _ttoi(text);
}

int CProducerDlg::getPerioda2()
{
	CString text;
	m_perioda2.GetWindowText(text);
	return _ttoi(text);
}

bool CProducerDlg::isSPrepinanim()
{
	return m_radio_s_prepinanim.GetCheck();
}

// CProducerDlg message handlers

BOOL CProducerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Initialize the font
	m_font.CreatePointFont(120, _T("Arial")); // 120 = 12pt font size

	m_staticPoziadavka.SetFont(&m_font);
	m_staticStav.SetFont(&m_font);
	m_staticKomunikacia.SetFont(&m_font);

	// Set the first radio button as checked by default
	CheckRadioButton(IDC_RADIO_S_PREPINANIM, IDC_RADIO_BEZ_PREPINANIA, IDC_RADIO_S_PREPINANIM);

	m_listbox_rychlost1_s.AddString(_T("37"));
	m_listbox_rychlost1_s.AddString(_T("38"));
	m_listbox_rychlost1_s.AddString(_T("39"));
	m_listbox_rychlost1_s.AddString(_T("40"));
	m_listbox_rychlost1_s.AddString(_T("41"));
	m_listbox_rychlost1_s.AddString(_T("42"));
	m_listbox_rychlost1_s.AddString(_T("43"));
	m_listbox_rychlost1_s.AddString(_T("44"));

	m_listbox_rychlost2_s.AddString(_T("37"));
	m_listbox_rychlost2_s.AddString(_T("38"));
	m_listbox_rychlost2_s.AddString(_T("39"));
	m_listbox_rychlost2_s.AddString(_T("40"));
	m_listbox_rychlost2_s.AddString(_T("41"));
	m_listbox_rychlost2_s.AddString(_T("42"));
	m_listbox_rychlost2_s.AddString(_T("43"));
	m_listbox_rychlost2_s.AddString(_T("44"));

	m_listbox_rychlost_bez.AddString(_T("-4000"));
	m_listbox_rychlost_bez.AddString(_T("-2000"));
	m_listbox_rychlost_bez.AddString(_T("-300"));
	m_listbox_rychlost_bez.AddString(_T("37"));
	m_listbox_rychlost_bez.AddString(_T("38"));
	m_listbox_rychlost_bez.AddString(_T("39"));
	m_listbox_rychlost_bez.AddString(_T("40"));
	m_listbox_rychlost_bez.AddString(_T("41"));
	m_listbox_rychlost_bez.AddString(_T("42"));
	m_listbox_rychlost_bez.AddString(_T("43"));
	m_listbox_rychlost_bez.AddString(_T("44"));
	m_listbox_rychlost_bez.AddString(_T("80"));
	m_listbox_rychlost_bez.AddString(_T("300"));
	m_listbox_rychlost_bez.AddString(_T("2000"));
	m_listbox_rychlost_bez.AddString(_T("4000"));
	m_listbox_rychlost_bez.AddString(_T("0"));

	m_listbox_rychlost1_s.SetCurSel(3);
	m_listbox_rychlost2_s.SetCurSel(4);
	m_listbox_rychlost_bez.SetCurSel(0x0F);
	m_resetchecked.SetCheck(0);
	m_perioda1.SetWindowText(_T("3683"));
	m_perioda2.SetWindowText(_T("650"));

	if (!unit1.portOpen) {
		MessageBox(L"Port sa nepodarilo otvoriť", L"Chyba", MB_OK);
		EndDialog(IDCANCEL);
	}

	SetTimer(1, 1000, 0);   // send telegram every 1000 ms
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CProducerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CProducerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CProducerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CProducerDlg::OnClickedButtonVysli()
{
	if (isSPrepinanim())
	{
		if (getVybranaRychlost1SPrepinanim() == -1 || getVybranaRychlost2SPrepinanim() == -1)
		{
			MessageBox(L"Vyberte rýchlosť 1 a rýchlosť 2 pre prepínanie", L"Chyba", MB_OK);
			return;
		}
	}
	else
	{
		if (getVybranaRychlostBezPrepinania() == -1)
		{
			MessageBox(L"Vyberte rýchlosť bez prepínania", L"Chyba", MB_OK);
			return;
		}
	}

	unit1.StlaceneVysli = true;
}

void CProducerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (unit1.StlaceneVysli == true) //zostavi sa poziadavka
	{
		unit1.StlaceneVysli = false;
		if (isSPrepinanim())
			unit1.ZostavPoziadavkuSPrepinanim();
		else
			unit1.ZostavPoziadavkuBezPrepinania();		
	}
	else  unit1.NicNeposielaj();
	
	unit1.VysliTelegram();

	CDialog::OnTimer(nIDEvent);
}

void CProducerDlg::OnBnClickedRadioSPrepinanim()
{
	m_listbox_rychlost1_s.EnableWindow(true);
	m_listbox_rychlost2_s.EnableWindow(true);
	m_listbox_rychlost_bez.EnableWindow(false);
	m_perioda1.EnableWindow(true);
	m_perioda2.EnableWindow(true);
}


void CProducerDlg::OnBnClickedRadioBezPrepinania()
{
	m_listbox_rychlost1_s.EnableWindow(false);
	m_listbox_rychlost2_s.EnableWindow(false);
	m_listbox_rychlost_bez.EnableWindow(true);
	m_perioda1.EnableWindow(false);
	m_perioda2.EnableWindow(false);
}
