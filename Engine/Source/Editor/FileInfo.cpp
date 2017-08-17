#include "FileInfo.hpp"
#include "AssetImporter.hpp"
#include "TextureImporter.hpp"
#include "AssetDataBase.hpp"

#include <FishEngine/GameObject.hpp>
#include <FishEngine/Resources.hpp>
#include <FishEngine/Application.hpp>
#include <FishEngine/Debug.hpp>
#include <FishEngine/Prefab.hpp>

#include <QImage>
#include <QIcon>
#include <QDir>

#include <boost/lexical_cast.hpp>

using namespace FishEngine;

namespace FishEditor
{
	std::map<std::string, FileInfo*> FileInfo::s_nameToNode;

	bool FileInfo::Create()
	{
		if (m_fileExists || !m_isDirectory)
		{
			abort();
		}

		if (m_isDirectory)
		{
			//boost::system::error_code error;
			s_nameToNode[boost::filesystem::absolute(m_path).make_preferred().string()] = this;
			//return boost::filesystem::create_directory(m_path);
			return QDir().mkpath(QString::fromStdString(m_path.string()));
		}
		return false;
	}

	void FileInfo::RemoveChild(FileInfo* fileInfo)
	{
		if (fileInfo->isDir())
		{
			std::remove(m_dirChildren.begin(), m_dirChildren.end(), fileInfo);
		}
		else
		{
			std::remove(m_fileChildren.begin(), m_fileChildren.end(), fileInfo);
		}
	}

	FileInfo * FileInfo::s_assetRoot = nullptr;

	FileInfo * FileInfo::CreateNewSubDir(const std::string & folderName)
	{
		std::string name = folderName;
		int sufix = 1;
		bool ok = false;
		while (!ok)
		{
			ok = true;
			for (auto const & d : m_dirChildren)
			{
				if (d->m_path.filename() == name)
				{
					name = folderName + " " + boost::lexical_cast<std::string>(sufix);
					sufix++;
					ok = false;
					break;
				}
			}
		}

		// TODO: delete
		auto fileInfo = new FileInfo;
		m_dirChildren.push_back(fileInfo);
		fileInfo->m_path = m_path / name;
		fileInfo->m_fileExists = false;
		fileInfo->m_parent = this;

		return fileInfo;
	}

	int FileInfo::GetChildIndex(FileInfo * child) const
	{
		int idx = -1;
		if (child->m_isDirectory)
		{
			idx = std::distance(m_dirChildren.begin(), std::find(m_dirChildren.begin(), m_dirChildren.end(), child));
		}
		else
		{
			idx = static_cast<int>( m_dirChildren.size() ) + std::distance(m_fileChildren.begin(), std::find(m_fileChildren.begin(), m_fileChildren.end(), child));
		}
		if (idx < 0)
		{
			abort();
		}
		return static_cast<int>(idx);
	}

	void FileInfo::Rename(const std::string& newName)
	{
		auto newPath = m_path.parent_path() / (newName + m_path.extension().string());
		if (!m_fileExists)
		{
			auto old_path = m_path;
			m_path = newPath;
			bool ok = Create();
			if (!ok)
			{
				m_path = old_path;
				ok = Create();
				if (!ok)
				{
					abort();
				}
			}
			return;
		}
		boost::system::error_code error;
		boost::filesystem::rename(m_path, newPath, error);
		if (error)
		{
			//Debug::LogWarning("FileInfo::Rename error");
			abort();
		}
		else
		{
			m_path = newPath;
		}
	}

	void FileInfo::SetAssetRootPath(const Path &path)
	{
		assert(boost::filesystem::is_directory(path));
		s_assetRoot = new FileInfo();
		s_assetRoot->m_isDirectory = true;
		s_assetRoot->m_parent = nullptr;
		s_assetRoot->m_path = path;
		//s_nameToNode[boost::filesystem::absolute(path).string()] = this;
		
		std::vector<Path> modelPaths;
		s_assetRoot->BuildNodeTree(path, modelPaths);
		
		
		for (auto & p : modelPaths)
		{
			AddFileNode(p);
		}
	}

	FileInfo* FileInfo::fileInfo(const std::string &path)
	{
		FishEngine::Path p = path;
		auto const & it = s_nameToNode.find(p.make_preferred().string());
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


	bool FileInfo::DeleteFile()
	{
		m_fileExists = false;

		//boost::system::error_code error;
		//boost::filesystem::remove(m_path, error);
		//if (error)
		//{
		//	abort();
		//	return false;
		//}
		QString path = QString::fromStdString(m_path.string());
		const bool success = m_isDirectory ?  QDir(path).removeRecursively() : QFile::remove(path);
		if (success)
		{
			// remove from parent
			if (m_parent != nullptr)
			{
				m_parent->RemoveChild(this);
			}
		}
		else
		{
			abort();
		}

		return true;
	}

	// path must be a dir
	void FileInfo::BuildNodeTree(const Path & path, std::vector<Path> & modelPaths)
	{
		s_nameToNode[boost::filesystem::absolute(path).make_preferred().string()] = this;
		
		// models should be imported after all materials and texutures
		//std::vector<Path> modelPaths;
		
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
				fileNode->BuildNodeTree(p, modelPaths);
			}
			else
			{
				m_fileChildren.emplace_back(fileNode);
				fileNode->m_isDirectory = false;
				auto relative_path = boost::filesystem::relative(p, FishEngine::Application::dataPath().parent_path());
				auto ext = p.extension();
				auto assetType = Resources::GetAssetType(ext);
				if (assetType == AssetType::Model)
				{
					modelPaths.push_back(p);
				}
				else
				{
					AssetDatabase::LoadAssetAtPath(relative_path);
				}
			}
		}

	}
	
	void FileInfo::AddFileNode(const Path & path)
	{
		auto parentDir = path.parent_path();
		auto parentNode = fileInfo(parentDir.string());
		auto fileNode = new FileInfo();
		fileNode->m_path = path;
		fileNode->m_parent = parentNode;
		auto relative_path = boost::filesystem::relative(path, FishEngine::Application::dataPath().parent_path());
		AssetDatabase::LoadAssetAtPath(relative_path);
	}
}


