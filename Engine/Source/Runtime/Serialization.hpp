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
	template <class T>
	class Meta(NonSerializable) SizeTag
	{
	private:
		// Store a reference if passed an lvalue reference, otherwise
		// make a copy of the data
		using Type = std::conditional_t<std::is_lvalue_reference<T>::value, T, std::decay_t<T>>;

		SizeTag & operator=(SizeTag const &) = delete;

	public:
		SizeTag(T && sz) : size(std::forward<T>(sz)) {}

		Type size;
	};

	template <class T>
	inline SizeTag<T> make_size_tag(T && sz)
	{
		return{ std::forward<T>(sz) };
	}
    
    template<class Base>
    struct base_class
    {
        template<class Derived>
        base_class(Derived const & derived) :
            base_ref(static_cast<Base const &>(derived))
        {
            static_assert( std::is_base_of<Base, Derived>::value, "Can only use base_class on a valid base class" );
        }
        
        Base const & base_ref;
    };


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
    static void Save ( Archive& archive, base_class<T> const & t )
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
}

#ifndef __REFLECTION_PARSER__
#include "generate/Class_Serialization.hpp"
#endif

