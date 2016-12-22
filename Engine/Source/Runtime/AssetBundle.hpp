#pragma once

#include "Object.hpp"

namespace FishEngine
{
    class AssetBundle : public Object
    {
    public:

        InjectClassName(AssetBundle)

        // Check if an AssetBundle contains a specific object.
        // Returns true if an object referred to by name is contained in the AssetBundle, false otherwise.
        bool Contains(const std::string& name);

        // Return all asset names in the AssetBundle.
        // Only works for normal AssetBundles(non - streamed scene AssetBundle), otherwise empty string array is returned.
        std::vector<std::string> GetAllAssetNames();

        // Return all the scene asset paths(paths to *.unity assets) in the AssetBundle.
        // Only works for streamed scene AssetBundles, otherwise empty string array is returned.
        std::vector<std::string> GetAllScenePaths();

        void LoadAllAssets();
    };
}