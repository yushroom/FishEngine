#include "LayerMask.hpp"

namespace FishEngine
{
	std::array<std::string, 32> LayerMask::s_layerNames = { {"Default", "TransparentFX", "Ignore RayCast", "", "Water", "UI"}};
	
	int LayerMask::s_editorFlag = 0;
	
	void LayerMask::SetLayer(int index, std::string const & name)
	{
		s_layerNames[index] = name;
		s_editorFlag++;
	}
}
