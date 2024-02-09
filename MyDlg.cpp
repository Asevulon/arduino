
// MyDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "arduino.h"
#include "MyDlg.h"
#include "afxdialogex.h"
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib,"OneCore.lib")
using namespace std;
// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно MyDlg



MyDlg::MyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ARDUINO_DIALOG, pParent)
	, m_R(0)
	, m_G(0)
	, m_B(0)
	, m_Lun(0)
	, m_Hue(0)
	, m_Sat(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COM_LIST, ComList);
	DDX_Control(pDX, IDC_COLOR_PICKER, ColorPicker);
	DDX_Control(pDX, IDC_LUMINANCE, LunCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_R);
	DDX_Text(pDX, IDC_EDIT2, m_G);
	DDX_Text(pDX, IDC_EDIT3, m_B);
	DDX_Text(pDX, IDC_EDIT4, m_Lun);
	DDX_Text(pDX, IDC_EDIT5, m_Hue);
	DDX_Text(pDX, IDC_EDIT6, m_Sat);
	DDX_Control(pDX, IDC_MODE_LIST, ModeList);
	DDX_Control(pDX, IDC_SLIDER1, EffectT);
}

BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &MyDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_ON, &MyDlg::OnBnClickedButtonOn)
	ON_BN_CLICKED(IDC_BUTTON_OFF, &MyDlg::OnBnClickedButtonOff)
	ON_BN_CLICKED(IDC_COLOR_PICKER, &MyDlg::OnClickedColorPicker)
	ON_BN_CLICKED(IDC_LUMINANCE, &MyDlg::OnClickedLuminance)
	ON_EN_CHANGE(IDC_EDIT1, &MyDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &MyDlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &MyDlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &MyDlg::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &MyDlg::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT6, &MyDlg::OnEnChangeEdit6)
	ON_CBN_SELCHANGE(IDC_MODE_LIST, &MyDlg::OnCbnSelchangeModeList)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &MyDlg::OnNMCustomdrawSlider1)
END_MESSAGE_MAP()


// Обработчики сообщений MyDlg

void MyDlg::GetComPorts()
{
	ULONG res = GetCommPorts(ports, PortsCount, &PortsFound);
	if (res != 0)abort();

	for (int i = 0; i < PortsFound; i++)
	{
		CString str = L"COM";
		str += to_string(ports[i]).c_str();
		ComList.InsertString(-1, str);
	}
	ComList.SetCurSel(PortsFound - 1);
}

void MyDlg::ProcessError()
{
	DWORD Error = GetLastError();
	wchar_t* text;
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, Error, 0, (LPTSTR)&text, 0, NULL);
	MessageBox(text, L"Ошибка", MB_ICONERROR);
	LocalFree(text);
}

void MyDlg::UpdateEditControls()
{
	COLORREF c = ColorPicker.GetColor();
	m_R = GetRValue(c);
	m_G = GetGValue(c);
	m_B = GetBValue(c);
	LunCtrl.GetHLS(&m_Hue, &m_Lun, &m_Sat);
	UpdateData(FALSE);
}

BOOL MyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
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

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	GetComPorts();
	ColorPicker.SetColor(RGB(50, 50, 50));
	ColorPicker.SetType(CMFCColorPickerCtrl::COLORTYPE::PICKER);
	ColorPicker.SetPalette(&palette);

	LunCtrl.SetColor(RGB(50, 50, 50));
	LunCtrl.SetType(CMFCColorPickerCtrl::COLORTYPE::LUMINANCE);
	LunCtrl.SetPalette(&palette1);

	ModeList.InsertString(-1, L"Статический");
	ModeList.InsertString(-1, L"Водопад");
	ModeList.SetCurSel(0);

	EffectT.SetRange(1, 20);
	EffectT.SetPos(7);
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void MyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void MyDlg::OnPaint()
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
HCURSOR MyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void MyDlg::OnBnClickedButtonConnect()
{
	// TODO: добавьте свой код обработчика уведомлений
	CloseHandle(hSerial);


	CString PortName;
	ComList.GetWindowTextW(PortName);

	hSerial = CreateFileW(PortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		ProcessError();
		return;
	}
	
	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (!GetCommState(hSerial, &dcbSerialParams))
	{
		MessageBox(L"getting state error", L"Error", MB_ICONERROR);
		return;
	}
	dcbSerialParams.BaudRate = CBR_115200;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	if (!SetCommState(hSerial, &dcbSerialParams))
	{
		MessageBox(L"error setting serial port state", L"Error", MB_ICONERROR);
		return;
	}

	CString SuccessMessage = PortName + L" подключен";
	MessageBox(SuccessMessage);

	delete led;
	led = new LED(hSerial);

	init = true;
}

