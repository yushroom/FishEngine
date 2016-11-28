#include "Resources.hpp"
#include "Debug.hpp"
#include <iostream>

namespace FishEngine
{
    //Path::Path(std::string path)
    //    : m_pathStr(path)
    //{

    //}


    void Resources::Init()
    {
        auto cwd = boost::filesystem::current_path();
        bool found;
        do
        {
            //std::cout << cwd << std::endl;
            found = true;
            for (auto& d : { "Assets/Models", "Assets/Textures", "Assets/Fonts", "Engine/Shaders" })
            {
                auto test = cwd / d;
                if (!boost::filesystem::exists(test) || !boost::filesystem::is_directory(test))
                {
                    found = false;
                    break;
                }
            }
            if (found)
                break;
            cwd = cwd.parent_path();
        } while (cwd.has_parent_path());

        if (!found)
        {
            Debug::LogError("Can't set root dir.");
            abort();
        }

        s_rootSystemDirectory = cwd;
    }

    Path Resources::s_assetsDirectory;
    Path Resources::s_rootSystemDirectory;
}
