#pragma once
#include <string>
#include <vector>
#include <list>

#include "FishEngine.hpp"
#include "ReflectClass.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"
#include "Color.hpp"
#include "Object.hpp"

#define USE_FRIEND_SERIALIZATION_METHOD 0

namespace FishEngine
{
#if 1
	class Meta(NonSerializable) OutputArchive
	{
	public:
		OutputArchive(std::ostream & stream) : m_stream(stream)
		{
		}

		OutputArchive& operator = (OutputArchive const &) = delete;
		~OutputArchive() noexcept = default;
#if !USE_FRIEND_SERIALIZATION_METHOD
		inline OutputArchive & operator << (bool t);
		inline OutputArchive & operator << (char t);
		inline OutputArchive & operator << (short t);
		inline OutputArchive & operator << (unsigned short t);
		inline OutputArchive & operator << (int t);
		inline OutputArchive & operator << (unsigned int t);
		inline OutputArchive & operator << (long t);
		inline OutputArchive & operator << (unsigned long t);
		inline OutputArchive & operator << (long long t);
		inline OutputArchive & operator << (unsigned long long t);
		inline OutputArchive & operator << (float t);
		inline OutputArchive & operator << (double t);
		inline OutputArchive & operator << (long double t);
#endif
		template <class T>
		inline OutputArchive& operator << (T const & t)
		{
			Serialization::Serialize(*this, const_cast<T &>(t));
			return *this;
		}

		template <class T>
		inline OutputArchive& operator << (T * t) = delete;

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


	class Meta(NonSerializable) InputArchive
	{
	public:
		InputArchive(std::istream & stream) : m_stream(stream)
		{
		}

		InputArchive & operator = (InputArchive const &) = delete;
		~InputArchive() noexcept = default;
#if !USE_FRIEND_SERIALIZATION_METHOD
		inline InputArchive & operator >> (bool & t);
		inline InputArchive & operator >> (char & t);
		inline InputArchive & operator >> (short & t);
		inline InputArchive & operator >> (unsigned short & t);
		inline InputArchive & operator >> (int & t);
		inline InputArchive & operator >> (unsigned int & t);
		inline InputArchive & operator >> (long & t);
		inline InputArchive & operator >> (unsigned long & t);
		inline InputArchive & operator >> (long long & t);
		inline InputArchive & operator >> (unsigned long long & t);
		inline InputArchive & operator >> (float & t);
		inline InputArchive & operator >> (double & t);
		inline InputArchive & operator >> (long double & t);
#endif
		template<class T> inline
		InputArchive & operator >> (T & t)
		{
			Serialization::Deserialize(*this, t);
			return *this;
		}


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

	template<class T>
	static OutputArchive & operator << (OutputArchive & archive, T * v) = delete;


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
	/* std::map                                                             */
	/************************************************************************/
	template<class T, class B>
	static OutputArchive & operator << (OutputArchive & archive, const std::map<T, B> & v)
	{
		archive << v.size();
		for (auto& p : v)
		{
			archive << p.first << p.second;
		}
		return archive;
	}

	template<class T, class B>
	static InputArchive & operator >> (InputArchive & archive, std::map<T, B> & v)
	{
		std::size_t size = 0;
		archive >> size;
		v.clear();
		auto hint = v.begin();
		for (size_t i = 0; i < size; ++i)
		{
			T key;
			B value;
			archive >> key >> value;
			hint = v.emplace_hint(hint, std::move(key), std::move(value));
		}
		return archive;
	}


	/************************************************************************/
	/* Vector3                                                              */
	/************************************************************************/
	static OutputArchive & operator << (OutputArchive& archive, Vector3 const & t)
	{
		archive << t.x << t.y << t.z;
		return archive;
	}

	static InputArchive & operator >> (InputArchive& archive, Vector3 & t)
	{
		archive >> t.x >> t.y >> t.z;
		return archive;
	}

	/************************************************************************/
	/* Color                                                                */
	/************************************************************************/
	static OutputArchive & operator << (OutputArchive& archive, Color const & t)
	{
		archive << t.r << t.g << t.b;
		return archive;
	}

	static InputArchive & operator >> (InputArchive& archive, Color & t)
	{
		archive >> t.r >> t.g >> t.b;
		return archive;
	}

	/************************************************************************/
	/* Vector4                                                              */
	/************************************************************************/
	static OutputArchive & operator << (OutputArchive& archive, Vector4 const & t)
	{
		archive << t.x << t.y << t.z << t.w;
		return archive;
	}

	static InputArchive & operator >> (InputArchive& archive, Vector4 & t)
	{
		archive >> t.x >> t.y >> t.z >> t.w;
		return archive;
	}

	/************************************************************************/
	/* Quaternion                                                           */
	/************************************************************************/
	static OutputArchive & operator << (OutputArchive& archive, Quaternion const & t)
	{
		archive << t.x << t.y << t.z << t.w;
		return archive;
	}

	static InputArchive & operator >> (InputArchive& archive, Quaternion & t)
	{
		archive >> t.x >> t.y >> t.z >> t.w;
		return archive;
	}

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

	/************************************************************************/
	/* UUID                                                                 */
	/************************************************************************/
	static OutputArchive & operator << (OutputArchive& archive, FishEngine::UUID const & t)
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

#if USE_FRIEND_SERIALIZATION_METHOD
#define SERIALIZA_ARITHMETIC(T) \
	static OutputArchive & operator<<(OutputArchive & archive, T t) \
	{ \
		archive.SaveBinary(std::addressof(t), sizeof(t)); \
		return archive; \
	} \
	static InputArchive & operator >> (InputArchive & archive, T & t) \
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

#else
	/************************************************************************/
	/* OutputArchive methods                                                */
	/************************************************************************/

