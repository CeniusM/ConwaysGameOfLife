#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <stdio.h>
#include <Windows.h>
#include <thread>
#include <string>
#include <conio.h>
using namespace std;

int mapWidth = 100;
int mapHeight = 60;

int nScreenWidth = mapWidth * 2;// each square is 2 wide
int nScreenHeight = mapHeight;



/*

Ideer

to get a 2d grid of the screen you can maby do something like this

wchar_t** arr2d = new wchar_t*[nScreenHeight];
for (int i = 0; i < nScreenHeight; i++)
{
	arr2d[i] = &screen[i * nScreenWidth];
}


// this sould edit, right?
// screen[4 * nScreenHeight + 6 * nScreenWidth] = 'A';

arr2d[4][6] = 'A';

*/


void GenerateNewMap(int** map)
{
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
}

void NewGeneration(int** map, int** newMap, int width, int height)
{
	for (int i = 0; i < mapWidth; i++)
	{
		for (int j = 0; j < mapHeight; j++)
		{
			int aliveAround = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					if (i + x < 0 || i + x > mapWidth - 1 || j + y < 0 || j + y > mapHeight - 1 || (x == 0 && y == 0))
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
}

void GenerateFrame(int** map, wchar_t* screen)
{
	for (int i = 0; i < mapWidth; i++)
	{
		for (int j = 0; j < mapHeight; j++)
		{
			if (map[i][j] == 1)
			{
				screen[(i << 1) + j * nScreenWidth] = 0x2588; // '█' = UTF-16 (hex) 0x2588
				screen[(i << 1) + j * nScreenWidth + 1] = 0x2588;
			}
			else
			{
				screen[(i << 1) + j * nScreenWidth] = ' ';
				screen[(i << 1) + j * nScreenWidth + 1] = ' ';
			}
		}
	}
}

int** InitMap(int x, int y)
{
	int** map = new int* [x];
	for (int i = 0; i < x; i++)
	{
		map[i] = new int[y] {};
	}
	return map;
}

void DeleteMap(int** map, int x)
{
	for (int i = 0; i < x; i++)
	{
		delete[] map[i];
	}
	delete[] map;
}

void SetConsoleSize(int x, int y, int charWidth, int charHeight)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, x * charWidth + 40/*why i need +40, no clue*/, y * charHeight + 40, TRUE);
}

int main()
{
	SetConsoleSize(nScreenWidth, nScreenHeight, 8, 16);

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

	// init maps
	int** map = InitMap(mapWidth, mapHeight);
	int** newMap = InitMap(mapWidth, mapHeight);

	GenerateNewMap(map);

	while (true)
	{
		if (GetKeyState('R') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
		{
			GenerateNewMap(map);
		}


		this_thread::sleep_for(20ms); // the game speed

		// timer
		auto start = std::chrono::high_resolution_clock::now();

		NewGeneration(map, newMap, mapWidth, mapHeight);

		GenerateFrame(map, screen);

		// timer
		auto finish = std::chrono::high_resolution_clock::now();
		auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
		//const char* str = milliseconds.count() + "ms";
		//SetConsoleTitleA(str);

		// Display Frame
		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	DeleteMap(map, mapWidth);
	DeleteMap(newMap, mapWidth);
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