#ifndef Debug_hpp
#define Debug_hpp

#include "FishEngine.hpp"

NAMESPACE_FISHENGINE_BEGIN

class Debug
{
public:
    // Logs message to the Console.
    static void Log(const char *format, ...);
    
    // A variant of Debug.Log that logs a warning message to the console.
    static void LogWarning(const char *format, ...);
    
    // A variant of Debug.Log that logs an error message to the console.
    static void LogError(const char *format, ...);

    static void Init();
};

NAMESPACE_FISHENGINE_END

#endif /* Debug_hpp */
