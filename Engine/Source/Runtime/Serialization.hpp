#pragma once

#include <type_traits>

#include "FishEngine.hpp"
#include "ReflectClass.hpp"

#include "Texture.hpp"
#include "Archive.hpp"
#include "Object.hpp"

#include "Archive.hpp"
//#include "Serialization/archives/yaml.hpp"
//#include "Serialization/archives/BinaryOutputArchive.hpp"

#include "Serialization/helper.hpp"
#include "Serialization/types/map.hpp"
#include "Serialization/types/list.hpp"
//#include "Serialization/types/vector.hpp"
#include "Serialization/NameValuePair.hpp"
#include "Serialization/helper.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>
//#include "generate/Enum_LightType.hpp"
//#include <boost/uuid/uuid_serialize.hpp>

namespace FishEngine
{
	class Meta(NonSerializable) Serialization
	{
	public:
		Serialization() = delete;
		
		//void SerializeScene(BinaryOutputArchive& archive);
	};
	

#if 0	
	template <class T>
	inline void Prologue(OutputArchive& archive, T const & t)
	{
		if (archive.ArchiveID() == ArchiveID<YAMLOutputArchive>())
		{
			auto & ar = dynamic_cast<YAMLOutputArchive&>(archive);
			Prologue(ar, t);
		}
		//else if (archive.ArchiveID() == ArchiveID<BinaryOutputArchive>())
		//{
		//	auto & ar = dynamic_cast<BinaryOutputArchive&>(archive);
		//	Prologue(ar, t);
		//}
		else
		{
			abort();
		}
	}
	
	template <class T>
	inline void Epilogue(OutputArchive& archive, T const & t)
	{
		if (archive.ArchiveID() == ArchiveID<YAMLOutputArchive>())
		{
			auto & ar = dynamic_cast<YAMLOutputArchive&>(archive);
			Epilogue(ar, t);
		}
		//else if (archive.ArchiveID() == ArchiveID<BinaryOutputArchive>())
		//{
		//	auto & ar = dynamic_cast<BinaryOutputArchive&>(archive);
		//	Epilogue(ar, t);
		//}
		else
		{
			abort();
		}
	}

	template <class T>
	InputArchive& operator >> (InputArchive& archive, NameValuePair<T> && nvp)
	{
		return archive;
	}

	template <class T>
	OutputArchive& operator<<(OutputArchive& archive, NameValuePair<T> const & nvp)
	{
		if (archive.ArchiveID() == ArchiveID<YAMLOutputArchive>())
		{
			auto & ar = dynamic_cast<YAMLOutputArchive&>(archive);
			ar << nvp;
		}
		//else if (archive.ArchiveID() == ArchiveID<BinaryOutputArchive>())
		//{
		//	auto & ar = dynamic_cast<BinaryOutputArchive&>(archive);
		//	ar << nvp;
		//}
		else
		{
			abort();
		}
		return archive;
	}


	template <class T, std::enable_if_t<std::is_arithmetic<T>::value, int> = 0>
	InputArchive& operator>>( InputArchive& archive, T& value )
	{
		return archive;
	}
	
	template <class T, std::enable_if_t<std::is_arithmetic<T>::value, int> = 0>
	OutputArchive& operator<<( OutputArchive& archive, T& value )
	{
		return archive;
	}
	
	template <class T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
	InputArchive& operator>>( InputArchive& archive, T& value )
	{
		archive << static_cast<std::underlying_type_t<T> > ( value );
		return archive;
	}
	
	template <class T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
	OutputArchive& operator<<( OutputArchive& archive, T& value )
	{
		archive >> static_cast<std::underlying_type_t<T> > ( value );
		return archive;
	}
	
	

	

	
	
  //  template <class T>
  //  InputArchive& operator>>( InputArchive& archive, BaseClassWrapper<T> && nvp)
  //  {
  //      return archive;
  //  }
  //  
  //  template <class T>
  //  OutputArchive& operator<<( OutputArchive& archive, BaseClassWrapper<T> const & base)
  //  {
		//if (archive.ArchiveID() == ArchiveID<YAMLOutputArchive>())
		//{
		//	auto & ar = dynamic_cast<YAMLOutputArchive&>(archive);
		//	ar << base;
		//}
		//else
		//{
		//	abort();
		//}
  //      return archive;
  //  }

	
	// template<class Archive, class T>
	// void Serialize (Archive & archive, NameValuePair<T> const & nvp)
	// {
	// 	archive << nvp.name << nvp.value;
	// }
	
 //    template<class Archive, typename T>
 //    static void Serialize ( Archive& archive, BaseClassWrapper<T> const & t )
 //    {
 //        archive << make_nvp(T::StaticClassName(), t.base_ref);
 //        //archive << t.base_ref;
 //    }

	/************************************************************************/
	/* std::shared_ptr                                                      */
	/************************************************************************/
	template<class Archive, typename T>
	static void operator << ( Archive& archive, std::shared_ptr<T> const & t )
	{
		if (t != nullptr)
		{
			archive << t->GetInstanceID();
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
	inline void Serialize ( Archive& archive, const std::weak_ptr<T> & v )
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
	inline void Serialize ( Archive& archive, boost::filesystem::path const & v )
	{
		archive << v.c_str();
	}
#endif
}

//#ifndef __REFLECTION_PARSER__
//#include "generate/Class_Serialization.hpp"
//#endif

