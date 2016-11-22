#ifndef Component_gen_hpp
#define Component_gen_hpp

#include "FishEngine.hpp"

namespace FishEngine
{
    template<typename T>
    constexpr bool IsUniqueComponent()
    {
        static_assert(std::is_base_of<Component, T>::value, "Component only");
        return false;
    }

    template<>
    constexpr bool IsUniqueComponent<Rigidbody>()
    {
        return true;
    }

    template<>
    constexpr bool IsUniqueComponent<Animator>()
    {
        return true;
    }

    FE_EXPORT ComponentPtr
    AddComponentToGameObject(
        const std::string&  componentClassName,
        GameObjectPtr&      gameObject);
}

#endif // Component_gen_hpp