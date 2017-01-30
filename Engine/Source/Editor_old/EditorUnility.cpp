#include "EditorUnility.hpp"
#include <nfd.h>
#include <Debug.hpp>
#include <Resources.hpp>
#include <boost/algorithm/string.hpp>

using namespace FishEngine;

namespace FishEditor
{
    std::string EditorUtility::OpenFilePanel(const std::string& title, const std::string& directory, const std::string& extension)
    {
        std::string result_path;
        const char* dir;
        if (directory.size() == 0)
        {
            dir = directory.c_str();
        }
        nfdchar_t *out_path = nullptr;
        nfdresult_t result = NFD_OpenDialog(extension.c_str(), dir, &out_path);

        if (result == NFD_OKAY)
        {
            Debug::Log("EditorUtility::OpenFilePanel Success!");
            Debug::Log("EditorUtility::OpenFilePanel Path: %s", out_path);
            result_path = std::string(out_path);
            free(out_path);
        }
        else if (result == NFD_CANCEL)
        {
            Debug::Log("EditorUtility::OpenFilePanel canceled");
        }
        else {
            Debug::LogError("EditorUtility::OpenFilePanel Error: %s\n", NFD_GetError());
        }

        return result_path;
    }

    std::string EditorUtility::SaveFilePanel(const std::string& title, const std::string& directory, const std::string& defaultName, const std::string& extension)
    {
        std::string result_path;
        Path default_path = boost::filesystem::path(directory) / defaultName;
        if (!boost::ends_with(defaultName, "." + extension))
        {
            default_path /= extension;
        }
        
        nfdchar_t *out_path = nullptr;
        nfdresult_t result = NFD_SaveDialog(extension.c_str(), default_path.string().c_str(), &out_path);

        if (result == NFD_OKAY)
        {
            Debug::Log("EditorUtility::SaveFilePanel Success!");
            Debug::Log("EditorUtility::SaveFilePanel Path: %s", out_path);
            result_path = std::string(out_path);
            free(out_path);
        }
        else if (result == NFD_CANCEL)
        {
            Debug::Log("EditorUtility::SaveFilePanel canceled");
        }
        else
        {
            Debug::LogError("EditorUtility::SaveFilePanel Error: %s\n", NFD_GetError());
        }
        return result_path;
    }
}
