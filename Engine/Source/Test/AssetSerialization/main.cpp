#if 0
#include <TextureImporter.hpp>
#include <iostream>

using namespace std;
using namespace FishEngine;

void GenerateMetaFiles(FishEngine::Path const & path)
{
	assert(boost::filesystem::exists(path));
	auto ext = path.extension();
	if (ext == ".png")
	{
		cout << path.stem() << " is png";
	}
}

int main()
{
	Resources::SetAssetsDirectory(R"(D:\program\FishEngine\Engine\Source\Test\AssetSerialization\Test\Assets)");
	TextureImporter importer;

	return 0;
}

#else

#include <yaml-cpp/yaml.h>
#include <FishEngine.hpp>
#include <Vector3.hpp>
#include <iostream>
#include <Object.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <Archive.hpp>
#include <Serialization.hpp>

namespace FishEngine
{
	class YAMLOutputArchive
	{
	public:
		YAMLOutputArchive(YAML::Emitter & emitter) : m_emitter(emitter)
		{

		}

		YAMLOutputArchive& operator = (YAMLOutputArchive const &) = delete;

		~YAMLOutputArchive() noexcept = default;

		template<typename T>
		void Save (T const & t)
		{
			m_emitter << YAML::Flow;
			m_emitter << YAML::BeginMap;
			m_emitter << t;
			m_emitter << YAML::EndMap;
		}

	private:
		YAML::Emitter & m_emitter;
	};

	/************************************************************************/
	/* arithmetic                                                           */
	/************************************************************************/
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, int> = 0>
	YAMLOutputArchive & operator << (YAMLOutputArchive & archive, T const & t)
	{
		archive.Save(t);
		return archive;
	}


	/************************************************************************/
	/* UUID                                                                 */
	/************************************************************************/
	YAMLOutputArchive & operator << (YAMLOutputArchive & archive, FishEngine::UUID const & t)
	{
		archive.Save(t);
		return archive;
	}

	static YAML::Emitter & operator << (YAML::Emitter& archive, FishEngine::UUID const & t)
	{
		//static_assert(sizeof(t) == 16, "Error");
		archive << boost::uuids::to_string(t);
		return archive;
	}

	template <class T>
	inline void prologue(YAML::Emitter& archive, T const & t)
	{
		archive << YAML::Flow;
		archive << YAML::BeginMap;
	}

	template <class T>
	inline void epilogue(YAML::Emitter& archive, T const & t)
	{
		archive << YAML::EndMap;
	}



	//inline void prologue(YAML::Emitter& archive, Vector3 const & t)
	//{
	//	archive << YAML::Flow;
	//	archive << YAML::BeginMap;
	//}

	//inline void epilogue(YAML::Emitter& archive, Vector3 const & t)
	//{
	//	archive << YAML::EndMap;
	//}
}


using namespace FishEngine;

//template<class T, class B>
//YAML::Emitter & operator << (YAML::Emitter& out, std::pair<T, B> const & v)
//{
//	//out << YAML::Flow;
//	//out << YAML::Key << v.first << YAML::Value << v.second;
//	out << v.first << v.second;
//	return out;
//}

//YAML::Emitter & operator << (YAML::Emitter& out, Vector3 const & v)
//{
//	out << YAML::Flow;
//	out << YAML::BeginMap << std::make_pair("x", v.x) << std::make_pair("y", v.y) << std::make_pair("z", v.z) << YAML::EndMap;
//	return out;
//}

int main()
{
	YAML::Emitter emitter;
	YAMLOutputArchive archive(emitter);
	archive << Vector3(1, 2, 3);
	archive << Transform();
	std::cout << emitter.c_str();

	return 0;
}


#endif