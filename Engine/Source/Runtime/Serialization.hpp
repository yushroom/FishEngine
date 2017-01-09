#pragma once

#include "FishEngine.hpp"
#include "ReflectClass.hpp"

#include "Texture.hpp"
#include "Archive.hpp"
#include "Object.hpp"

#include "Serialization/types/map.hpp"
#include "Serialization/types/list.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "generate/Enum_LightType.hpp"
//#include <boost/uuid/uuid_serialize.hpp>

namespace FishEngine
{
    class Meta(NonSerializable) Serialization
    {
    public:
        Serialization() = delete;
        
        void SerializeScene(BinaryOutputArchive& archive);

    };


	/************************************************************************/
	/* std::string                                                          */
	/************************************************************************/
	static BinaryOutputArchive & operator << (BinaryOutputArchive & archive, const std::string & str)
	{
		archive << str.size();
		archive.SaveBinary(str.data(), str.size());
		return archive;
	}

	static BinaryInputArchive & operator >> (BinaryInputArchive & archive, std::string & str)
	{
		std::size_t size = 0;
		archive >> size;
		str.resize(size);
		archive.LoadBinary(const_cast<char *>(str.data()), size);
		return archive;
	}

	TextOutputArchive & operator << (TextOutputArchive & archive, std::string const & t)
	{
		archive.Save(t);
		return archive;
	}

	TextInputArchive & operator >> (TextInputArchive & archive, std::string & t)
	{
		archive.Load(t);
		return archive;
	}
    
    /************************************************************************/
    /* UUID                                                                 */
    /************************************************************************/
    static BinaryOutputArchive & operator << ( BinaryOutputArchive& archive, FishEngine::UUID const & t )
    {
        //static_assert(sizeof(t) == 16, "Error");
        archive.SaveBinary(t.data, sizeof(t));
        return archive;
    }
    
    static BinaryInputArchive & operator >> (BinaryInputArchive& archive, FishEngine::UUID & t)
    {
        //static_assert(sizeof(t) == 16, "Error");
        archive.LoadBinary(t.data, sizeof(t));
        return archive;
    }
    
    static TextOutputArchive & operator << ( TextOutputArchive & archive, FishEngine::UUID const & t)
    {
        archive << boost::uuids::to_string(t);
        return archive;
    }
    
    
    /************************************************************************/
    /* std::shared_ptr                                                      */
    /************************************************************************/
    
    template<typename T, typename std::enable_if_t<std::is_base_of<Component, T>::value, int> = 0>
    static BinaryOutputArchive & operator <<(BinaryOutputArchive& archive, const std::shared_ptr<T> & v)
    {
        archive << v->GetGUID();
		return archive;
    }
    
    template<typename T>
    static void Deserialize(BinaryInputArchive& archive, std::shared_ptr<T> & v) {}
    
    static BinaryOutputArchive & operator <<(BinaryOutputArchive& archive, const std::shared_ptr<Texture> & v)
    {
        archive << v->GetGUID();
		return archive;
    }
    
    /************************************************************************/
    /* std::weak_ptr                                                        */
    /************************************************************************/
    
    template<class Archive, typename T>
    static Archive & operator << ( Archive& archive, const std::weak_ptr<T> & v )
    {
		auto t = v.lock();
		if (t != nullptr)
		{
			archive << t->GetGUID();
		}
        return archive;
    }
    
    template<class Archive, typename T>
    static Archive & operator >> ( Archive& archive, std::weak_ptr<T> & v )
    {
        return archive;
    }

    
    
    template<class T>
    static BinaryOutputArchive & operator << (BinaryOutputArchive & archive, T * v) = delete;
    
    template<class T>
    static BinaryInputArchive & operator >> (BinaryInputArchive & archive, T * t) = delete;
}

#ifndef __REFLECTION_PARSER__
#include "generate/Class_Serialization.hpp"
#endif

