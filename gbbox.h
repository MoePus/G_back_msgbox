#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <functional>
#include <thread>

struct GBMSGBOXPARAMS
{
	uint32_t cbSize;
	HANDLE Hwnd;
	HINSTANCE HINSTANCE;
	LPCWSTR lpszText;
	LPCWSTR lpszCaption;
	uint32_t dwStyle;
	uint32_t lpszIcon;
	uint32_t dwContextHelpId;
	void* lpfnMsgBoxCallback;
	uint32_t dwLanguageId;
	uint32_t unk_28;
	uint32_t unk_2C;
	uint32_t unk_30;
	const uint32_t * lpButtonIdGroup;
	const wchar_t** lpButtonTextGroup;
	uint32_t buttonNum;
	uint32_t unk_40;
	uint32_t cancelID;
	uint32_t dwTimeOut;
	uint32_t unk_4C;
	uint32_t unk_50;
	uint32_t unk_54;
	uint32_t unk_58;
	uint32_t unk_5C;
	std::function<void(::HWND hwnd,void* data)> GBBOXCallback;
	void* userData;
};
namespace GBbox
{
	uint32_t _hookFunAddr;
}

class GBMSGBOX
{
public:
	enum mirror_option
	{
		GB_NO_MIRROR = 0x0,
		GB_CAPTION_MIRROR = 0x10,
		GB_TEXT_MIRROR = 0x1,
		GB_TEXTANDCAPTION_MIRROR = 0x11
	};
	template<typename funcType>
	void GBBOXASYNC(std::wstring text, std::wstring caption, uint32_t style, std::vector<std::wstring> button, mirror_option mirror = GB_NO_MIRROR, uint32_t timeOut = -1, funcType CallBck = NULL, void* data = NULL);
	template<typename funcType>
	void GBBOX(std::wstring text, std::wstring caption, uint32_t style, std::vector<std::wstring> button, mirror_option mirror = GB_NO_MIRROR, uint32_t timeOut = -1, funcType CallBck = NULL, void* data = NULL);
	GBMSGBOX::GBMSGBOX()
	{
		HMODULE user32 = LoadLibraryA("user32.dll");
		if (!user32)
		{
			MSGBOXDRAWHOOKER(0,0,0,0);
		}
		SoftModalMessageBox = (FunctionType_SoftModalMessageBox)GetProcAddress(user32, "SoftModalMessageBox");

		void* hookFunAddr = memstr(reinterpret_cast<char*>(user32 + 0x1000), 1024 * 1024, reinterpret_cast<char*>(subMatchVChar));
		if (hookFunAddr)
		{
			hookFunAddr = reinterpret_cast<void*>(reinterpret_cast<long>(hookFunAddr) - 0x15);
			hookFunAddr = memstr(reinterpret_cast<char*>(hookFunAddr), 0x15, "\x8B\xFF");
			hookFunAddr = reinterpret_cast<void*>(reinterpret_cast<long>(hookFunAddr) - 0x5);
			DWORD oldProtect;
			if (VirtualProtect(hookFunAddr, 7, PAGE_EXECUTE_READWRITE, &oldProtect))
			{
				uint8_t payload[] = { 0xE9,0x00,0x00,0x00,0x00,0xEB,0xF9 };
				*(uint32_t*)(payload + 1) = reinterpret_cast<uint32_t>(MSGBOXDRAWHOOKER) - (reinterpret_cast<uint32_t>(hookFunAddr) + 5);
				memcpy(hookFunAddr, payload, 7);
				GBbox::_hookFunAddr = reinterpret_cast<uint32_t>(hookFunAddr) + 7;
			}
		}
	}
	/*void demo()
	{

		GBMSGBOXPARAMS params;
		memset(&params, 0, sizeof(GBMSGBOXPARAMS));


		params.cbSize = 'Magi';
		params.lpszCaption = L"Demo";
		params.lpszText = L"defaultGbBox";

		const static uint32_t num1 = 1;
		params.buttonNum = 1;
		params.lpButtonIdGroup = &num1;
		const wchar_t* buttonText = L"Hello world!";
		params.lpButtonTextGroup = &buttonText;

		params.dwTimeOut = 1500;
		params.dwStyle = 0xffffffff;

		params.GBBOXCallback = demoCallBck;

		callSoftModalMessageBox(&params);
	}*/
	
private:


