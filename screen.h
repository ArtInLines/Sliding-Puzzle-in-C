#ifndef SCREEN_H_
#define SCREEN_H_

#ifdef _WIN32
#include <Windows.h> // For clearing terminal screen on windows
#include <conio.h>   // For getch
#elif
#include <termios.h> // termios, TCSANOW, ECHO, ICANON
#include <unistd.h>  // STDIN_FILENO
#endif

void clear_screen()
{
#ifdef _WIN32
    COORD topLeft  = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
#else
    if (system("CLS")) system("clear");
#endif
}

#endif // SCREEN_H_