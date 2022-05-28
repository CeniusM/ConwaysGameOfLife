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

int mapWidth = 60;
int mapHeight = 60;

int nScreenWidth = mapWidth * 2;// each square is 2 wide
int nScreenHeight = mapHeight;



int main()
{
	// set console size
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, nScreenWidth * 8 + 40/*why i need +40, no clue*/, nScreenHeight * 16 + 40, TRUE);

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
	int** map = new int* [mapWidth];
	for (int i = 0; i < mapWidth; i++)
	{
		map[i] = new int[mapHeight]{};
	}
	int** newMap = new int* [mapWidth];
	for (int i = 0; i < mapWidth; i++)
	{
		newMap[i] = new int[mapHeight] {};
	}

	// generate map
	for (int i = 0; i < mapWidth; i++)
	{
		for (int j = 0; j < mapHeight; j++)
		{
			if (rand() % 3 == 1)
			{
				map[i][j] = 1;
			}
		}
	}

	while (true)
	{
		this_thread::sleep_for(5ms); // the game speed

		//// test
		//for (int i = 0; i < nScreenWidth; i++)
		//	for (int j = 0; j < nScreenHeight; j++)
		//		map[i][j] ^= 0b1;

		// timer
		auto start = std::chrono::high_resolution_clock::now();


		for (int i = 0; i < mapWidth; i++)
		{
			for (int j = 0; j < mapHeight; j++)
			{
				int aliveAround = 0;
				for (int x = -1; x < 2; x++)
				{
					for (int y = -1; y < 2; y++)
					{
						if (i + x < 0 || i + x > mapHeight - 1 || j + y < 0 || j + y > mapHeight - 1 || (x == 0 && y == 0))
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
					if (aliveAround == 2 || aliveAround == 3)
					{
						newMap[i][j] = 1;
					}
				}
				else if (map[i][j] == 0)
				{
					if (aliveAround == 3)
						newMap[i][j] = 1;
				}
			}
		}


		for (int i = 0; i < mapWidth; i++)
		{
			for (int j = 0; j < mapHeight; j++)
			{
				map[i][j] = newMap[i][j];
			}
		}


		// generate frame
		for (int i = 0; i < nScreenWidth; i += 2) 
		{
			for (int j = 0; j < nScreenHeight; j++)
			{
				if (map[i >> 1][j] == 1)
				{
					screen[i + j * nScreenWidth] = 0x2588; // '█' = UTF-16 (hex) 0x2588
					screen[i + j * nScreenWidth + 1] = 0x2588;
				}
				else
				{
					screen[i + j * nScreenWidth] = ' ';
					screen[i + j * nScreenWidth + 1] = ' ';
				}
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

	for (int i = 0; i < mapWidth; i++)
	{
		delete[] map[i];
	}
	delete[] map;
	for (int i = 0; i < mapWidth; i++)
	{
		delete[] newMap[i];
	}
	delete[] newMap;
}



/*

gradiants

█ full			= UTF-16 (hex) 0x2588 = alt 219
▓ sort of full	= UTF-16 (hex) 0x2593 = alt 178
▒ half			= UTF-16 (hex) 0x2592 = alt 177
░ low			= UTF-16 (hex) 0x2591 = alt 176

*/











/*
graveyard
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
		}
		for (int i = 0; i < nScreenWidth; i++)
		{
			delete[] newMap[i];
		}
		delete[] newMap;

*/