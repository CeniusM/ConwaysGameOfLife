#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <stdio.h>
#include <Windows.h>
#include <thread>
#include <string>
using namespace std;

int nScreenWidth = 120;
int nScreenHeight = 40;



int main()
{
	// Create Screen Buffer
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth; i++)
	{
		for (int j = 0; j < nScreenHeight; j++)
		{
			screen[i + j * nScreenWidth] = ' ';
		}
	}
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	int** map = new int* [40];
	for (int i = 0; i < 40; i++)
	{
		map[i] = new int[40]{};
	}

	while (true)
	{
		auto start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < 40; i++)
		{
			for (int j = 0; j < 40; j++)
			{
				if (i == j)
					map[i][j] = 0;
			}
		}

		this_thread::sleep_for(20ms);

		// Display Frame
		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		auto finish = std::chrono::high_resolution_clock::now();

		auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
		std::cout << milliseconds.count() << "µs\n";

		const char* str = milliseconds.count() + "ms";
		SetConsoleTitleA(str);
	}

	for (int i = 0; i < 40; i++)
	{
		delete[] map[i];
	}
	delete[] map;
}