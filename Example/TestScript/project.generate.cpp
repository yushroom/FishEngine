#include <iostream>
#include <boost/config.hpp> // for BOOST_SYMBOL_EXPORT
//#include <boost/dll/alias.hpp> // for BOOST_DLL_ALIAS

#include "Assets/Rotator.hpp" 

// `extern "C"` - specifies C linkage: forces the compiler to export function/variable by a pretty (unmangled) C name.
#define API extern "C" BOOST_SYMBOL_EXPORT

using RotatorPtr = std::shared_ptr<Rotator>;

API FishEngine::Script* CreateCustomScript(const char* className)
{
	std::cout << className << std::endl;
	if (std::string(className) == "Rotator")
	{
		return new Rotator();
	}
	return nullptr;
}

API void DestroyCustomScript(FishEngine::Script * script)
{
	delete script;
}
