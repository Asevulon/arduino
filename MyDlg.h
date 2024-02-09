
// MyDlg.h: файл заголовка
//

#pragma once
#include<fstream>
#include"LED.h"
// Диалоговое окно MyDlg
class MyDlg : public CDialogEx
{
// Создание
public:
	MyDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ARDUINO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	ULONG ports[255];
	ULONG PortsCount = 255;
	ULONG PortsFound = 0;
	void GetComPorts();

	HANDLE hSerial;
	LED* led;
	bool init = false;

	void ProcessError();
	void UpdateEditControls();
	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox ComList;
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonOn();
	afx_msg void OnBnClickedButtonOff();
	CMFCColorPickerCtrl ColorPicker;
	CPalette palette;
	CPalette palette1;

	CMFCColorPickerCtrl LunCtrl;
	afx_msg void OnClickedColorPicker();
	afx_msg void OnClickedLuminance();
	unsigned char m_R;
	unsigned char m_G;
	unsigned char m_B;
	double m_Lun;
	double m_Hue;
	double m_Sat;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit6();
	CComboBox ModeList;
	afx_msg void OnCbnSelchangeModeList();
	CSliderCtrl EffectT;
	afx_msg void OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
};
