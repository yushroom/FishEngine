#pragma once

#include "FishEngine.hpp"
#include <array>

namespace FishEngine
{
    class LayerMask
    {
    public:
        explicit LayerMask(int layer);

        int value() const
        {
            return m_value;
        }

        static int GetMask(std::vector<std::string> const & layerNames)
		{
			int result = 0;
			for (auto const & name : layerNames)
			{
				for (int i = 0; i < 32; ++i)
				{
					if (s_layerNames[i] == name)
					{
						result |= i;
						break;
					}
				}
			}
			return result;
		}

        static std::string LayerToName(int layer)
		{
			if (layer < 0 || layer > 31)
				return "";
			return s_layerNames[layer];
		}

        static int NameToLayer(std::string layerName)
		{
			for (int i = 0; i < 32; ++i)
			{
				if (s_layerNames[i] == layerName)
					return i;
			}
			return -1;
		}
		
		static std::array<std::string, 32> const & allLayerNames()
		{
			return s_layerNames;
		}

    private:
        int m_value = -1;

		static std::array<std::string, 32> s_layerNames;
		
		
		friend class ::UIGameObjectHeader;
		// Editor
		static void SetLayer(int index, std::string const & name);
		
		static int s_editorFlag;	// special flag for Editor use only;
    };
}
