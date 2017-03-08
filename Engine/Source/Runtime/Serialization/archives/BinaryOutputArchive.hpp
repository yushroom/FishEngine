#pragma once

#include "../../Archive.hpp"
#include "../NameValuePair.hpp"

namespace FishEngine
{
	class BinaryOutputArchive;

	template <>
	constexpr int ArchiveID<BinaryOutputArchive>()
	{
		return 2;
	}

	class BinaryOutputArchive : public OutputArchive
	{
	public:
		BinaryOutputArchive(std::ostream & ostream) : m_ostream(ostream) {}
		BinaryOutputArchive(BinaryOutputArchive const &) = delete;
		BinaryOutputArchive& operator = (BinaryOutputArchive const &) = delete;

		~BinaryOutputArchive() = default;

		virtual int ArchiveID()
		{
			return FishEngine::ArchiveID<BinaryOutputArchive>();
		}

		void SaveBinary(const void * data, std::size_t size)
		{
			auto const writtenSize = static_cast<std::size_t>(m_ostream.rdbuf()->sputn(reinterpret_cast<const char*>(data), size));
			if (writtenSize != size)
			{
				//throw Exception("Failed to write " + std::to_string(size) + " bytes to output stream! Wrote " + std::to_string(writtenSize));
				abort();
			}
		}

		template<typename T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
		BinaryOutputArchive & operator << (T t)
		{
			SaveBinary(std::addressof(t), sizeof(t));
			return *this;
		}

		BinaryOutputArchive & operator << (std::string const & str)
		{
			SaveBinary(str.data(), str.size());
			return *this;
		}

		template<class T>
		BinaryOutputArchive & operator << (NameValuePair<T> const & t)
		{
			(*this) << t.value;
			return *this;
		}

		//! Serialization for std::vectors of arithmetic (but not bool) using binary serialization
		template<typename T, typename std::enable_if_t<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value, int> = 0>
		BinaryOutputArchive & operator << (const std::vector<T> & v)
		{
			(*this) << v.size();
			SaveBinary(v.data(), v.size() * sizeof(T));
			return *this;
		}

		//! Serialization for non-arithmetic vector types
		template<typename T, typename std::enable_if_t<!std::is_arithmetic<T>::value, int> = 0>
		BinaryOutputArchive & operator << (const std::vector<T> & v)
		{
			(*this) << v.size();
			for (auto && x : v)
			{
				(*this) << x;
			}
			return *this;
		}

		template<class T, class B>
		BinaryOutputArchive & operator << (std::map<T, B> const & v)
		{
			//(*this) << make_size_tag(v.size());
			(*this) << v.size();
			for (auto& p : v)
			{
				(*this) << p.first << p.second;
			}
		}

		//template<typename T, typename std::enable_if_t<std::is_base_of<Object, T>::value, int> = 0>
		BinaryOutputArchive & operator << (ObjectPtr const & object);

	private:
		std::ostream & m_ostream;
	};
}
