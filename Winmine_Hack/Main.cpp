#include <iostream>
#include <cstdlib>
#include "Process.h"

int main(void)
{
	Process* pp;
	char temp;
	char* map;

	pp = new Process();
	if (!pp->GetProcess())
	{
		std::cout << "Failed to open WINMINE.EXE\n";
		return -1;
	}
	do
	{
		std::cout << "Type \'s\' when you are ready!! >> ";
		std::cin >> temp;
	} while (temp != 's');
	do
	{
		if (!pp->ReadMineField())
		{
			std::cout << "Cannot read map.\n";
			return -1;
		}
		std::cout << "Type \'e\' to finish!! >> ";
		temp = getchar();
		system("cls");
	} while (temp != 'e');
	return 0;
}