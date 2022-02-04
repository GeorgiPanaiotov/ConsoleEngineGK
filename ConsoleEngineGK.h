#pragma once
#include <Windows.h>
#include <string>


class ConsoleEngineGK
{
private:
	int consoleWidth;
	int consoleHeight;
	CHAR_INFO* screenBuffer;
	HANDLE consoleHandle;
	std::wstring appName;
	SMALL_RECT rectWnd;


public:
	ConsoleEngineGK()
	{
		consoleWidth = 120;
		consoleHeight = 40;
		rectWnd = { 0, 0, (short)consoleWidth, (short)consoleHeight };
		consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		appName = L"Default";
	};

	void CreateConsole(int width, int height)
	{
		consoleWidth = width;
		consoleHeight = height;
		rectWnd = { 0, 0, (short)consoleWidth, (short)consoleHeight };
		SetConsoleWindowInfo(consoleHandle, TRUE, &rectWnd);

		COORD coordinates = { (short)consoleWidth, (short)consoleHeight };
		SetConsoleScreenBufferSize(consoleHandle, coordinates);
		SetConsoleActiveScreenBuffer(consoleHandle);

		rectWnd = { 0, 0, (short)consoleWidth - 1, (short)consoleHeight - 1 };
		SetConsoleWindowInfo(consoleHandle, TRUE, &rectWnd);

		screenBuffer = new CHAR_INFO[consoleWidth * consoleHeight];
		memset(screenBuffer, 0, sizeof(CHAR_INFO) * consoleWidth * consoleHeight);
	};

	void DrawString(int x, int y, std::string c, short col = 0x000F)
	{
		for (int i = 0; i < c.length(); i++)
		{
			screenBuffer[y * consoleWidth + x + i].Char.UnicodeChar = c[i];
			screenBuffer[y * consoleWidth + x + i].Attributes = col;
		}
		Draw();
	}

	void Draw()
	{
		WriteConsoleOutput(consoleHandle, screenBuffer, { (short)consoleWidth, (short)consoleHeight }, { 0,0 }, &rectWnd);
	};
};
