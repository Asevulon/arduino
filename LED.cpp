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
	bool rCode = false;
	switch (mode)
	{
	case Static: rCode = OnModeStatic();
		break;
	case Off: rCode = OnModeOff();
		break;
	default:
		break;
	}
	return rCode;
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