	OutputArchive & OutputArchive::operator<<(bool t)
	{
		SaveBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	OutputArchive & OutputArchive::operator<<(char t)
	{
		SaveBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	OutputArchive & OutputArchive::operator<<(short t)
	{
		SaveBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	OutputArchive & OutputArchive::operator<<(unsigned short t)
	{
		SaveBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	OutputArchive & OutputArchive::operator<<(int t)
	{
		SaveBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	OutputArchive & OutputArchive::operator<<(unsigned int t)
	{
		SaveBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	OutputArchive & OutputArchive::operator<<(long t)
	{
		SaveBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	OutputArchive & OutputArchive::operator<<(unsigned long t)
	{
		SaveBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	OutputArchive & OutputArchive::operator<<(long long t)
	{
		SaveBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	OutputArchive & OutputArchive::operator<<(unsigned long long t)
	{
		SaveBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	OutputArchive & OutputArchive::operator<<(float t)
	{
		SaveBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	OutputArchive & OutputArchive::operator<<(double t)
	{
		SaveBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	OutputArchive & OutputArchive::operator<<(long double t)
	{
		SaveBinary(std::addressof(t), sizeof(t));
		return *this;
	}




	/************************************************************************/
	/* InputArchive Methods                                                 */
	/************************************************************************/

	InputArchive & InputArchive::operator >> (bool & t)
	{
		LoadBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	InputArchive & InputArchive::operator >> (char & t)
	{
		LoadBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	InputArchive & InputArchive::operator >> (short & t)
	{
		LoadBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	InputArchive & InputArchive::operator >> (unsigned short & t)
	{
		LoadBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	InputArchive & InputArchive::operator >> (int & t)
	{
		LoadBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	InputArchive & InputArchive::operator >> (unsigned int & t)
	{
		LoadBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	InputArchive & InputArchive::operator >> (long & t)
	{
		LoadBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	InputArchive & InputArchive::operator >> (unsigned long & t)
	{
		LoadBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	InputArchive & InputArchive::operator >> (long long & t)
	{
		LoadBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	InputArchive & InputArchive::operator >> (unsigned long long & t)
	{
		LoadBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	InputArchive & InputArchive::operator >> (float & t)
	{
		LoadBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	InputArchive & InputArchive::operator >> (double & t)
	{
		LoadBinary(std::addressof(t), sizeof(t));
		return *this;
	}

	InputArchive & InputArchive::operator >> (long double & t)
	{
		LoadBinary(std::addressof(t), sizeof(t));
		return *this;
	}
#endif

#if 0

	//! Saving for POD types to binary
	template<class T> inline
	typename std::enable_if_t<std::is_arithmetic<T>::value>
	Serialize(OutputArchive & archive, T const & t)
	{
		//std::cout << "Serialize" << std::endl;
		archive.SaveBinary(std::addressof(t), sizeof(t));
	}


	//! Loading for POD types from binary
	template<class T> inline
	typename std::enable_if_t<std::is_arithmetic<T>::value>
	Deserialize(InputArchive & archive, T & t)
	{
		//std::cout << "Deserialize" << std::endl;
		archive.LoadBinary(std::addressof(t), sizeof(t));
	}

	void Serialize(FishEngine::OutputArchive & archive, const char* str) = delete;
	void Deserialize(FishEngine::InputArchive & archive, char* str) = delete;

	void Serialize(FishEngine::OutputArchive & archive, std::string const & str)
	{
		archive << str.size();
		archive.SaveBinary(str.data(), str.size());
	}

	void Deserialize(FishEngine::InputArchive & archive, std::string & str)
	{
		std::size_t size = 0;
		archive >> size;
		str.resize(size);
		archive.LoadBinary(const_cast<char *>(str.data()), size);
	}
#endif

#else
    class Archive
    {
    public:
        Archive();
        Archive(const Archive&);
        Archive& operator=(const Archive& ArchiveToCopy);
        virtual ~Archive();
        virtual void StartArray(const std::string& name) = 0;
        virtual void EndArray(const std::string& name) = 0;
        virtual void AddObject(ObjectPtr object) = 0;
    };
    
    class TextArchive : public Archive
    {
    public:
        virtual void StartArray(const std::string& name) override;
        virtual void EndArray(const std::string& name) override;
        virtual void AddObject(ObjectPtr object) override;
    };

    template<typename T>
    void Serialize(Archive& archive, const std::string& name, const T& value);

    template<typename T>
    void Serialize(Archive& archive, const std::string& name, std::shared_ptr<T> value)
    {
        Serialize(archive, name, value->m_uuid);
    }

    template<typename T>
    void Serialize(Archive& archive, const std::string& name, std::weak_ptr<T> value)
    {
        Serialize(archive, name, value->m_uuid);
    }

    template<typename T>
    void Serialize(Archive& archive, const std::string& name, const std::vector<T>& value)
    {
        archive.StartArray(name);
        for (auto& x : value)
        {
            Serizlize(archive, "", x);
        }
        archive.EndArray(name);
    }

    template<typename T>
    void Serialize(Archive& archive, const std::string& name, const std::list<T>& value)
    {
        archive.StartArray(name);
        for (auto& x : value)
        {
            Serizlize(archive, "", x);
        }
        archive.EndArray(name);
    }

    void Serialize(Archive& archive, const std::string& name, bool value);
    void Serialize(Archive& archive, const std::string& name, short value);
    void Serialize(Archive& archive, const std::string& name, unsigned short value);
    void Serialize(Archive& archive, const std::string& name, int  value);
    void Serialize(Archive& archive, const std::string& name, unsigned int value);
    void Serialize(Archive& archive, const std::string& name, long value);
#endif
}

#undef USE_FRIEND_SERIALIZATION_METHOD