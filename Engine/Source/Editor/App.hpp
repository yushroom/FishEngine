#ifndef App_hpp
#define App_hpp

#include <ReflectClass.hpp>

namespace FishEditor
{
    class Meta(NonSerializable) App
    {
    public:
        virtual ~App() = 0;
        virtual void Init() = 0;
        //virtual void Update() = 0;
        //virtual void Clean() = 0;
    };
}

#endif // App_hpp
