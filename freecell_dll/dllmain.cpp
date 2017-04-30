// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>

using namespace std;

typedef int(*_stdcall SET_INT)(LPCWSTR lpValueName, BYTE Data);

wchar_t *convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}

void handle_freecell()
{
	// Rq 1: Change message displayed on illegal move to "Not in this game."

	// create a new char array with the desired output.
	char new_message[17] = "Not in this game";

	// old message is currently read-only, so read it into a char array.
	char old_message[17];
	for (int i = 0; i < sizeof(old_message); i++) 
	{
		old_message[i] = 0x01010C00 + i;
	}
	
	// change old message's protections to read-write so that we can edit it.
	unsigned long oldProtect;
	VirtualProtect((LPVOID)old_message, 16, PAGE_EXECUTE_READWRITE, &oldProtect);

	// change old message to new message.
	for (int i = 0; i < sizeof(old_message); i++)
	{
		int current_address = 0x01010C00 + i;
		*(char *)current_address = new_message[i];
	}

	// revert protections.
	VirtualProtect((LPVOID)old_message, 16, oldProtect, &oldProtect);





	// Rq 2: Set total games won in the statistics window to 1000.
	
	// change the registry value for all-time wins to 1000.
	HKEY hKey; 
	RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Applets\\FreeCell", 0, KEY_ALL_ACCESS, &hKey);
	DWORD value = 1000;
	RegSetValueEx(hKey, TEXT("won"), 0, REG_BINARY, (BYTE *)&value, sizeof(value));
	// close key since we are done with the operation.
	RegCloseKey(hKey);





	// Rq 3: Next valid move will win the game.


	


	// Rq 4: Change 'Abort' dialog option when using Ctrl-Shift-F10 cheat code to automatically win game.
	HINSTANCE hinstAcc;
	HACCEL old_haccel = LoadAccelerators(hinstAcc, L"FREEMENU");

	ACCEL accelerator[8];
	CopyAcceleratorTable(old_haccel, accelerator, 8);

	// 0x0D  = CONTROL, SHIFT, VIRTKEY
	// 114   = Key of accelerator that brings up 'Abort' dialog box.
	// VK_F6 = Makes F6 the accelerator.
	accelerator[7] = { 0x0D, 114, VK_F6 };

	// remove old accelerator table.
	DestroyAcceleratorTable(old_haccel);

	// load new accelerator table with editted accelerator list.
	HACCEL new_haccel = LoadAccelerators(hinstAcc, MAKEINTRESOURCE(accelerator));





	// Rq 5: Create cheat code that when Ctrl-Shift-F2 to win the game.

	
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		MessageBoxA(0, "Hello World!", "Hello Title", 1);
		handle_freecell();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

