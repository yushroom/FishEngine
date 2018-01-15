#include "MainWindow.hpp"

#include <FishEngine/Application.hpp>
#include <FishEngine/Debug.hpp>
#include <FishEngine/Path.hpp>
#include <FishEngine/Shader.hpp>
#include <FishEngine/ShaderCompiler.hpp>

#include <FishGUI/Utils.hpp>
#include "EditorApplication.hpp"
#include "FileInfo.hpp"

void MainWindow::Init()
{
	FishEngine::Debug::Init();
	FishEngine::Application::s_isEditor = true;
	auto cwd = ApplicationFilePath();
#if FISHENGINE_PLATFORM_APPLE
	cwd = FishEngine::Path(cwd).parent_path().string();
#endif
	auto & editorPath = FishEditor::EditorApplication::m_applicationPath;
	editorPath = FishEngine::Path(cwd);
	
	//s_rootSystemDirectory = cwd.absolutePath().toStdString();
	auto shaderRoot = editorPath / "shaders";
	auto shaderIncludeDir = shaderRoot / "include";
	//auto p = FishEngine::Path(cwd.absolutePath().toStdString()) / "shaders" / "include";
	FishEngine::ShaderCompiler::setShaderIncludeDir(shaderIncludeDir.string());
	//ShaderCompiler::s_shaderIncludeDir = shaderRootDirectory() / "include";
	
	FishEngine::Shader::Init(shaderRoot.string());
	
	//FishEngine::Timer t("Load assets");
	FishEditor::FileInfo::SetAssetRootPath(FishEngine::Application::s_dataPath);
	//t.StopAndPrint();
}

