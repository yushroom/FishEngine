#pragma once

#include <map>
#include <array>
#include <Resources.hpp>
#include <ReflectClass.hpp>

#include <Path.hpp>

using FishEngine::Path;
class ProjectViewFileModel;

namespace FishEditor
{
	class Meta(NonSerializable) FileInfo
	{
	public:
		FileInfo() = default;
		//FileNode(FileNode &&) = default;
		FileInfo(FileInfo const &) = delete;
		FileInfo & operator=(FileInfo const &) = delete;

		bool isDir() const { return m_isDirectory; }

		const Path & path() const { return m_path; }

		FileInfo * parent() const { return m_parent; }

		Path absoluteFilePath() const
		{
			return boost::filesystem::absolute(m_path).make_preferred();
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

		std::size_t childCount() const
		{
			return m_dirChildren.size() + m_fileChildren.size();
		}


		std::size_t subDirCount() const
		{
			return m_dirChildren.size();
		}
		

		FileInfo * childAt(int index) const
		{
			assert(index >= 0 && index < childCount());
			if (index < m_dirChildren.size())
				return m_dirChildren[index];
			else
				return m_fileChildren[index - m_dirChildren.size()];
		}

		FileInfo * CreateNewSubDir(const std::string & folderName);

		FileInfo * subDirAt(int index) const
		{
			assert(index >= 0 && index < m_dirChildren.size());
			return m_dirChildren[index];
		}

		int GetChildIndex(FileInfo * child) const;

		// newName: remove parent path and extension
		void Rename(const std::string& newName);

		static void SetAssetRootPath(Path const & path);
		static FileInfo* assetRoot() { return s_assetRoot; }
		static FileInfo* fileInfo(std::string const & path);
		static void UpdateThumbnail();

		// delete the file
		bool DeleteFile();

	private:
		friend class ::ProjectViewFileModel;
		void BuildNodeTree(const Path & path, std::vector<Path> & modelPaths);
		static void AddFileNode(const Path & path);

		Path                    m_path;
		FileInfo*               m_parent = nullptr;
		bool                    m_isDirectory = false;
		std::vector<FileInfo*>  m_dirChildren;
		std::vector<FileInfo*>  m_fileChildren;

		bool                    m_hasThumbnail = false;
		//std::array<std::uint8_t, 128*128*4> m_thumbnail;
		//std::vector<std::uint8_t> m_thumbnail;

		bool m_fileExists = true; // this value is false when file not created
		bool Create();

		// simply remove from children, do NOT delete real file
		void RemoveChild(FileInfo* fileInfo);

		static FileInfo *                       s_assetRoot;
		static std::map<std::string, FileInfo*> s_nameToNode;
		static void UpdateThumbnailImpl(FileInfo *node);
	};
}
