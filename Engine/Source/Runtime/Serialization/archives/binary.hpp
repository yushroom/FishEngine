#pragma once

#include <iostream>
#include "../../ReflectClass.hpp"

namespace FishEngine
{
	class Meta(NonSerializable) BinaryOutputArchive
	{
	public:
		BinaryOutputArchive(std::ostream & stream) : m_stream(stream)
		{
		}

		BinaryOutputArchive& operator = (BinaryOutputArchive const &) = delete;
		
		~BinaryOutputArchive() noexcept = default;

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


	class Meta(NonSerializable) BinaryInputArchive
	{
	public:
		BinaryInputArchive(std::istream & stream) : m_stream(stream)
		{
		}

		BinaryInputArchive & operator = (BinaryInputArchive const &) = delete;
		
		~BinaryInputArchive() noexcept = default;

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

	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value || std::is_enum<T>::value, int> = 0>
	BinaryOutputArchive & operator << (BinaryOutputArchive & archive, T const & t)
	{
		archive.SaveBinary(std::addressof(t), sizeof(t));
		return archive;
	}

	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value || std::is_enum<T>::value, int> = 0>
	BinaryInputArchive & operator >> (BinaryInputArchive & archive, T & t)
	{
		archive.LoadBinary(std::addressof(t), sizeof(t));
		return archive;
	}
}
