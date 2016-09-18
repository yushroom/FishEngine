#include "Debug.hpp"
#include <cstdio>
#include <cstdarg>
#include <iostream>

#if FISHENGINE_PLATFORM_WINDOWS
#include <windows.h>
static HANDLE hstdout;
#define vsprintf vsprintf_s
#elif FISHENGINE_PLATFORM_APPL
#else
#endif

#define SPRINT_BUF_SIZE 1024
static char sprint_buf[SPRINT_BUF_SIZE];

using std::cout;

NAMESPACE_FISHENGINE_BEGIN

void Debug::Init()
{
#ifdef FISHENGINE_PLATFORM_WINDOWS
    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    FILE* dont_care;
    freopen_s(&dont_care, "CON", "w", stdout);
    //freopen("CON", "w", stdout);
    
    //HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hstdout, &csbi);
#endif
}


void Debug::Log(const char *fmt, ...)
{
#if FISHENGINE_PLATFORM_WINDOWS
    SetConsoleTextAttribute(hstdout, 0x0F);
#elif FISHENGINE_PLATFORM_APPLE || FISHENGINE_PLATFORM_LINUX
    // http://stackoverflow.com/questions/9005769/any-way-to-print-in-color-with-nslog#
    // https://wiki.archlinux.org/index.php/Color_Bash_Prompt#List_of_colors_for_prompt_and_Bash
    
    printf("\e[0;37m");    // white
#endif
    std::cout << "[info] ";
    va_list args;
    va_start(args, fmt);
    int n = vsprintf(sprint_buf, fmt, args);
    va_end(args);
    std::cout.write(sprint_buf, n);
    std::cout << std::endl;
#if FISHENGINE_PLATFORM_APPLE || FISHENGINE_PLATFORM_LINUX
    printf("\e[m");
#endif
}

void Debug::LogWarning(const char *fmt, ...)
{
#if FISHENGINE_PLATFORM_WINDOWS
    SetConsoleTextAttribute(hstdout, 0x0E);
#elif FISHENGINE_PLATFORM_APPLE || FISHENGINE_PLATFORM_LINUX
    printf("\e[0;33m");    // yellow
#endif
    std::cout << "[warning] ";
    va_list args;
    va_start(args, fmt);
    int n = vsprintf(sprint_buf, fmt, args);
    va_end(args);
    std::cout.write(sprint_buf, n);
    std::cout << std::endl;
#if FISHENGINE_PLATFORM_APPLE || FISHENGINE_PLATFORM_LINUX
    printf("\e[m");
#endif
}

void Debug::LogError(const char *fmt, ...)
{
#if FISHENGINE_PLATFORM_WINDOWS
    SetConsoleTextAttribute(hstdout, 0x0C);
#elif FISHENGINE_PLATFORM_APPLE || FISHENGINE_PLATFORM_LINUX
    printf("\e[0;31m");    // red
#endif
    std::cout << "[error] ";
    va_list args;
    va_start(args, fmt);
    int n = vsprintf(sprint_buf, fmt, args);
    va_end(args);
    std::cout.write(sprint_buf, n);
    std::cout << std::endl;
#if FISHENGINE_PLATFORM_APPLE || FISHENGINE_PLATFORM_LINUX
    printf("\e[m");
#endif
}

NAMESPACE_FISHENGINE_END

#undef vsprintf_s
