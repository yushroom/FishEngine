#pragma once

#include "FishEngine.hpp"
#include "ReflectClass.hpp"

#include "Texture.hpp"
#include "Archive.hpp"
#include "Object.hpp"

#include "Serialization/types/map.hpp"
#include "Serialization/types/list.hpp"
#include "Serialization/NameValuePair.hpp"

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
    
//    template<class Archive, class T>
//    Archive & operator << (Archive & archive, NameValuePair<T> const & nvp)
//    {
//        archive << nvp.name << nvp.value;
//        return archive;
//    }



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
    inline void Save ( Archive& archive, const std::weak_ptr<T> & v )
    {
		auto t = v.lock();
		if (t != nullptr)
		{
			archive << t->GetGUID();
		}
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
    
//    /************************************************************************/
//    /* Vector3 Serialization                                                */
//    /************************************************************************/
//    template<class Archive>
//    inline void Save (Archive& archive, Vector3 const & t)
//    {
//        archive << make_nvp("x", t.x) << make_nvp("y", t.y) << make_nvp("z", t.z);
//    }
//    
//    template<class Archive>
//    inline void Load (Archive& archive, Vector3 & t)
//    {
//        archive >> t.x >> t.y >> t.z;
//    }
//    
//    /************************************************************************/
//    /* Vector4 Serialization                                                */
//    /************************************************************************/
//    template<class Archive>
//    static Archive & operator << (Archive& archive, Vector4 const & t)
//    {
//        archive << make_nvp("x", t.x) << make_nvp("y", t.y) << make_nvp("z", t.z) << make_nvp("w", t.w);
//        return archive;
//    }
//    
//    template<class Archive>
//    static Archive & operator >> (Archive& archive, Vector4 & t)
//    {
//        archive >> t.x >> t.y >> t.z >> t.w;
//        return archive;
//    }
    
    
//    // Quaternion
//    template<typename Archive>
//    void Save ( Archive& archive, Quaternion const & q )
//    {
//        archive << q.x << q.y << q.z << q.w;
//    }
//    
//    template<typename Archive>
//    void Load ( Archive& archive, Quaternion & q )
//    {
//        archive >> q.x >> q.y >> q.z >> q.w;
//    }
}

#ifndef __REFLECTION_PARSER__
#include "generate/Class_Serialization.hpp"
#endif

