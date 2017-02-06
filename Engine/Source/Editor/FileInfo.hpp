#pragma once

#include <boost/filesystem.hpp>
#include <map>
#include <array>
#include <Resources.hpp>

//typedef boost::filesystem::path Path;
using FishEngine::Path;

namespace FishEditor
{
    class FileInfo
    {
    public:
        FileInfo() = default;
        //FileNode(FileNode &&) = default;
        FileInfo(FileInfo const &) = delete;
        FileInfo & operator=(FileInfo const &) = delete;

        bool isDir() const { return m_isDirectory; }

        Path absoluteFilePath() const
        {
            return boost::filesystem::absolute(m_path);
        }

        std::string stem() const
        {
            if ( !m_path.has_stem() )
                return m_path.filename().string();
            return m_path.stem().string();
        }

        std::string fileName() const
        {
            return m_path.filename().string();
        }

        int childCount() const
        {
            return m_dirChildren.size() + m_fileChildren.size();
        }

        int subDirCount() const
        {
            return m_dirChildren.size();
        }
        
        const Path & path() const
        {
            return m_path;
        }
        
        FileInfo * parent() const
        {
            return m_parent;
        }

        FileInfo * childAt(int index) const
        {
            assert(index >= 0 && index < childCount());
            if (index < m_dirChildren.size())
                return m_dirChildren[index];
            else
                return m_fileChildren[index - m_dirChildren.size()];
        }

        FileInfo * subDirAt(int index) const
        {
            assert(index >= 0 && index < m_dirChildren.size());
            return m_dirChildren[index];
        }
        
        std::vector<FileInfo*> const & subDirs() const
        {
            return m_dirChildren;
        }

        static void SetAssetRootPath(std::string const & path);
        static FileInfo* assetRoot() { return s_assetRoot; }
        static FileInfo* fileInfo(std::string const & path);
        static void UpdateThumbnail();

    private:
        void BuildNodeTree(const Path & m_path);

        Path                    m_path;
        FileInfo*               m_parent = nullptr;
        bool                    m_isDirectory = false;
        std::vector<FileInfo*>  m_dirChildren;
        std::vector<FileInfo*>  m_fileChildren;

        bool                    m_hasThumbnail = false;
        //std::array<std::uint8_t, 128*128*4> m_thumbnail;
        std::vector<std::uint8_t> m_thumbnail;

        static FileInfo *                       s_assetRoot;
        static std::map<std::string, FileInfo*> s_nameToNode;
        static void UpdateThumbnailImpl(FileInfo *node);
    };
}
