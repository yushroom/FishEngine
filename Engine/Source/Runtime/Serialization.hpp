#pragma once

#include "FishEngine.hpp"
#include "ReflectClass.hpp"

#include "Texture.hpp"
#include "Archive.hpp"
//#include "ReflectEnum.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"
#include "Color.hpp"
#include "Object.hpp"

#include "Serialization/types/map.hpp"
#include "Serialization/types/list.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
//#include <boost/uuid/uuid_serialize.hpp>

namespace FishEngine
{
    class Meta(NonSerializable) Serialization
    {
    public:
        Serialization() = delete;
        
        void SerializeScene(OutputArchive& archive);

    };
    
    /************************************************************************/
    /* UUID                                                                 */
    /************************************************************************/
    static OutputArchive & operator << ( OutputArchive& archive, FishEngine::UUID const & t )
    {
        //static_assert(sizeof(t) == 16, "Error");
        archive.SaveBinary(t.data, sizeof(t));
        return archive;
    }
    
    static InputArchive & operator >> (InputArchive& archive, FishEngine::UUID & t)
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
    /* std::string                                                          */
    /************************************************************************/
    static OutputArchive & operator << (OutputArchive & archive, const std::string & str)
    {
        archive << str.size();
        archive.SaveBinary(str.data(), str.size());
        return archive;
    }
    
    static InputArchive & operator >> (InputArchive & archive, std::string & str)
    {
        std::size_t size = 0;
        archive >> size;
        str.resize(size);
        archive.LoadBinary(const_cast<char *>(str.data()), size);
        return archive;
    }
    
    
    /************************************************************************/
    /* std::shared_ptr                                                      */
    /************************************************************************/
    
    template<typename T, typename std::enable_if_t<std::is_base_of<Component, T>::value, int> = 0>
    static OutputArchive & operator <<(OutputArchive& archive, const std::shared_ptr<T> & v)
    {
        archive << v->GetGUID();
    }
    
    template<typename T>
    static void Deserialize(InputArchive& archive, std::shared_ptr<T> & v) {}
    
    static OutputArchive & operator <<(OutputArchive& archive, const std::shared_ptr<Texture> & v)
    {
        archive << v->GetGUID();
    }
    
    /************************************************************************/
    /* std::weak_ptr                                                        */
    /************************************************************************/
    
    template<class Archive, typename T>
    static Archive & operator << ( Archive& archive, const std::weak_ptr<T> & v )
    {
        return archive;
    }
    
    template<class Archive, typename T>
    static Archive & operator >> ( Archive& archive, std::weak_ptr<T> & v )
    {
        return archive;
    }

    
    
    template<class T>
    static OutputArchive & operator << (OutputArchive & archive, T * v) = delete;
    
    template<class T>
    static InputArchive & operator >> (InputArchive & archive, T * t) = delete;
    
    
    /************************************************************************/
    /* Enums                                                                */
    /************************************************************************/
    template<class T, typename std::enable_if_t<std::is_enum<T>::value, int> = 0>
    inline OutputArchive & operator << (OutputArchive& archive, T const & t)
    {
        //archive << static_cast<uint32_t>(t);
        archive.SaveBinary(&t, sizeof(t));
        return archive;
    }
    
    template<class T, typename std::enable_if_t<std::is_enum<T>::value, int> = 0>
    inline InputArchive & operator >> (InputArchive& archive, T & t)
    {
        //uint32_t temp;
        //archive >> temp;
        //t = static_cast<T>(temp);
        archive.LoadBinary(&t, sizeof(t));
        return archive;
    }
}

#ifndef __REFLECTION_PARSER__
#include "generate/Class_Serialization.hpp"
#endif

