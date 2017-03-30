#include "EditorResources.hpp"
#include <Shader.hpp>
#include <Debug.hpp>
#include <Material.hpp>
#include <Mesh.hpp>
#include <ShaderCompiler.hpp>

#include <QDir>
#include <QCoreApplication>

using namespace FishEditor;
using namespace FishEngine;

Path EditorResources::s_rootSystemDirectory;
std::map<std::string, ShaderPtr> EditorResources::s_builtinShaders;
std::map<std::string, MaterialPtr> EditorResources::s_builtinMaterialInstance;

void EditorResources::Init()
{
	QDir cwd = QCoreApplication::applicationDirPath();
#if FISHENGINE_PLATFORM_APPLE
	cwd.cdUp();
#endif
	s_rootSystemDirectory = cwd.absolutePath().toStdString();

//	ShaderCompiler::s_shaderIncludeDir = shaderRootDirectory() / "include";
//	Shader::Init(shaderRootDirectory().string());
	Mesh::Init(modelRootDirectory().string());

	const auto& root_dir = shaderRootDirectory();
  //  for (auto& n : { "PBR", "PBR-Reference", "Diffuse", "DebugCSM"})
  //  {
		//s_builtinShaders[n] = Shader::CreateFromFile(root_dir / (std::string(n) + ".surf"));
  //      s_builtinShaders[n]->setName(n);
  //  }

  //  for (auto& n : { "ScreenTexture", "Deferred", "CascadedShadowMap",
  //      "DisplayCSM", "DrawQuad", "GatherScreenSpaceShadow", "SolidColor",
  //      "PostProcessShadow", "PostProcessGaussianBlur", "PostProcessSelectionOutline" })
  //  {
		//s_builtinShaders[n] = Shader::CreateFromFile(root_dir / (std::string(n) + ".shader"));
  //      s_builtinShaders[n]->setName(n);
  //  }

	//s_builtinShaders["SkyboxCubed"]         = Shader::CreateFromFile(root_dir / "Skybox-Cubed.shader");
	//s_builtinShaders["SkyboxProcedural"]    = Shader::CreateFromFile(root_dir / "Skybox-Procedural.shader");
	s_builtinShaders["SolidColor-Internal"] = Shader::CreateFromFile(root_dir / "Editor/SolidColor.shader");
	s_builtinShaders["Alpha-Internal"]      = Shader::CreateFromFile(root_dir / "Editor/Alpha.shader");
	s_builtinShaders["VertexLit-Internal"]  = Shader::CreateFromFile(root_dir / "Editor/VertexLit.shader");
	//s_builtinShaders["SkyboxProcedural"]->setName("SkyboxProcedural");
	//s_builtinShaders["SkyboxCubed"]->setName("SkyboxCubed");
	s_builtinShaders["SolidColor-Internal"]->setName("SolidColor-Internal");
	s_builtinShaders["Alpha-Internal"]->setName("Alpha-Internal");
	s_builtinShaders["VertexLit-Internal"]->setName("VertexLit-Internal");
}

ShaderPtr EditorResources::builtinShader(const std::string& name)
{
	auto it = s_builtinShaders.find(name);
	if (it != s_builtinShaders.end())
	{
		return it->second;
	}
	Debug::LogWarning("No built-in shader called %s", name.c_str());
	abort();
	return nullptr;
}

FishEngine::MaterialPtr EditorResources::builtinMaterial(const std::string& name)
{
	auto it = s_builtinMaterialInstance.find(name);
	if (it != s_builtinMaterialInstance.end())
	{
		return it->second;
	}
	auto mtl = InstantiateBuiltinMaterial(name);
	s_builtinMaterialInstance[name] = mtl;
	return mtl;
}

MaterialPtr EditorResources::InstantiateBuiltinMaterial(const std::string& name)
{
	Debug::Log("Material::InstantiateBuiltinMaterial, %s", name.c_str());
	auto shader = builtinShader(name);
	assert(shader != nullptr);
	auto material = Material::CreateMaterial();
	material->setName(name);
	material->setShader(shader);
	return material;
}
