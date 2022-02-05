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

	enum COLORS
	{
		BLACK_FG = 0x0000,
		DARK_BLUE_FG = 0x0001,
		DARK_GREEN_FG = 0x0002,
		DARK_CYAN_FG = 0x0003,
		DARK_RED_FG = 0x0004,
		DARK_MAGENTA_FG = 0x0005,
		DARK_YELLOW_FG = 0x0006,
		GREY_FG = 0x0007,
		DARK_GREY_FG = 0x0008,
		BLUE_FG = 0x0009,
		GREEN_FG = 0x000A,
		CYAN_FG = 0x000B,
		RED_FG = 0x000C,
		MAGENTA_FG = 0x000D,
		YELLOW_FG = 0x000E,
		WHITE_FG = 0x000F,
		BLACK_BG = 0x0000,
		DARK_BLUE_BG = 0x0010,
		DARK_GREEN_BG = 0x0020,
		DARK_CYAN_BG = 0x0030,
		DARK_RED_BG = 0x0040,
		DARK_MAGENTA_BG = 0x0050,
		DARK_YELLOW_BG = 0x0060,
		GREY_BG = 0x0070,
		DARK_GREY_BG = 0x0080,
		BLUE_BG = 0x0090,
		GREEN_BG = 0x00A0,
		CYAN_BG = 0x00B0,
		RED_BG = 0x00C0,
		MAGENTA_BG = 0x00D0,
		YELLOW_BG = 0x00E0,
		WHITE_BG = 0x00F0,
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

	void DrawString(int x, int y, std::string c, short color = 0x000F)
	{
		for (int i = 0; i < c.length(); i++)
		{
			screenBuffer[y * consoleWidth + x + i].Char.UnicodeChar = c[i];
			screenBuffer[y * consoleWidth + x + i].Attributes = color;
		}
		Draw();
	}

	void FillColor(short col)
	{
		for (int i = 0; i < consoleWidth * consoleHeight; i++)
		{
			screenBuffer[i].Char.UnicodeChar = 0x2591;
			screenBuffer[i].Attributes = col;
		}
		Draw();
	};

	void DrawRect(int x, int y, int endX, int endY, short color = 0x000F)
	{

		int tempTopY = y;
		int tempTopX = tempTopY * consoleWidth + x;
		int currentX = x;

		for (int i = 0; i < consoleWidth * consoleHeight; i++)
		{
			for (int j = 0; j < consoleWidth * consoleHeight; j++)
			{
				if(currentX == endX)
				{
					currentX = x;
					break;
				}
				if (tempTopX <= tempTopY * consoleWidth + endX)
				{
					screenBuffer[tempTopX].Char.UnicodeChar = 0x2591;
					screenBuffer[tempTopX].Attributes = color;
					tempTopX++;
					currentX++;
				}
			}
			if (tempTopY == endY) {
				break;
			}
			tempTopX = tempTopY * consoleWidth + x;
			tempTopY++;
		};

		Draw();
	};

	void SetAppName(std::string appName)
	{
		wchar_t convAppName[256] = { 0 };
		memset(convAppName, 0, 256);
		for (int i = 0; i < appName.length(); i++)
		{
			convAppName[i] = appName[i];
		}
		SetConsoleTitle(convAppName);
	};



protected:
	void Draw()
	{
		WriteConsoleOutput(consoleHandle, screenBuffer, { (short)consoleWidth, (short)consoleHeight }, { 0,0 }, &rectWnd);
	};
};
