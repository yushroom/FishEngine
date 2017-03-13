#pragma once

#include "ReflectClass.hpp"
#include <Resources.hpp>

class MainWindow;

namespace FishEditor
{
	class Meta(NonSerializable) EditorResources
	{
	public:
		static FishEngine::Path shaderRootDirectory()
		{
			return s_rootSystemDirectory / "Shaders";
		}

		static FishEngine::Path shaderHeaderDirectory()
		{
			return s_rootSystemDirectory / "Shaders/include";
		}

		static FishEngine::Path textureRootDirectory()
		{
			return s_rootSystemDirectory / "Resources/Textures";
		}

		static FishEngine::Path modelRootDirectory()
		{
			return s_rootSystemDirectory / "Resources/Models";
		}
		
		static void Init();

		static FishEngine::ShaderPtr builtinShader(const std::string& name);

		static FishEngine::MaterialPtr builtinMaterial(const std::string& name);

		static FishEngine::MaterialPtr InstantiateBuiltinMaterial(const std::string& name);

		// static Path fontRootDirectory()
		// {
		//     return s_rootSystemDirectory / "Resources/Fonts";
		// }

	private:
		friend class ::MainWindow;

		static FishEngine::Path s_rootSystemDirectory;

		static std::map<std::string, FishEngine::ShaderPtr> s_builtinShaders;
		static std::map<std::string, FishEngine::MaterialPtr>   s_builtinMaterialInstance;
	};
}
