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

    /************************************************************************/
    /* std::shared_ptr                                                      */
    /************************************************************************/
	template<class Archive, typename T>
    static void Save ( Archive& archive, std::shared_ptr<T> const & v )
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
    static Archive & operator >> ( Archive& archive, std::weak_ptr<T> & v )
    {
        return archive;
    }

	template<class Archive>
	static void DynamicSerializeObject(Archive archive, std::shared_ptr<Object> obj)
	{
		auto name = obj->ClassName();
		if (name == "GameObject")
		{
			archive << *dynamic_pointer_cast<GameObject>(obj);
		}
		else if (name == "Transform")
		{
			archive << *dynamic_pointer_cast<Transform>(obj);
		}
	}
}

#ifndef __REFLECTION_PARSER__
#include "generate/Class_Serialization.hpp"
#endif

