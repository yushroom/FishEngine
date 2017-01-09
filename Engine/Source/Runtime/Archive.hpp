#pragma once
#include <string>
#include <vector>
#include <list>
#include <iostream>

#include "FishEngine.hpp"
#include "ReflectClass.hpp"

namespace FishEngine
{
	class Meta(NonSerializable) OutputArchive
	{
	public:
		OutputArchive(std::ostream & stream) : m_stream(stream)
		{
		}

		OutputArchive& operator = (OutputArchive const &) = delete;
        
		~OutputArchive() noexcept = default;

		void SaveBinary(const void * data, std::size_t size)
		{
			auto const written_size = static_cast<std::size_t>(m_stream.rdbuf()->sputn(reinterpret_cast<const char*>(data), size));
			if (written_size != size)
			{
				abort();
			}
		}

	private:
		std::ostream & m_stream;
	};
    
    
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
    
    template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, int> = 0>
    TextOutputArchive & operator << ( TextOutputArchive & archive, T const & t)
    {
        archive.Save(t);
        return archive;
    }
    
    template<typename T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
    TextOutputArchive & operator << ( TextOutputArchive & archive, T const & t)
    {
        archive.Save(t);
        return archive;
    }
    
    TextOutputArchive & operator << ( TextOutputArchive & archive, std::string const & t)
    {
        archive.Save(t);
        return archive;
    }


	class Meta(NonSerializable) InputArchive
	{
	public:
		InputArchive(std::istream & stream) : m_stream(stream)
		{
		}

		InputArchive & operator = (InputArchive const &) = delete;
        
		~InputArchive() noexcept = default;

        void LoadBinary(void * const data, std::size_t size)
		{
			auto const read_size = static_cast<std::size_t>(m_stream.rdbuf()->sgetn(reinterpret_cast<char*>(data), size));
			if (read_size != size)
			{
				abort();
			}
		}

	private:
		std::istream & m_stream;
	};

#define SERIALIZA_ARITHMETIC(T) \
	inline OutputArchive & operator<<(OutputArchive & archive, T t) \
	{ \
		archive.SaveBinary(std::addressof(t), sizeof(t)); \
		return archive; \
	} \
	inline InputArchive & operator >> (InputArchive & archive, T & t) \
	{ \
		archive.LoadBinary(std::addressof(t), sizeof(t)); \
		return archive; \
	}

	SERIALIZA_ARITHMETIC(bool);
	SERIALIZA_ARITHMETIC(char);
	SERIALIZA_ARITHMETIC(char16_t);
	SERIALIZA_ARITHMETIC(char32_t);
	SERIALIZA_ARITHMETIC(wchar_t);
	SERIALIZA_ARITHMETIC(signed char);
	SERIALIZA_ARITHMETIC(short int);
	SERIALIZA_ARITHMETIC(int);
	SERIALIZA_ARITHMETIC(long int);
	SERIALIZA_ARITHMETIC(long long int);
	SERIALIZA_ARITHMETIC(unsigned char);
	SERIALIZA_ARITHMETIC(unsigned short int);
	SERIALIZA_ARITHMETIC(unsigned int);
	SERIALIZA_ARITHMETIC(unsigned long int);
	SERIALIZA_ARITHMETIC(unsigned long long int);
	SERIALIZA_ARITHMETIC(float);
	SERIALIZA_ARITHMETIC(double);
	SERIALIZA_ARITHMETIC(long double);

#undef SERIALIZA_ARITHMETIC
}
