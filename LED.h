#pragma once
#define LED_NUM 120
#include"ArduinoDialogMessages.h"

enum ModeLED
{
	Static,
	Off
};

struct Color
{
	UINT8 r = 0;
	UINT8 g = 0;
	UINT8 b = 0;
	Color() { }
	Color(UINT8 r, UINT8 g, UINT8 b) :r(r), g(g), b(b) {}
	//заполняет массив arr[3]
	void AsArray(UINT8* arr);
	//заполняет 3 элемента массива начиная с pos
	void AsArray(UINT8* arr, UINT32 pos);
};

class LED
{
private:
	ModeLED mode = Static;
	ModeLED WakeUpState = Static;

	UINT8 m_r[LED_NUM];
	UINT8 m_g[LED_NUM];
	UINT8 m_b[LED_NUM];

	HANDLE hSerial;

	Color m_color;

	bool IsSleeping = false;
protected:
	bool OnModeStatic();
	bool OnModeOff();
public:
	LED(HANDLE& SerialHandle);
	void SetMode(ModeLED m);
	void SetColor(Color& c);
	void SetColor(UINT8 r, UINT8 g, UINT8 b);
	void SetColor(COLORREF c);
	void WakeUp();
	bool MakeAndSend();
};

