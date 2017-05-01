// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>

using namespace std;

typedef int(*_stdcall MOVE_CARDS)(HWND);

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

	// get handle of current window.
	HWND hwnd = GetForegroundWindow();
	// change moveindex int stored in memory to 53 so that the transfer loop in move_cards is repeated for all 52 cards.
	*(unsigned int *)0x01007864 = 53;

	// call the move_cards function.
	MOVE_CARDS move_cards_fcn = (MOVE_CARDS)0x01004FC7;
	move_cards_fcn(hwnd);
	



	// Rq 4: Change 'Abort' dialog option when using Ctrl-Shift-F10 cheat code to automatically win game.
	
	HINSTANCE hinstAcc;
	HACCEL old_haccel = LoadAccelerators(hinstAcc, L"FREEMENU");

	ACCEL accelerator[9];
	CopyAcceleratorTable(old_haccel, accelerator, 8);

	// 0x0D  = CONTROL, SHIFT, VIRTKEY
	// 114   = Key of accelerator that brings up 'Abort' dialog box.
	// VK_F6 = Makes F6 the accelerator.
	accelerator[7] = { 0x0D, 114, VK_F6 };
	
	// this is actually for Rq 5, but creating now so that I will not have to repeat accelerator table creation in Rq 5.
	// 0x0D  = CONTROL, SHIFT, VIRTKEY
	// 114   = Key of accelerator that brings up 'Abort' dialog box.
	// VK_F2 = Makes F2 the accelerator.
	accelerator[8] = { 0x0D, 116, VK_F2 };

	// remove old accelerator table.
	DestroyAcceleratorTable(old_haccel);

	// load new accelerator table with editted accelerator list.
	HACCEL new_haccel = LoadAccelerators(hinstAcc, MAKEINTRESOURCE(accelerator));





	// Rq 5: Create cheat code that when Ctrl-Shift-F2 to win the game.

	// wasn't sure how to program this Rq, but I believe that you need to add a 16th case (case for ID 116) for the switch statement located at 0x01001EA1
	// then have that case point to loc_10020ED at 0x010020ED which is where the Rq 4 cheat takes you after pressing 'Abort', thus doing the same cheat as Rq 4, but 
	// skipping the message box as desired.
	// NOTE: this is based upon the assumption that you don't mean by using this cheat, you auto win with no card movement required.
	
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

