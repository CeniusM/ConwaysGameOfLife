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

	// init map
	int** map = new int* [nScreenWidth];
	for (int i = 0; i < nScreenWidth; i++)
	{
		map[i] = new int[nScreenHeight]{};
	}

	// generate map
	for (int i = 0; i < nScreenWidth; i++)
	{
		for (int j = 0; j < nScreenHeight; j++)
		{
			if (rand() % 3 == 1)
			{
				map[i][j] = 1;
			}
		}
	}

	while (true)
	{
		this_thread::sleep_for(1000ms); // the game speed

		//// test
		//for (int i = 0; i < nScreenWidth; i++)
		//	for (int j = 0; j < nScreenHeight; j++)
		//		map[i][j] ^= 0b1;

		// timer
		auto start = std::chrono::high_resolution_clock::now();

		// new genearation
		int** newMap = new int* [nScreenWidth];
		for (int i = 0; i < nScreenWidth; i++)
		{
			newMap[i] = new int[nScreenHeight]{};
		}
		
		for (int i = 0; i < nScreenWidth; i++)
		{
			for (int j = 0; j < nScreenHeight; j++)
			{
				int aliveAround = 0;
				for (int x = -1; x < 2; x++)
				{
					for (int y = -1; y < 2; y++)
					{
						if (i + x < 0 || i + x > nScreenWidth - 1 || j + y < 0 || j + y > nScreenHeight - 1)
							continue;							
						if (map[i + x][j + y] == 1)
								aliveAround++;
					}
				}
				if (map[i][j] == 1)
				{
					if (aliveAround < 2)
					{
						newMap[i][j] = 0;
						continue;
					}
					else if (aliveAround > 3)
					{
						newMap[i][j] = 0;
						continue;
					}
					newMap[i][j] = 1;
				}
				else if (aliveAround == 3)
					newMap[i][j] = 0;
			}
		}
		
		// clean up and new init
		for (int i = 0; i < nScreenWidth; i++)
		{
			for (int j = 0; j < nScreenHeight; j++)
			{
				map[i][j] = newMap[i][j];
			}
			delete[] newMap[i];
		}
		delete[] newMap;


		// generate frame
		for (int i = 0; i < nScreenWidth; i++)
		{
			for (int j = 0; j < nScreenHeight; j++)
			{
				if (map[i][j] == 1)
					screen[i + j * nScreenWidth] = 'O'; //(char)219; //(char)219 = box character
				else
					screen[i + j * nScreenWidth] = ' ';

				if (i == 0)
					screen[i + j * nScreenWidth] = 'B';
				else if (j == 0)
					screen[i + j * nScreenWidth] = 'B';
				else if (i == nScreenWidth - 1)
					screen[i + j * nScreenWidth] = 'B';
				else if (j == nScreenHeight - 1)
					screen[i + j * nScreenWidth] = 'B';
			}
		}


		// timer
		auto finish = std::chrono::high_resolution_clock::now();
		auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
		//const char* str = milliseconds.count() + "ms";
		//SetConsoleTitleA(str);

		// Display Frame
		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	for (int i = 0; i < 40; i++)
	{
		delete[] map[i];
	}
	delete[] map;
}