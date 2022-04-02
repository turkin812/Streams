
// Task3Dlg.cpp : файл реализации
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

// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
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


// диалоговое окно CTask3Dlg



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


// обработчики сообщений CTask3Dlg

BOOL CTask3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetTimer(1, 500, NULL);

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
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

	HANDLE CurrentProc = GetCurrentProcess(); // текущий процесс	//Создал описатель и через функцию получил Хендл нашего окна
	SetProcessAffinityMask(CurrentProc, 1);	//	//Заставил работаь нашу программу на одном ядре (хандл текущего приложения, номер ядра)

	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST); // GetCurrentThread текущий поток и задаю ему приоритет

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
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

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CTask3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
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
		C1++; // раз в 200мс обновляется на 0
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
	// TODO: добавьте свой код обработчика уведомлений
	/*DWORD dwThreadID;
	DWORD dwThreadID2;*/
	SetTimer(1, 200, NULL); // Каждые 200мс вызывается OnTimer
	hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);//Поток
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
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	SliderThread.SetPos(C1 / (C1 + C2) * 100); //Кол-во итераций первого потока / все итерации
	SliderThread2.SetPos(C2 / (C1 + C2) * 100);
	C1 = 0; // Чтобы не накапливались итерации, снова от нуля
	C2 = 0;
	
	CDialogEx::OnTimer(nIDEvent);
}


void CTask3Dlg::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	/* if (x != 0){
		SliderThread.SetState(PBST_NORMAL);
	}*/
	*pResult = 0;
}


void CTask3Dlg::OnNMCustomdrawProgress2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	/*if (y != 0){
		SliderThread2.SetState(PBST_NORMAL);
	}*/
	*pResult = 0;
}


void CTask3Dlg::OnBnClickedButton2()
{
	// TODO: добавьте свой код обработчика уведомлений
	DWORD Exit_1;
	DWORD Exit_2;
	GetExitCodeThread(hThread, &Exit_1); //Возращает код завершения в эту ссылку &Exit_1, если Exit_1 =  0 или -1 то ошибка
	GetExitCodeThread(hThread2, &Exit_2);
	TerminateThread(hThread, Exit_1); // Завершает поток
	TerminateThread(hThread2, Exit_2);
	KillTimer(1); //Удаляет первый таймер
	SliderThread.SetPos(0); // По нулям прогресс бар
	SliderThread2.SetPos(0);
}


void CTask3Dlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
}


void CTask3Dlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
}


void CTask3Dlg::OnBnClickedCheck1()
{
	// TODO: добавьте свой код обработчика уведомлений
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
	// TODO: добавьте свой код обработчика уведомлений
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
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
}


void CTask3Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	SetThreadPriority(hThread, SliderThread1.GetPos());
	SetThreadPriority(hThread2, Slider2.GetPos());

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
