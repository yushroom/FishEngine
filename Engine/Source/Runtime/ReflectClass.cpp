#include "ReflectClass.hpp"
#include <map>

#include "generate/Class_ConponentInfo.hpp"

namespace FishEngine
{
    bool IsDerivedFrom(const std::string& className, const std::string& baseClassName)
    {
        std::string name = className;
        do {
            if (name == baseClassName)
            {
                return true;
            }
            name = s_componentInheritance[name];
        } while (name.size() > 0);
        return false;
    }
}
