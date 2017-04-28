#pragma once

#include <ReflectClass.hpp>
#include "FishEditor.hpp"

struct ScriptManagerImpl;

namespace FishEditor
{
	class Meta(NonSerializable) ScriptManager
	{
	public:
		static ScriptManager & GetInstance()
		{
			static ScriptManager manager;
			return manager;
		}

		void BuildScriptsInProject();

		void Reload();

		FishEngine::ScriptPtr CreateScript(std::string const & name);

	private:
		ScriptManager();
		~ScriptManager();

		ScriptManager(ScriptManager const &) = delete;
		ScriptManager(ScriptManager &&) = delete;
		ScriptManager& operator=(ScriptManager const &) = delete;

		ScriptManagerImpl *		m_impl = nullptr;
		std::list<FishEngine::ScriptPtr>	m_scripts;
	};
}