#include "OpenProjectDialog.hpp"

#include <FishEngine/Application.hpp>

void OpenProjectDialog::Set(const char* path)
{
	FishEngine::Application::s_dataPath = path;
}

