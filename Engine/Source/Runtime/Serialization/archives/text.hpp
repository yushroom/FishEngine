#pragma once

#include <iostream>
#include "../../ReflectClass.hpp"
#include "../../GUID.hpp"

namespace FishEngine
{
    class Meta(NonSerializable) TextOutputArchive
    {
    public:
        TextOutputArchive(std::ostream & stream) : m_stream(stream)
        {
        }
        
        TextOutputArchive& operator = (TextOutputArchive const &) = delete;
        
        ~TextOutputArchive() noexcept = default;
        
        template<typename T>
        void Save ( T const & t)
        {
            m_stream << t << std::endl;
        }
        
//        template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, int> = 0>
//        TextOutputArchive & operator << (T const & t)
//        {
//            this->Save(t);
//            return *this;
//        }
        
    private:
        std::ostream & m_stream;
    };


	class Meta(NonSerializable) TextInputArchive
	{
	public:
		TextInputArchive(std::istream & stream) : m_stream(stream)
		{
		}

		TextInputArchive& operator = (TextInputArchive const &) = delete;

		~TextInputArchive() noexcept = default;

		template<typename T>
		void Load ( T & t )
		{
			m_stream >> t;
		}

	private:
		std::istream & m_stream;
	};

    /************************************************************************/
    /* arithmetic                                                           */
    /************************************************************************/
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, int> = 0>
    TextOutputArchive & operator << ( TextOutputArchive & archive, T const & t)
    {
        archive.Save(t);
        return archive;
    }

	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, int> = 0>
	TextInputArchive & operator >> ( TextInputArchive & archive, T & t )
	{
		archive.Load(t);
		return archive;
	}


	/************************************************************************/
	/* enums                                                                */
	/************************************************************************/
	template<typename T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
	TextOutputArchive & operator << ( TextOutputArchive & archive, T const & t )
	{
		//archive.Save(std::string(FishEngine::EnumToString(t)));
		archive.Save(static_cast<uint32_t>(t));
		return archive;
	}

	template<typename T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
	TextInputArchive & operator >> ( TextInputArchive & archive, T & t )
	{
		//archive.Load(std::string(FishEngine::EnumToString(t)));
		uint32_t e;
		archive.Load(e);
		t = static_cast<T>(e);
		return archive;
	}

	/************************************************************************/
	/* std::string                                                          */
	/************************************************************************/

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
	static TextOutputArchive & operator << (TextOutputArchive & archive, GUID const & t)
	{
		archive << ToString(t);
		return archive;
	}
}
