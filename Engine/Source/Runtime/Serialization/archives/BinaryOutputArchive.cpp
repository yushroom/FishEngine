#include "BinaryOutputArchive.hpp"

#include "../../Object.hpp"

namespace FishEngine
{
	BinaryOutputArchive & FishEngine::BinaryOutputArchive::operator<<(ObjectPtr const & object)
	{
		object->Serialize(*this);
		return *this;
	}
}
