#include "Debug.hpp"
#include <iostream>

#if FISHENGINE_PLATFORM_WINDOWS
#include <windows.h>
static HANDLE hstdout;
#endif


using std::cout;

namespace FishEngine
{
	bool Debug::s_colorMode = true;
	
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

#if 0
	void  Debug::_Log(const std::string & message)
	//void Debug::Log(const char *fmt, ...)
	{
		if (s_colorMode)
		{
#if FISHENGINE_PLATFORM_WINDOWS
			SetConsoleTextAttribute(hstdout, 0x0F);
#elif FISHENGINE_PLATFORM_APPLE || FISHENGINE_PLATFORM_LINUX
			// http://stackoverflow.com/questions/9005769/any-way-to-print-in-color-with-nslog#
			// https://wiki.archlinux.org/index.php/Color_Bash_Prompt#List_of_colors_for_prompt_and_Bash

			printf("\e[0;37m");    // white
#endif
		}

		std::cout << message;
		std::cout << std::endl;

#if FISHENGINE_PLATFORM_APPLE || FISHENGINE_PLATFORM_LINUX
		if (s_colorMode)
		{
			printf("\e[m");
		}
#endif
	}

	void Debug::_LogWarning(const std::string & message)
	{
		if (s_colorMode)
		{

#if FISHENGINE_PLATFORM_WINDOWS
			SetConsoleTextAttribute(hstdout, 0x0E);
#elif FISHENGINE_PLATFORM_APPLE || FISHENGINE_PLATFORM_LINUX
			printf("\e[0;33m");    // yellow
#endif
		}
		
		std::cout << message;
		std::cout << std::endl;

#if FISHENGINE_PLATFORM_APPLE || FISHENGINE_PLATFORM_LINUX
		if (s_colorMode)
		{
			printf("\e[m");
		}
#endif
	}

	void Debug::_LogError(const std::string & message)
	{
		if (s_colorMode)
		{
#if FISHENGINE_PLATFORM_WINDOWS
			SetConsoleTextAttribute(hstdout, 0x0C);
#elif FISHENGINE_PLATFORM_APPLE || FISHENGINE_PLATFORM_LINUX
			printf("\e[0;31m");    // red
#endif
		}
		std::cout << message;
		std::cout << std::endl;
#if FISHENGINE_PLATFORM_APPLE || FISHENGINE_PLATFORM_LINUX
		if (s_colorMode)
		{
			printf("\e[m");
		}
#endif
	}
#endif
}

void FishEngine::Debug::Log(LogChannel channel, std::string const & message, const char* file, int line, const char * func)
{
#if FISHENGINE_PLATFORM_WINDOWS
	WORD colorAttr = 0x0F;
	switch (channel)
	{
	case LogChannel::Warning:
		colorAttr = 0x0E;
		break;
	case LogChannel::Error:
		colorAttr = 0x0C;
		break;
	}
	SetConsoleTextAttribute(hstdout, colorAttr);
#elif FISHENGINE_PLATFORM_APPLE || FISHENGINE_PLATFORM_LINUX
	// http://stackoverflow.com/questions/9005769/any-way-to-print-in-color-with-nslog#
	// https://wiki.archlinux.org/index.php/Color_Bash_Prompt#List_of_colors_for_prompt_and_Bash

	if (channel == LogChannel::Info)
	{
		printf("\e[0;37m");    // white
	}
	else if (channel == LogChannel::Warning)
	{
		printf("\e[0;33m");    // yellow
	}
	else
	{
		printf("\e[0;31m");    // red
	}
#endif

	std::cout << "[" << file << ":" << line << ", " << func << "] ";
	std::cout << message;
	std::cout << std::endl;

#if FISHENGINE_PLATFORM_APPLE || FISHENGINE_PLATFORM_LINUX
	if (s_colorMode)
	{
		printf("\e[m");
	}
#endif
	}