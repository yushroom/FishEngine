#pragma once

#include "FishEngine.hpp"
#include "ReflectClass.hpp"

#include "Texture.hpp"
#include "Archive.hpp"
#include "Object.hpp"

#include "Serialization/helper.hpp"
#include "Serialization/types/map.hpp"
#include "Serialization/types/list.hpp"
//#include "Serialization/types/vector.hpp"
#include "Serialization/NameValuePair.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>
#include "generate/Enum_LightType.hpp"
//#include <boost/uuid/uuid_serialize.hpp>

namespace FishEngine
{
    class Meta(NonSerializable) Serialization
    {
    public:
        Serialization() = delete;
        
        //void SerializeScene(BinaryOutputArchive& archive);
    };
    
	template<class Archive, class T>
	void Save (Archive & archive, NameValuePair<T> const & nvp)
	{
		archive << nvp.name << nvp.value;
	}
    
    template<class Archive, typename T>
    static void Save ( Archive& archive, BaseClassWrapper<T> const & t )
    {
        archive << make_nvp(T::StaticClassName(), t.base_ref);
        //archive << t.base_ref;
    }

    /************************************************************************/
    /* std::shared_ptr                                                      */
    /************************************************************************/
	template<class Archive, typename T>
    static void Save ( Archive& archive, std::shared_ptr<T> const & t )
    {
		if (t != nullptr)
		{
			archive << t->GetGUID();
		}
		else
		{
			archive << nullptr;
		}
    }
    
    /************************************************************************/
    /* std::weak_ptr                                                        */
    /************************************************************************/
    template<class Archive, typename T>
    inline void Save ( Archive& archive, const std::weak_ptr<T> & v )
    {
		auto t = v.lock();
		if (t != nullptr)
		{
			archive << t->GetGUID();
		}
		else
		{
			archive << nullptr;
		}
    }
    
    template<class Archive, typename T>
    static Archive & operator >> ( Archive & archive, std::weak_ptr<T> & v )
    {
        return archive;
    }

	/************************************************************************/
	/* boost::filesystem::path                                              */
	/************************************************************************/
	template<class Archive, typename T>
	inline void Save(Archive& archive, boost::filesystem::path const & v)
	{
		archive << v.c_str();
	}

}

#ifndef __REFLECTION_PARSER__
#include "generate/Class_Serialization.hpp"
#endif

