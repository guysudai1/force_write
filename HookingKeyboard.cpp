// HookingKeyboard.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <string>
#include <iostream>
#include <Windows.h>

HHOOK _hook;
std::string desired_string;
int INDEX;

class keys {
	public: 
		static const int plus = VK_OEM_PLUS;
		static const int comm = VK_OEM_COMMA;
		static const int minu = VK_OEM_MINUS;
		static const int dot = VK_OEM_PERIOD;
		static const int question_mark = VK_OEM_2;
		static const int tilda = VK_OEM_3;
} ;

void Unhook() {
	UnhookWindowsHookEx(_hook);
	exit(0);
}

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
	KBDLLHOOKSTRUCT keyboardStrct;
	
	if (code >= 0 && wParam == WM_KEYDOWN) {
		keyboardStrct = *((KBDLLHOOKSTRUCT*)lParam);
		int letter = keyboardStrct.vkCode;
		// Detect if the program sent the key or the user pressed it.
		if (keyboardStrct.time == 1337) return 0;
		if (('A' <= letter && letter <= 'Z') || ('0' <= letter && letter <= '9') || (VK_OEM_1 <= letter && letter <= VK_OEM_3) || ('[' <= letter && letter <= '\'')) {
			std::cout << "Pressed" << std::endl;
			// Check if wrote string already.
			if (INDEX >= desired_string.length()) {
				Unhook();
			}
			// Get keys state
			SHORT caps_locked = GetKeyState(VK_CAPITAL) & 1;
			SHORT shift_locked = GetAsyncKeyState(VK_SHIFT) & 1;

			INPUT input;
			input.type = INPUT_KEYBOARD;
			input.ki.dwFlags = 0;
			input.ki.time = 1337; // Time is 1337 so the program knows it's not the user.
			// Check if current letter in word is lowercase or uppercase.
			SHORT lower = (isupper(desired_string[INDEX]) != 0) ? 1 : 0;
			input.ki.wVk = toupper(desired_string[INDEX]);
			std::cout << "Caps: " << caps_locked << " Shift(L): " << (GetAsyncKeyState(VK_LSHIFT) & 1) << " Shift(R): " << (GetAsyncKeyState(VK_RSHIFT) & 1) << " Upper: " << lower << std::endl;
			// Test if program needs to set CAPSLOCK or not.
			if ((caps_locked ^ shift_locked) ^ lower != 0) {
				// toggle CAPSLOCK
				input.ki.wVk = VK_CAPITAL;
				SendInput(1, &input, sizeof(INPUT));
				// Write character instead of user's character.
				input.ki.wVk = toupper(desired_string[INDEX]);

				SendInput(1, &input, sizeof(INPUT));

				// Release flag
				input.ki.dwFlags = KEYEVENTF_KEYUP;

				// Release character
				input.ki.wVk = toupper(desired_string[INDEX]);
				SendInput(1, &input, sizeof(INPUT));

				// Release CAPSLOCK
				input.ki.wVk = VK_CAPITAL;
				SendInput(1, &input, sizeof(INPUT));

			}
			else {
				if (input.ki.wVk == '.') input.ki.wVk = keys::dot;
				else if (input.ki.wVk == ',') input.ki.wVk = keys::comm;
				else if (input.ki.wVk == '-') input.ki.wVk = keys::minu;
				else if (input.ki.wVk == '+') input.ki.wVk = keys::plus;
				else if (input.ki.wVk == '?') input.ki.wVk = keys::question_mark;
				else if (input.ki.wVk == '~') input.ki.wVk = keys::tilda;

				input.ki.wVk = toupper(input.ki.wVk);
				SendInput(1, &input, sizeof(INPUT));

				input.ki.dwFlags = KEYEVENTF_KEYUP;
				input.ki.wVk = toupper(input.ki.wVk);
				SendInput(1, &input, sizeof(INPUT));
			}


			INDEX++;
			return 3;
		}
		else {
			return CallNextHookEx(_hook, code, wParam, lParam);
		}

	}
	return CallNextHookEx(_hook, code, wParam, lParam);
}



BOOL WINAPI DllMain(HINSTANCE hinstDLL,
	DWORD     fdwReason,
	LPVOID    lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH) {
		INDEX = 0;

		// Acquire string
		desired_string = "www.youtube.com";
		desired_string += '\x0d';
		// Hook keyboard function.
		_hook = SetWindowsHookExA(WH_KEYBOARD_LL, KeyboardProc, hinstDLL, 0);

		if (!_hook) {
			MessageBoxA(NULL, "Failed to install hook!", "Error", MB_ICONERROR);
		}
		else {
			MessageBoxA(NULL, "Hooked keyboard!", "Hook", MB_ICONINFORMATION);
		}

		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{

		}
		// Unhook.
	}
	else if (fdwReason == DLL_PROCESS_DETACH) {
		Unhook();
}
}


