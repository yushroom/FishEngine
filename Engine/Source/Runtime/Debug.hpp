#ifndef Debug_hpp
#define Debug_hpp

#include "FishEngine.hpp"
#include "ReflectClass.hpp"
#include "StringFormat.hpp"

namespace FishEngine
{
	enum class LogChannel
	{
		Info,
		Warning,
		Error
	};

	class FE_EXPORT Meta(NonSerializable) Debug
	{
	public:
		Debug() = delete;
		
		// Logs message to the Console.
		//static void Log(const char *format, ...);
		//static void _Log(const std::string & message);

		// A variant of Debug.Log that logs a warning message to the console.
		//static void LogWarning(const char *format, ...);
		//static void _LogWarning(const std::string & message);

		// A variant of Debug.Log that logs an error message to the console.
		//static void LogError(const char *format, ...);
		//static void _LogError(const std::string & message);

		static void Log(LogChannel channel, std::string const & message, const char* file, int line, const char * func);
		
		static void setColorMode(bool value)
		{
			s_colorMode = value;
		}

		static void Init();
		
	private:
		static bool s_colorMode;
	};
}

#if defined(_MSC_VER)
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#define LogInfo(message) FishEngine::Debug::Log(FishEngine::LogChannel::Info, (message), __FILE__, __LINE__, __PRETTY_FUNCTION__)
#define LogWarning(message) FishEngine::Debug::Log(FishEngine::LogChannel::Warning, (message), __FILE__, __LINE__, __PRETTY_FUNCTION__)
#define LogError(message) FishEngine::Debug::Log(FishEngine::LogChannel::Error, (message), __FILE__, __LINE__, __PRETTY_FUNCTION__)

#endif /* Debug_hpp */
