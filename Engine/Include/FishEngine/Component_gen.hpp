#ifndef Component_gen_hpp
#define Component_gen_hpp

#include "FishEngine.hpp"

namespace FishEngine
{
	FE_EXPORT ComponentPtr
	AddComponentToGameObject( std::string const & componentClassName, GameObjectPtr const & gameObject);
}

#endif // Component_gen_hpp