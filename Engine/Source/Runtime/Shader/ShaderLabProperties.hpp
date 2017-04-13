#pragma once

#include <string>
#include <map>
#include <vector>
#include <utility>
#include "../Vector4.hpp"
#include "../Color.hpp"

namespace FishEngine
{
	struct ShaderLabRangeProperty
	{
		ShaderLabRangeProperty() = default;

		ShaderLabRangeProperty(float value, float minValue, float maxValue)
			: value(value), minValue(minValue), maxValue(maxValue)
		{

		}

		float value = 0;
		float minValue = 0;
		float maxValue = 1;
	};


	struct Meta(NonSerializable) ShaderLabProperties
	{
	public:

		ShaderLabProperties() = default;

		// displayName and name
		std::vector<std::pair<std::string, std::string>> m_names;
		std::map<std::string, float> m_Floats;
		std::map<std::string, ShaderLabRangeProperty> m_Ranges;
		std::map<std::string, Color> m_Colors;
		std::map<std::string, Vector4> m_Vectors;
		std::map<std::string, TexturePtr> m_TexEnvs;

		void AddProperty(
			std::string const & name,
			std::string const & displayName,
			std::string const & type,
			std::string const & defalutValueString);
	};
}