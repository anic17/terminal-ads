#include <windows.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

FILE *ad_file = NULL;

const int scrollcount = 2;

enum
{
    FREE = 1,
    BASIC,
    PREMIUM,
    PREMIUM_PLUS,
} payment_tiers;

int pay_tier = FREE;
int colortable[] = {0x6, 0x2, 0x3, 0xe, 0xd, 0x9, 0xb, 0xa};
int color_count = 0;

int gotoxy(HANDLE hConsole, int x, int y)
{
    COORD position = {x, y};
    SetConsoleCursorPosition(hConsole, position);
}

char *get_new_ad()
{
    const size_t allocsz = 256;
    char *buf = calloc(allocsz, sizeof(char));
    if (ad_file == NULL)
    {
        ad_file = fopen("ad.lst", "rb");
        if (!ad_file)
            return NULL;
    }
    if (fgets(buf, allocsz, ad_file))
    {
        if (buf[0] != '\0')
        {
            char *equalbuf = calloc(allocsz, sizeof(char));
            memset(equalbuf, '=', strlen(buf) - 1);
            printf("%s%s\n", buf, equalbuf);
            if (pay_tier < BASIC)
                SetConsoleTitle(buf);

            free(equalbuf);
            return buf;
        }
    }
    else
    {
        fseek(ad_file, 0, SEEK_SET);
        free(buf);
        get_new_ad();
    }
    return NULL;
}

void SetWindow(int Width, int Height)
{
    COORD coord;
    coord.X = Width;
    coord.Y = Height;

    SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    Rect.Bottom = Height - 1;
    Rect.Right = Width - 1;

    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleWindowInfo(Handle, TRUE, &Rect);

    SetConsoleScreenBufferSize(Handle, coord);
}

void SetColor(WORD color)
{

    HANDLE hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsoleColor, color % 256);
}

int show_banner(CONSOLE_SCREEN_BUFFER_INFO csbi)
{
    COORD bufsize = {csbi.dwSize.X, csbi.dwSize.Y};
    COORD windowsize = {csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1};
    COORD cursor = {csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y};

    WORD old_color = csbi.wAttributes;
    COORD src, dest;
    SMALL_RECT rect;

    CHAR_INFO *buffer = calloc((size_t)bufsize.X * (size_t)bufsize.Y, sizeof(CHAR_INFO));

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set the source and destination positions
    src.X = 0;
    src.Y = scrollcount;

    dest.X = 0;
    dest.Y = 0;

    // Set the rectangle to cover the entire console
    rect.Left = 0;
    rect.Top = 0;
    rect.Right = windowsize.X;
    rect.Bottom = windowsize.Y;
    if (old_color % 16 == old_color / 16) // Make sure the colors aren't the same
        old_color = 7;

    // Read the block of text from the source position
    ReadConsoleOutput(console, buffer, (COORD){bufsize.X, bufsize.Y}, src, &rect);

    // Write the block of text to the destination position
    WriteConsoleOutput(console, buffer, (COORD){bufsize.X, bufsize.Y}, dest, &rect);

    int display_y = cursor.Y - scrollcount - windowsize.Y;
    if (cursor.Y < windowsize.Y - scrollcount)
    {
        display_y = 0;
    }
    gotoxy(console, 0, display_y);
    SetColor((colortable[color_count++ % 8]));
    get_new_ad();
    SetColor(old_color);
    return display_y;
}

int show_ads()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

    DWORD dwMode = 0;
    GetConsoleMode(hInput, &dwMode);
    SetConsoleMode(hInput, dwMode & (~ENABLE_PROCESSED_INPUT));

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT lpScrollRectangle;
    CHAR_INFO chiFill;
    chiFill.Char.AsciiChar = ' ';
    chiFill.Attributes = 0;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    COORD oldpos = csbi.dwCursorPosition;
    show_banner(csbi);
    oldpos.Y += scrollcount;

    int latest_banner_y = 0;
    while (1)
    {
        gotoxy(hConsole, oldpos.X, oldpos.Y);

        Sleep(1000);
        GetConsoleScreenBufferInfo(hConsole, &csbi);

        oldpos = csbi.dwCursorPosition;
        if ((oldpos.Y < 2) && pay_tier != PREMIUM)
        {
            latest_banner_y = show_banner(csbi);
            oldpos.Y += latest_banner_y + scrollcount;
        }

        gotoxy(hConsole, oldpos.X, oldpos.Y);
        if (pay_tier == PREMIUM)
        {
            return 0;
        }
    }
}

int main(int argc, char *argv[])
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char command_line[2 * MAX_PATH + 30];
    char argn[] = "--silent";
    int ad_tier_arg = 1;
    if (argc > 1)
        ad_tier_arg = atoi(argv[1]);
    SetConsoleCtrlHandler(NULL, TRUE);
    if (argc > 1 && strcmp(argv[1], argn) == 0)
    {
        if (argc > 2)
            ad_tier_arg = atoi(argv[2]);
        if (ad_tier_arg >= 1 && ad_tier_arg <= 4)
            pay_tier = ad_tier_arg;

        if (pay_tier == PREMIUM_PLUS)
            return 0;

        show_ads();
        signal(SIGINT, SIG_IGN);
        signal(SIGABRT, SIG_IGN);
    }
    else
    {
        if (argc > 2)
            ad_tier_arg = atoi(argv[2]);
        char *exec_name = calloc((MAX_PATH + 1) * 2, sizeof(TCHAR));
        GetModuleFileName(NULL, exec_name, MAX_PATH * 2);
        // If the program is started with "--silent" switch, pass it to the new process
        sprintf(command_line, "%s %s %d", exec_name, argn, ad_tier_arg);
        if (CreateProcess(NULL,
                          command_line,
                          NULL,
                          NULL,
                          FALSE,
                          0,
                          NULL,
                          NULL,
                          &si,
                          &pi))
        {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        else
        {
            fprintf(stderr, "Error creating instance (%lu).\n", GetLastError());
        }
    }

    return 0;
}
