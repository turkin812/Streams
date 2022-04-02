
// Task3Dlg.cpp : ���� ����������
//

#include "stdafx.h"
#include "Task3.h"
#include "Task3Dlg.h"
#include "afxdialogex.h"

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int Otnoshenie_1;
int Otnoshenie_2;

HANDLE hThread;
HANDLE hThread2;

double C1;
double C2;

BOOL CheckProc_1 = false;
BOOL CheckProc_2 = false;

// ���������� ���� CAboutDlg ������������ ��� �������� �������� � ����������

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ������ ����������� ����
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

// ����������
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// ���������� ���� CTask3Dlg



CTask3Dlg::CTask3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTask3Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTask3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, OnStart);
	DDX_Control(pDX, IDC_BUTTON2, OnExit);
	DDX_Control(pDX, IDC_CHECK1, OnCheckSleep1);
	DDX_Control(pDX, IDC_CHECK2, OnCheckSleep2);
	DDX_Control(pDX, IDC_SLIDER1, SliderThread1); //Slider1
	DDX_Control(pDX, IDC_PROGRESS2, SliderThread2); //Progress2
	DDX_Control(pDX, IDC_PROGRESS1, SliderThread); //Progress1
	DDX_Control(pDX, IDC_SLIDER2, Slider2); //Slider2
}

BEGIN_MESSAGE_MAP(CTask3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTask3Dlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CTask3Dlg::OnNMCustomdrawProgress1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS2, &CTask3Dlg::OnNMCustomdrawProgress2)
	ON_BN_CLICKED(IDC_BUTTON2, &CTask3Dlg::OnBnClickedButton2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CTask3Dlg::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CTask3Dlg::OnNMCustomdrawSlider2)
	ON_BN_CLICKED(IDC_CHECK1, &CTask3Dlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CTask3Dlg::OnBnClickedCheck2)
	ON_NOTIFY(NM_OUTOFMEMORY, IDC_SLIDER1, &CTask3Dlg::OnSlider1)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// ����������� ��������� CTask3Dlg

BOOL CTask3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetTimer(1, 500, NULL);

	// ���������� ������ "� ���������..." � ��������� ����.

	// IDM_ABOUTBOX ������ ���� � �������� ��������� �������.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	SliderThread1.SetRange(THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_HIGHEST);
	Slider2.SetRange(THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_HIGHEST);

	SliderThread1.SetPos(THREAD_PRIORITY_NORMAL);
	Slider2.SetPos(THREAD_PRIORITY_NORMAL);

	SliderThread1.SetTicFreq(1);
	Slider2.SetTicFreq(1);

	SliderThread.SetRange(0, 100);
	SliderThread2.SetRange(0, 100);

	HANDLE CurrentProc = GetCurrentProcess(); // ������� �������	//������ ��������� � ����� ������� ������� ����� ������ ����
	SetProcessAffinityMask(CurrentProc, 1);	//	//�������� ������� ���� ��������� �� ����� ���� (����� �������� ����������, ����� ����)

	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST); // GetCurrentThread ������� ����� � ����� ��� ���������

	// ������ ������ ��� ����� ����������� ����.  ����� ������ ��� �������������,
	//  ���� ������� ���� ���������� �� �������� ����������
	SetIcon(m_hIcon, TRUE);			// ������� ������
	SetIcon(m_hIcon, FALSE);		// ������ ������

	// TODO: �������� �������������� �������������

	return TRUE;  // ������� �������� TRUE, ���� ����� �� ������� �������� ����������
}

void CTask3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��� ���������� ������ ����������� � ���������� ���� ����� ��������������� ����������� ���� �����,
//  ����� ���������� ������.  ��� ���������� MFC, ������������ ������ ���������� ��� �������������,
//  ��� ������������� ����������� ������� ��������.

void CTask3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �������� ���������� ��� ���������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ������������ ������ �� ������ ����������� ��������������
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ��������� ������
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ������� �������� ��� ������� ��� ��������� ����������� ������� ��� �����������
//  ���������� ����.
HCURSOR CTask3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

long x = 0;
long y = 0;