void MyDlg::OnBnClickedButtonOn()
{
	if (!init)return;
	led->WakeUp();
	if (!led->MakeAndSend())ProcessError();
}


void MyDlg::OnBnClickedButtonOff()
{
	if (!init)return;

	led->SetMode(Off);
	if (!led->MakeAndSend()) ProcessError();
}


void MyDlg::OnClickedColorPicker()
{
	if (!init)return;
	led->SetColor(ColorPicker.GetColor());
	LunCtrl.SetHLS(ColorPicker.GetHue(), ColorPicker.GetLuminance(), ColorPicker.GetSaturation());
	if (!led->MakeAndSend())ProcessError();
	UpdateEditControls();
} 


void MyDlg::OnClickedLuminance()
{
	if (!init)return;
	ColorPicker.SetColor(LunCtrl.GetColor());
	led->SetColor(LunCtrl.GetColor());
	if (!led->MakeAndSend())ProcessError();
	UpdateEditControls();
}








void MyDlg::OnEnChangeEdit1()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// функция и вызов CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
	if (!init)return;

	UpdateData();
	ColorPicker.SetColor(RGB(m_R, m_G, m_B));
	LunCtrl.SetColor(RGB(m_R, m_G, m_B));
	led->SetColor(LunCtrl.GetColor());
	if (!led->MakeAndSend())ProcessError();
	UpdateEditControls();

}


void MyDlg::OnEnChangeEdit2()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// функция и вызов CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
	if (!init)return;

	UpdateData();
	ColorPicker.SetColor(RGB(m_R, m_G, m_B));
	LunCtrl.SetColor(RGB(m_R, m_G, m_B));
	led->SetColor(LunCtrl.GetColor());
	if (!led->MakeAndSend())ProcessError();
	UpdateEditControls();

}


void MyDlg::OnEnChangeEdit3()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// функция и вызов CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
	if (!init)return;

	UpdateData();
	ColorPicker.SetColor(RGB(m_R, m_G, m_B));
	LunCtrl.SetColor(RGB(m_R, m_G, m_B));
	led->SetColor(LunCtrl.GetColor());
	if (!led->MakeAndSend())ProcessError();
	UpdateEditControls();

}


void MyDlg::OnEnChangeEdit4()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// функция и вызов CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
	if (!init)return;

	UpdateData();
	LunCtrl.SetHLS(m_Hue, m_Lun, m_Sat);
	ColorPicker.SetColor(LunCtrl.GetColor());
	led->SetColor(LunCtrl.GetColor());
	if (!led->MakeAndSend())ProcessError();
	UpdateEditControls();

}


void MyDlg::OnEnChangeEdit5()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// функция и вызов CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
	if (!init)return;

	UpdateData();
	LunCtrl.SetHLS(m_Hue, m_Lun, m_Sat);
	ColorPicker.SetColor(LunCtrl.GetColor());
	led->SetColor(LunCtrl.GetColor());
	if (!led->MakeAndSend())ProcessError();
	UpdateEditControls();

}


void MyDlg::OnEnChangeEdit6()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// функция и вызов CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
	if (!init)return;

	UpdateData();
	LunCtrl.SetHLS(m_Hue, m_Lun, m_Sat);
	ColorPicker.SetColor(LunCtrl.GetColor());
	led->SetColor(LunCtrl.GetColor());
	if (!led->MakeAndSend())ProcessError();
	UpdateEditControls();
}


void MyDlg::OnCbnSelchangeModeList()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (!init)return;
	CString curmode;
	ModeList.GetWindowTextW(curmode);

	if (curmode == L"Статический")led->SetMode(Static);
	else if (curmode == L"Водопад")led->SetMode(Waterfall);

	if (!led->MakeAndSend())ProcessError();
}


void MyDlg::OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;

	if (!init)return;

	led->SetEffectT(EffectT.GetPos());
	if (!led->MakeAndSend())ProcessError();
}
