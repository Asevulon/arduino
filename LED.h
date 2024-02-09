#pragma once
#define LED_NUM 120
#include"ArduinoDialogMessages.h"
#include"Global.h"

enum ModeLED
{
	Static,
	Off,
	Waterfall
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
	struct ModeParamsInfo
	{
		HANDLE thread;
		bool stop = false;
		HANDLE StopEvent;
		void CreateStopEvent();
		void CreateEffectThread(LPTHREAD_START_ROUTINE lpAddress, LPVOID lpParam);
		void StopEffectThread();
	};
	ModeParamsInfo mpi;

	ModeLED mode = Static;
	ModeLED WakeUpState = Static;
	
	UINT32 EffectSpeed = 10;
	int EffectT = 1;
	HANDLE hSerial;

	Color m_color;

	bool IsSleeping = false;
protected:
	bool OnModeStatic();
	bool OnModeOff();
	bool OnModeWaterfall();

public:
	LED(HANDLE& SerialHandle);
	void SetMode(ModeLED m);
	void SetColor(Color& c);
	void SetColor(UINT8 r, UINT8 g, UINT8 b);
	void SetColor(COLORREF c);
	void SetEffectT(int T);
	void WakeUp();
	bool MakeAndSend();
};

