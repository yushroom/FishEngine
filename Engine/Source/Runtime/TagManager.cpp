#include "TagManager.hpp"

std::vector<std::string> FishEngine::TagManager::s_tags = {
	"Untagged",
	"Respawn",
	"Finish",
	"EditorOnly",
	"MainCamera",
	"Player",
	"CameraController",
};

int FishEngine::TagManager::s_editorFlag = 0;
