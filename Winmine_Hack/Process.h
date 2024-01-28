#pragma once

#include <Windows.h>

#define WINMINE_NAME	L"WINMINE.EXE"

class Process
{
private:
	const LPUINT	pWidth = (LPUINT)0x010056AC;
	const LPUINT	pHeight = (LPUINT)0x010056A8;
	const LPBYTE	pField = (LPBYTE)0x01005360;
	HANDLE			hProcess;
public:
	Process(void);
	~Process(void);
	bool	GetProcess(void);
	bool	ReadMineField(void);
private:
	DWORD	GetWinminePID(void);
	bool	CreateWinmineProcess(DWORD);
};