	const static HBRUSH __stdcall MSGBOXDRAWHOOKER(::HWND hWnd, int a2, HDC a3, GBMSGBOXPARAMS *a4)
	{
		
		if (a2 == 0x110 && a4->cbSize == 'Magi')
		{
			if (a4->GBBOXCallback)
			{
				std::thread th(a4->GBBOXCallback, hWnd, a4->userData);
				th.detach();
			}
		}



		uint32_t funcaddr = GBbox::_hookFunAddr;
		::HWND _hWnd = hWnd;
		int _a2 = a2;
		HDC _a3 = a3;
		void* _a4 = a4;
		HBRUSH result;
		__asm
		{	
			push _a4
			push _a3
			push _a2
			push _hWnd
			mov EBX, funcaddr
			call EBX
			mov result, EAX
		}
		return result;
	}



    uint8_t subMatchVChar[16] = { 0x3B,0xC1,0x0F,0x87,0x30,0x02,0x00,0x00,0x0F,0x84,0xAA,0x01,0x00,0x00,0x83,0xE8 };
	char* memstr(char* full_data, int full_data_len, char* substr)
	{
		if (full_data == NULL || full_data_len <= 0 || substr == NULL) {
			return NULL;
		}

		if (*substr == '\0') {
			return NULL;
		}

		int sublen = strlen(substr);

		int i;
		char* cur = full_data;
		int last_possible = full_data_len - sublen + 1;
		for (i = 0; i < last_possible; i++) {
			if (*cur == *substr) {
				if (memcmp(cur, substr, sublen) == 0) {
					//found  
					return cur;
				}
			}
			cur++;
		}

		return NULL;
	}


	/*static void demoCallBck(HWND hWnd)
	{
		std::cout << (int)hWnd;
	}*/
	__forceinline int callSoftModalMessageBox(GBMSGBOXPARAMS* params)
	{
		void* func_proc = SoftModalMessageBox;
		void* param_proc = params;
		int result;
		__asm
		{
			mov EAX, param_proc
			push EAX
			mov ebx, func_proc
			call ebx
			mov result,EAX
		}
		return result;
	}
	typedef int(*FunctionType_SoftModalMessageBox)(GBMSGBOXPARAMS* lp);
	FunctionType_SoftModalMessageBox SoftModalMessageBox;
};
template<typename funcType>
inline void GBMSGBOX::GBBOXASYNC(std::wstring text, std::wstring caption, uint32_t style, std::vector<std::wstring> button, mirror_option mirror, uint32_t timeOut, funcType CallBck, void* data)
{
	std::thread th(&GBMSGBOX::GBBOX<funcType>, this, text, caption, style, button, mirror, timeOut, CallBck, data);
	th.detach();
}

template<typename funcType>
void GBMSGBOX::GBBOX(std::wstring text, std::wstring caption, uint32_t style, std::vector<std::wstring> button, mirror_option mirror, uint32_t timeOut, funcType CallBck, void* data)
{

	if (mirror)
	{
		style = (15*mirror) << 16;
	}
	GBMSGBOXPARAMS params;
	memset(&params, 0, sizeof(GBMSGBOXPARAMS));
	params.cbSize = 'Magi';
	params.lpszCaption = caption.c_str();
	params.lpszText = text.c_str();
	params.dwTimeOut = timeOut;
	params.dwStyle = style;
	if (data)
	{
		params.GBBOXCallback = CallBck;
		params.userData = data;
	}

	uint32_t size = button.size();
	params.buttonNum = size;
	auto buffer = new uint32_t[size*2];
	for (uint32_t i = 0; i < size; i++)
	{
		buffer[i] = i + 1;
	}
	for (uint32_t i = 0; i < size; i++)
	{
		buffer[size + i] = reinterpret_cast<uint32_t>(button[i].c_str());
	}
	params.lpButtonIdGroup = buffer;
	params.lpButtonTextGroup = const_cast<const wchar_t**>(reinterpret_cast<wchar_t**>(&buffer[size]));

	callSoftModalMessageBox(&params);
}