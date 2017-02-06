#include "ReflectClass.hpp"
#include <map>

#include "generate/Class_ComponentInfo.hpp"

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
	
	bool IsDerivedFrom(int derivedClassID, int baseClassID)
	{
		int name = derivedClassID;
		do {
			if (name == baseClassID)
			{
				return true;
			}
			name = s_componentInheritance_int[name];
		} while (name > 0);
		return false;
	}
}
