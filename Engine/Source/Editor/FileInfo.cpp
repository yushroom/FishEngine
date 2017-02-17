#include "FileInfo.hpp"
#include "Resources.hpp"
#include "AssetImporter.hpp"
#include "TextureImporter.hpp"
#include "AssetDataBase.hpp"

#include <QImage>
#include <QIcon>

using namespace FishEngine;

namespace FishEditor
{
    std::map<std::string, FileInfo*> FileInfo::s_nameToNode;
    FileInfo * FileInfo::s_assetRoot = nullptr;

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
            auto type = Resources::GetAssetType(node->m_path.extension());
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
#if 1
				auto ext = p.extension();
				if (Resources::GetAssetType(ext) == AssetType::Texture)
				{
					auto texture = AssetDatabase::LoadAssetAtPath<Texture>(p);
					//auto importer = AssetImporter::GetAtPath(p);
					//auto texture = AssetImporter::s_importerGuidToTexture[importer->GetGUID()];
					//auto data = texture->rawdata().data();
					//AssetDatabase::m_cacheIcons[p] = QIcon(QPixmap::fromImage(QImage(data, texture->width(), texture->height(), QImage::Format_RGB888)));
				}
				else if (ext == ".fbx" || ext == ".FBX")
				{
					auto model = AssetDatabase::LoadAssetAtPath<GameObject>(p);
				}
#endif
            }
        }
    }
}


