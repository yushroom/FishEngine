#include "FileInfo.hpp"

namespace FishEditor
{
    std::map<std::string, FileInfo*> FileInfo::s_nameToNode;
    FileInfo * FileInfo::s_assetRoot = nullptr;

    enum class AssetType
    {
        Unknown,
        Texture,
        Model,
        Shader,
        Material,
        Script,
    };

    AssetType GetAssetType(Path const & ext)
    {
        //auto ext = path.extension();
        if (ext == ".jpg" || ext == ".png" || ext == ".jpeg" || ext == ".tga")
        {
            return AssetType::Texture;
        }
        else if (ext == ".obj" || ext == ".fbx")
        {
            return AssetType::Model;
        }
        else if (ext == ".shader")
        {
            return AssetType::Shader;
        }
        else if (ext == ".mat")
        {
            return AssetType::Material;
        }
        else if (ext == ".hpp" || ext == ".cpp")
        {
            return AssetType::Script;
        }
        return AssetType::Unknown;
    }


    void FileInfo::SetAssetRootPath(const std::string &path)
    {
        assert(boost::filesystem::is_directory(path));
        s_assetRoot = new FileInfo();
        s_assetRoot->m_isDirectory = true;
        s_assetRoot->m_parent = nullptr;
        s_assetRoot->m_path = path;
        //s_nameToNode[boost::filesystem::absolute(path).string()] = this;
        s_assetRoot->BuildNodeTree(path);
    }

    FileInfo* FileInfo::fileInfo(const std::string &path)
    {
        auto const & it = s_nameToNode.find(path);
        if (it == FileInfo::s_nameToNode.end())
        {
            abort();
        }
        return it->second;
    }

    void FileInfo::UpdateThumbnailImpl(FileInfo* node)
    {
        if (node->m_isDirectory)
        {
            for (auto const & child : node->m_dirChildren)
                UpdateThumbnailImpl(child);
            for (auto const & child : node->m_fileChildren)
                UpdateThumbnailImpl(child);
        }
        else
        {
            auto type = GetAssetType(node->m_path.extension());
            if (type == AssetType::Texture)
            {
                node->m_hasThumbnail = true;
            }
        }
    }

    void FileInfo::UpdateThumbnail()
    {
        UpdateThumbnailImpl(s_assetRoot);
    }


    // path must be a dir
    void FileInfo::BuildNodeTree(const Path &path)
    {
        s_nameToNode[boost::filesystem::absolute(path).string()] = this;
        for (auto& it : boost::filesystem::directory_iterator(path))
        {
            const Path & p = it.path();
            if (p.extension() == ".DS_Store" || p.extension() == ".meta")
            {
                continue;
            }

            auto fileNode = new FileInfo();
            fileNode->m_path = p;
            fileNode->m_parent = this;

            if (boost::filesystem::is_directory(p))
            {
                m_dirChildren.emplace_back(fileNode);
                fileNode->m_isDirectory = true;
                fileNode->BuildNodeTree(p);
            }
            else
            {
                m_fileChildren.emplace_back(fileNode);
                fileNode->m_isDirectory = false;
            }
        }
    }
}


