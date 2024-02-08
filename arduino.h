
// arduino.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CarduinoApp:
// Сведения о реализации этого класса: arduino.cpp
//

class CarduinoApp : public CWinApp
{
public:
	CarduinoApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CarduinoApp theApp;
