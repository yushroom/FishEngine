#ifndef Component_gen_hpp
#define Component_gen_hpp

#include "FishEngine.hpp"

namespace FishEngine
{
    FE_EXPORT ComponentPtr
    AddComponentToGameObject(
        const std::string&  componentClassName,
        GameObjectPtr&      gameObject);
}

#endif // Component_gen_hpp