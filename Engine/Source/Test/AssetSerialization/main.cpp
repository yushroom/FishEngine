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
    static YAML::Emitter & operator << (YAML::Emitter& archive, FishEngine::UUID const & t)
    {
        //static_assert(sizeof(t) == 16, "Error");
        archive << boost::uuids::to_string(t);
        return archive;
    }
    
    template<class T, class B>
    YAML::Emitter & operator << (YAML::Emitter& out, std::pair<T, B> const & v)
    {
    	//out << YAML::Flow;
    	//out << YAML::Key << v.first << YAML::Value << v.second;
    	out << v.first << v.second;
    	return out;
    }
    
    template<typename T>
    struct emittable : std::conditional_t<
        std::is_arithmetic<T>::value,
        std::true_type, std::false_type>
    {};
    
    
	class YAMLOutputArchive
	{
	public:
		YAMLOutputArchive(YAML::Emitter & emitter) : m_emitter(emitter)
		{

		}

		YAMLOutputArchive& operator = (YAMLOutputArchive const &) = delete;

		~YAMLOutputArchive() noexcept = default;
        

        template<typename T, std::enable_if_t<!emittable<T>::value, int> = 0>
		YAMLOutputArchive & operator << (T const & t)
		{
            prologue(*this, t);
            Save(*this, t);
            epilogue(*this, t);
            return *this;
		}
        
        template<typename T, std::enable_if_t<emittable<T>::value, int> = 0>
        YAMLOutputArchive & operator << (T const & t)
        {
            m_emitter << t;
            return *this;
        }
        
        
        YAMLOutputArchive & operator << (std::string const & t)
        {
            m_emitter << t;
            return *this;
        }

        YAMLOutputArchive & operator << (const char* t)
        {
            m_emitter << t;
            return *this;
        }

//	private:
//        //template <typename T>
//        template <class T>
//        friend inline void prologue(YAMLOutputArchive & archive, T const & t);
//        
//        template <class T>
//        friend inline void epilogue(YAMLOutputArchive& archive, T const & t);
        
		YAML::Emitter & m_emitter;
	};
    
    template <class T>
    inline void Save (YAMLOutputArchive & archive, std::weak_ptr<T> const & t)
    {
        auto spt = t.lock();
        archive << "fileID";
        if (spt == nullptr)
            archive << 0;
        else
            archive << spt->GetGUID();
    }
    
    template <class T>
    inline void Save (YAMLOutputArchive & archive, std::list<T> const & t)
    {
        for (auto & x : t)
            archive << x;
    }

    

	/************************************************************************/
	/* UUID                                                                 */
	/************************************************************************/
	YAMLOutputArchive & operator << (YAMLOutputArchive & archive, FishEngine::UUID const & t)
	{
		//archive.Save(t);
		return archive;
	}
    
    
    template<class T, class B>
    YAMLOutputArchive & operator << (YAMLOutputArchive & archive, std::pair<T, B> const & v)
    {
        archive << v.first << v.second;
        return archive;
    }
    
    //! delete it
    template<class T>
    YAMLOutputArchive & operator << (YAMLOutputArchive & archive, NameValuePair<T> const & nvp)
    {
        archive << nvp.name << nvp.value;
        return archive;
    }

    
    template <class T>
    inline void prologue(YAMLOutputArchive & archive, T const & t)
    {
        archive.m_emitter << YAML::Flow;
        archive.m_emitter << YAML::BeginMap;
    }
    
    template <class T>
    inline void epilogue(YAMLOutputArchive& archive, T const & t)
    {
        archive.m_emitter << YAML::EndMap;
    }
    
    template <class T>
    inline void prologue(YAMLOutputArchive & archive, std::list<T> const & t)
    {
        archive.m_emitter << YAML::Flow;
        archive.m_emitter << YAML::BeginSeq;
    }
    
    template <class T>
    inline void epilogue(YAMLOutputArchive& archive, std::list<T> const & t)
    {
        archive.m_emitter << YAML::EndSeq;
    }
}


using namespace FishEngine;


int main()
{
	YAML::Emitter emitter;
	YAMLOutputArchive archive(emitter);
	archive << Vector3(1, 2, 3);
	archive << Transform();
    std::cout << emitter.c_str() << std::endl;

	return 0;
}


#endif
