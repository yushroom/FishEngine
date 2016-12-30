#include "../Serialization.hpp"

namespace FishEngine::Serialization
{
	class BinaryOutputArchive : public OutputArchive<BinaryOutputArchive>
	{
	public:
		BinaryOutputArchive(std::ostream & stream)
			: OutputArchive<BinaryOutputArchive>(this), m_stream(stream)
		{
		}

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

	class BinaryInputArchive : public InputArchive<BinaryInputArchive>
	{
	public:
		BinaryInputArchive(std::istream & stream)
			: InputArchive<BinaryInputArchive>(this), m_stream(stream)
		{
		}

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

	// ######################################################################
	// Common BinaryArchive serialization functions

	//! Saving for POD types to binary
	template<class T> inline
		typename std::enable_if_t<std::is_arithmetic<T>::value>
		Serialize(BinaryOutputArchive & archive, T const & t)
	{
		std::cout << "Serialize" << std::endl;
		archive.SaveBinary(std::addressof(t), sizeof(t));
	}


	//! Loading for POD types from binary
	template<class T> inline
		typename std::enable_if_t<std::is_arithmetic<T>::value>
		Deserialize(BinaryInputArchive & archive, T & t)
	{
		archive.LoadBinary(std::addressof(t), sizeof(t));
	}

	//! Serializing NVP types to binary
	template<class T> inline
		void Serialize(BinaryOutputArchive & archive, NameValuePair<T> & t)
	{
		archive(t.value);
	}
}