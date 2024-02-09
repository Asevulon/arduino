#include "pch.h"
#include "LED.h"
using namespace std;

bool LED::OnModeStatic()
{
	char lModeWord = STATIC_MODE;
	DWORD dwSize = sizeof(lModeWord);
	DWORD dwBytesWritten;
	if (!WriteFile(hSerial, &lModeWord, dwSize, &dwBytesWritten, NULL)) return false;

	unsigned char data[3] = { 0, 0, 0 };
	m_color.AsArray(data);
	dwSize = sizeof(data);
	if (!WriteFile(hSerial, data, dwSize, &dwBytesWritten, NULL)) return false;

	return true;
}

bool LED::OnModeOff()
{
	IsSleeping = true;

	char lModeWord = OFF;
	DWORD dwSize = sizeof(lModeWord);
	DWORD dwBytesWritten;
	if (!WriteFile(hSerial, &lModeWord, dwSize, &dwBytesWritten, NULL)) return false;

	return true;
}

LED::LED(HANDLE& SerialHandle)
{
	hSerial = SerialHandle;
	mpi.CreateStopEvent();
}

void LED::SetEffectT(int T)
{
	EffectT = T;
}

void LED::SetMode(ModeLED m)
{
	WakeUpState = mode;
	mode = m;
}

void LED::SetColor(Color& c)
{
	m_color = c;
}

void LED::WakeUp()
{
	if (!IsSleeping)return;
	IsSleeping = false;
	mode = WakeUpState;
	
}
void LED::SetColor(UINT8 r, UINT8 g, UINT8 b)
{
	m_color = Color(r, g, b);
}
void LED::SetColor(COLORREF c)
{
	m_color = Color(GetRValue(c), GetGValue(c), GetBValue(c));
}

bool LED::MakeAndSend()
{
	mpi.StopEffectThread();
	
	bool rCode = false;

	switch (mode)
	{
	case Static: rCode = OnModeStatic();
		break;
	case Off: rCode = OnModeOff();
		break;
	case Waterfall: rCode = OnModeWaterfall();
	default:
		break;
	}
	return rCode;
}

bool LED::OnModeWaterfall()
{
	char lModeWord = WATERFALL;
	DWORD dwSize = sizeof(lModeWord);
	DWORD dwBytesWritten;
	if (!WriteFile(hSerial, &lModeWord, dwSize, &dwBytesWritten, NULL)) return false;

	unsigned char data[3] = { 0, 0, 0 };
	m_color.AsArray(data);
	dwSize = sizeof(data);
	if (!WriteFile(hSerial, data, dwSize, &dwBytesWritten, NULL)) return false;

	unsigned char ET = EffectT;
	dwSize = sizeof(ET);
	if (!WriteFile(hSerial, &ET, dwSize, &dwBytesWritten, NULL)) return false;

	return true;
}


void SendRGB(char* r, char* g, char* b, HANDLE hSerial)
{
	char lModeWord = SHOW_PACKAGE;
	DWORD dwSize = sizeof(lModeWord);
	DWORD dwBytesWritten;
	WriteFile(hSerial, &lModeWord, dwSize, &dwBytesWritten, NULL);

	char data[3];
	for (int i = 0; i < LED_NUM; i++)
	{
		dwSize = sizeof(data);
		data[0] = r[i];
		data[1] = g[i];
		data[2] = b[i];
		WriteFile(hSerial, data, dwSize, &dwBytesWritten, NULL);
		
		dwSize = sizeof(lModeWord);
		ReadFile(hSerial, &lModeWord, dwSize, &dwBytesWritten, NULL);
	}
}


void Color::AsArray(UINT8* arr)
{
	arr[0] = r;
	arr[1] = g;
	arr[2] = b;
}

void Color::AsArray(UINT8* arr, UINT32 pos)
{
	arr[pos] = r;
	arr[pos + 1] = g;
	arr[pos + 2] = b;
}

void LED::ModeParamsInfo::CreateStopEvent()
{
	//CloseHandle(StopEvent);
	StopEvent = CreateEventW(NULL, TRUE, TRUE, L"LEDStopEvent");
	SetEvent(StopEvent);
}

void LED::ModeParamsInfo::CreateEffectThread(LPTHREAD_START_ROUTINE lpAddress, LPVOID lpParam)
{
	stop = true;
	WaitForSingleObject(StopEvent, INFINITE);
	CloseHandle(thread);

	stop = false; 
	thread = CreateThread(NULL, NULL, lpAddress, lpParam, NULL, NULL);
	ResetEvent(StopEvent);
}

void LED::ModeParamsInfo::StopEffectThread()
{
	stop = true;
	WaitForSingleObject(StopEvent, INFINITE);
	CloseHandle(thread);
}
