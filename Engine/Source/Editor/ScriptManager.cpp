#include "ScriptManager.hpp"
#include <memory>

#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif
#include <boost/dll/import.hpp>

#include <Application.hpp>
#include <Debug.hpp>


using namespace FishEngine;
using namespace FishEditor;

struct ScriptManagerImpl
{
	Path sharedLibPath;

	boost::dll::detail::library_function<Script*(const char*)> createFunc;
	boost::dll::detail::library_function<void(Script *)> deleteFunc;

	ScriptManagerImpl(Path const & path)
		: sharedLibPath(path),
		createFunc(boost::dll::import<Script*(const char*)>(path, "CreateCustomScript")),
		deleteFunc(boost::dll::import<void(Script*)>(path, "DestroyCustomScript"))
	{
	}
};


ScriptManager::ScriptManager()
{
	BuildScriptsInProject();
	auto project_name = Application::dataPath().parent_path().stem().string();
#if FISHENGINE_PLATFORM_WINDOWS
	auto path = Format("../build/%1%/%2%.dll", CMAKE_INTDIR, project_name);
#else
	auto path = Format("../build/%1%/lib%2%.dylib", CMAKE_INTDIR, project_name);
#endif
	auto sharedLibPath = Application::dataPath() / path;
	if (!boost::filesystem::exists(sharedLibPath))
	{
		LogError(sharedLibPath.string() + " not found");
		abort();
	}
	m_impl = new ScriptManagerImpl(sharedLibPath);
}

ScriptManager::~ScriptManager()
{
	delete m_impl;
}

ScriptPtr ScriptManager::CreateScript(std::string const & name)
{
	auto s = std::shared_ptr<Script>(m_impl->createFunc(name.c_str()), [this](auto s) {
		LogInfo("Delete from dll");
		m_impl->deleteFunc(s);
	});
	m_scripts.push_back(s);
	return s;
}

void FishEditor::ScriptManager::BuildScriptsInProject()
{
	try {
		Py_Initialize();
		//PyRun_SimpleString("print('Hello Python')\n");
		auto pName = PyUnicode_DecodeFSDefault("BuildProject");
		auto pModule = PyImport_Import(pName);
		auto pFunc = PyObject_GetAttrString(pModule, "buildProject");
		if (!pFunc || !PyCallable_Check(pFunc))
			abort();
		auto build_path = Application::dataPath().parent_path();
		auto pbuildPath = PyUnicode_DecodeFSDefault(build_path.string().c_str());
		auto pbuildType = PyUnicode_DecodeFSDefault(CMAKE_INTDIR);
		auto pArgs = PyTuple_New(2);
		PyTuple_SetItem(pArgs, 0, pbuildPath);
		PyTuple_SetItem(pArgs, 1, pbuildType);
		PyObject_CallObject(pFunc, pArgs);
		//Py_DECREF(s);
		//Py_DECREF(pArgs);
		//Py_DECREF(pFunc);
		//Py_DECREF(pModule);
		//Py_DECREF(pName);
		Py_Finalize();
	}
	catch (std::exception & e)
	{
		LogError(e.what());
		abort();
	}
}
