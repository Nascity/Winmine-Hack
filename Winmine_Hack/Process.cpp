#include "Process.h"
#include <TlHelp32.h>
#include <cwchar>

Process::Process(void)
	: hProcess(0) { }

Process::~Process(void)
{
	CloseHandle(hProcess);
}

#include <iostream>

bool Process::GetProcess(void)
{
	DWORD	dwWinminePID;

	dwWinminePID = GetWinminePID();
	if (!dwWinminePID)
		return false;
	if (!CreateWinmineProcess(dwWinminePID))
		return false;
	return true;
}

DWORD Process::GetWinminePID(void)
{
	HANDLE			hProcSnap;
	PROCESSENTRY32W	pe;

	hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcess == INVALID_HANDLE_VALUE)
		return 0;
	pe.dwSize = sizeof(PROCESSENTRY32W);

	if (Process32FirstW(hProcSnap, &pe))
	{
		do
		{
			if (!wcscmp(pe.szExeFile, WINMINE_NAME))
			{
				CloseHandle(hProcSnap);
				return pe.th32ProcessID;
			}
		} while (Process32NextW(hProcSnap, &pe));
	}
	CloseHandle(hProcSnap);
	return 0;
}

bool Process::CreateWinmineProcess(DWORD dwPID)
{
	HANDLE			hProcSnap;
	MODULEENTRY32W	me;

	hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	if (hProcSnap == INVALID_HANDLE_VALUE)
		return false;
	me.dwSize = sizeof(MODULEENTRY32W);

	if (Module32FirstW(hProcSnap, &me))
	{
		do
		{
			if (!wcscmp(me.szModule, WINMINE_NAME))
			{
				hProcess = OpenProcess(
					PROCESS_CREATE_THREAD | PROCESS_VM_READ,
					FALSE,
					dwPID);
				CloseHandle(hProcSnap);
				if (!hProcess)
					return false;
				return true;
			}
		} while (Module32NextW(hProcSnap, &me));
	}
	CloseHandle(hProcSnap);
	return false;
}

bool Process::ReadMineField(void)
{
	UINT width, height;
	SIZE_T uiSize;
	char* pmap;
	char temp;
	int i, j;

	if (!ReadProcessMemory(hProcess, pWidth, &width, sizeof(UINT), &uiSize) || uiSize != sizeof(UINT))
		return false;
	if (!ReadProcessMemory(hProcess, pHeight, &height, sizeof(UINT), &uiSize) || uiSize != sizeof(UINT))
		return false;

	pmap = new char [32 * 24];
	if (!ReadProcessMemory(hProcess, pField, pmap, sizeof(char) * 32 * 24, &uiSize) || uiSize != sizeof(char) * 32 * 24)
	{
		delete[] pmap;
		return false;
	}
	for (i = 0; i < height; i++)
	{
		for (j = 1; j <= width; j++)
		{
			temp = pmap[i * 32 + j];
			std::cout << ' ';
			if (temp & 0x80)
				std::cout << 'X';
			else if (temp & 0x40 && temp - 0x40)
				std::cout << (int)(temp - 0x40);
			else
				std::cout << '.';
			std::cout << ' ';
		}
		std::cout << '\n';
	}
	return true;
}