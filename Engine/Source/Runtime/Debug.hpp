#ifndef Debug_hpp
#define Debug_hpp

#include "FishEngine.hpp"

namespace FishEngine
{
    class Debug
    {
    public:
        Debug() = delete;
        
        // Logs message to the Console.
        static void Log(const char *format, ...);

        // A variant of Debug.Log that logs a warning message to the console.
        static void LogWarning(const char *format, ...);

        // A variant of Debug.Log that logs an error message to the console.
        static void LogError(const char *format, ...);
        
        static void setColorMode(bool value)
        {
            s_colorMode = value;
        }

        static void Init();
        
    private:
        static bool s_colorMode;
    };
}

#endif /* Debug_hpp */