DWORD WINAPI ThreadFunc(PVOID pvParam){
	int j = 9;
	double k = cos(M_PI / 9);
	for (int i = 0;; i++)
	{


		double Otnoshenie = cos(M_PI / j) / (cos(2 * M_PI) + k);
		j++;
		k = k * 2;


		if (Otnoshenie < 0.01 || Otnoshenie == 1)
		{
			i = 0; j = 3; k = cos(M_PI / 9);
		}
		C1++; // ��� � 200�� ����������� �� 0
		//Otnoshenie_1 = Otnoshenie * 100;

		if (CheckProc_1 == true)
		{
			Sleep(0);
		}


	}

	return 0;
}

DWORD WINAPI ThreadFunc2(PVOID pvParam){
	int j = 9;
	double k = cos(M_PI / 9);
	for (int i = 0;; i = i + 5)
	{
		double Otnoshenie = cos(M_PI / j) / (cos(2 * M_PI) + k);
		j++;
		k = k * 2;

		if (Otnoshenie < 0.01 || Otnoshenie == 1)
		{
			i = 0; j = 3; k = cos(M_PI / 9);
		}
		C2++;
		//Otnoshenie_2 = Otnoshenie * 100;

		if (CheckProc_2 == true)
		{
			Sleep(0);
		}
	}

	return 0;
}

void CTask3Dlg::OnBnClickedButton1()
{
	// TODO: �������� ���� ��� ����������� �����������
	/*DWORD dwThreadID;
	DWORD dwThreadID2;*/
	SetTimer(1, 200, NULL); // ������ 200�� ���������� OnTimer
	hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);//�����
	hThread2 = CreateThread(NULL, 0, ThreadFunc2, NULL, 0, NULL);
	/*SetThreadPriority(hThread, THREAD_PRIORITY_IDLE);
	SetThreadPriority(hThread2, THREAD_PRIORITY_IDLE);
	ResumeThread(hThread);
	ResumeThread(hThread2);
	



	CloseHandle(hThread);
	CloseHandle(hThread2);*/

	OnStart.EnableWindow(false);
}

void CTask3Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �������� ���� ��� ����������� ��������� ��� ����� ������������
	SliderThread.SetPos(C1 / (C1 + C2) * 100); //���-�� �������� ������� ������ / ��� ��������
	SliderThread2.SetPos(C2 / (C1 + C2) * 100);
	C1 = 0; // ����� �� ������������� ��������, ����� �� ����
	C2 = 0;
	
	CDialogEx::OnTimer(nIDEvent);
}


void CTask3Dlg::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �������� ���� ��� ����������� �����������
	/* if (x != 0){
		SliderThread.SetState(PBST_NORMAL);
	}*/
	*pResult = 0;
}


void CTask3Dlg::OnNMCustomdrawProgress2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �������� ���� ��� ����������� �����������
	/*if (y != 0){
		SliderThread2.SetState(PBST_NORMAL);
	}*/
	*pResult = 0;
}


void CTask3Dlg::OnBnClickedButton2()
{
	// TODO: �������� ���� ��� ����������� �����������
	DWORD Exit_1;
	DWORD Exit_2;
	GetExitCodeThread(hThread, &Exit_1); //��������� ��� ���������� � ��� ������ &Exit_1, ���� Exit_1 =  0 ��� -1 �� ������
	GetExitCodeThread(hThread2, &Exit_2);
	TerminateThread(hThread, Exit_1); // ��������� �����
	TerminateThread(hThread2, Exit_2);
	KillTimer(1); //������� ������ ������
	SliderThread.SetPos(0); // �� ����� �������� ���
	SliderThread2.SetPos(0);
}


void CTask3Dlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �������� ���� ��� ����������� �����������
	*pResult = 0;
}


void CTask3Dlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �������� ���� ��� ����������� �����������
	*pResult = 0;
}


void CTask3Dlg::OnBnClickedCheck1()
{
	// TODO: �������� ���� ��� ����������� �����������
	if (CheckProc_1 == false)
	{
		CheckProc_1 = true;
	}
	else
	{
		CheckProc_1 = false;
	}
}


void CTask3Dlg::OnBnClickedCheck2()
{
	// TODO: �������� ���� ��� ����������� �����������
	if (CheckProc_2 == false)
	{
		CheckProc_2 = true;
	}
	else
	{
		CheckProc_2 = false;
	}
}


void CTask3Dlg::OnSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �������� ���� ��� ����������� �����������
	*pResult = 0;
}


void CTask3Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �������� ���� ��� ����������� ��������� ��� ����� ������������
	SetThreadPriority(hThread, SliderThread1.GetPos());
	SetThreadPriority(hThread2, Slider2.GetPos());

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
