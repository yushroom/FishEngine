#pragma once

#include <type_traits>
#include <Object.hpp>
#include "ReflectClass.hpp"
#include "Serialization/NameValuePair.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace FishEngine
{	
	template <class T>
	constexpr int ArchiveID();
	
	class Meta(NonSerializable) InputArchive
	{
	public:
		
		InputArchive(std::istream & is) : m_istream(is) { }

		virtual ~InputArchive() = default;
		//virtual int ArchiveID() = 0;

		template<class T>
		InputArchive & operator >> (T & t)
		{
			return *this;
		}

		template<class T>
		InputArchive & operator >> (NameValuePair<T> && t)
		{
			return *this;
		}
		
	protected:
		std::istream & m_istream;
	};
	
	
	class Meta(NonSerializable) OutputArchive
	{
	public:

		OutputArchive(std::ostream & os) : m_ostream(os)
		{

		}

		virtual ~OutputArchive() = default;
		//virtual int ArchiveID() = 0;

		OutputArchive & operator << (short t)
		{
			this->Serialize(t);
			return *this;
		}

		OutputArchive & operator << (unsigned short t)
		{
			this->Serialize(t);
			return *this;
		}

		OutputArchive & operator << (int t)
		{
			this->Serialize(t);
			return *this;
		}

		OutputArchive & operator << (unsigned int t)
		{
			this->Serialize(t);
			return *this;
		}

		OutputArchive & operator << (long t)
		{
			this->Serialize(t);
			return *this;
		}

		OutputArchive & operator << (unsigned long t)
		{
			this->Serialize(t);
			return *this;
		}

		OutputArchive & operator << (long long t)
		{
			this->Serialize(t);
			return *this;
		}

		OutputArchive & operator << (unsigned long long t)
		{
			this->Serialize(t);
			return *this;
		}

		OutputArchive & operator << (float t)
		{
			this->Serialize(t);
			return *this;
		}

		OutputArchive & operator << (double t)
		{
			this->Serialize(t);
			return *this;
		}

		OutputArchive & operator << (bool t)
		{
			this->Serialize(t);
			return *this;
		}

		OutputArchive & operator << (std::nullptr_t const & t)
		{
			this->Serialize(t);
			return *this;
		}

		template<typename T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
		OutputArchive & operator << (T const & t)
		{
			(*this) << static_cast<std::underlying_type_t<T>>(t);
			return *this;
		}

		OutputArchive & operator << (std::string const & t)
		{
			this->Serialize(t);
			return *this;
		}

		OutputArchive & operator << (const char* t)
		{
			this->Serialize(t);
			return *this;
		}

		OutputArchive & operator << (boost::uuids::uuid const & t)
		{
			(*this) << boost::uuids::to_string(t);
			return *this;
		}

		template<class T, std::enable_if_t<std::is_base_of<Object, T>::value, int> = 0>
		OutputArchive & operator << (std::shared_ptr<T> const & obj)
		{
			SerializeObject(obj);
			return *this;
		}

		//OutputArchive & operator << (ObjectPtr const & obj)
		//{
		//	SerializeObject(obj);
		//	return *this;
		//}

		OutputArchive & operator << (std::weak_ptr<Object> const & obj)
		{
			SerializeWeakObject(obj);
			return *this;
		}

		template<class T>
		OutputArchive & operator << (NameValuePair<T> const & nvp)
		{
			SerializeNameOfNVP(nvp.name);
			MiddleOfNVP();
			(*this) << nvp.value;
			EndNVP();
			return *this;
		}

		template<class T, std::enable_if_t<std::is_base_of<Object, T>::value, int> = 0>
		OutputArchive & operator << (NameValuePair<const std::shared_ptr<T> &> const & nvp)
		{
			SerializeNameOfNVP(nvp.name);
			MiddleOfNVP();
			//(*this) << nvp.value;
			SerializeObject(nvp.value);
			EndNVP();
			return *this;
		}

		template<class T>
		OutputArchive & operator << (std::vector<T> const & t)
		{
			BeginSequence(t.size());
			for (auto & item : t)
			{
				(*this) << item;
			}
			EndSequence();
			return *this;
		}

		template<class T>
		OutputArchive & operator << (std::list<T> const & t)
		{
			BeginSequence(t.size());
			for (auto & item : t)
			{
				(*this) << item;
			}
			EndSequence();
			return *this;
		}

		template<class T, class B>
		OutputArchive & operator << (std::map<T, B> const & t)
		{
			BeginMap();
			for (auto & item : t)
			{
				(*this) << item.first << item.second;
			}
			EndMap();
			return *this;
		}

		virtual void BeginClass() {}
		virtual void EndClass() {}

		virtual void BeginMap() {}
		virtual void EndMap() {}

		virtual void BeginSequence(std::size_t sequenceSize) {}
		virtual void EndSequence() {}

	protected:
		virtual void Serialize(short t) { m_ostream << t; }
		virtual void Serialize(unsigned short t) { m_ostream << t; }
		virtual void Serialize(int t) { m_ostream << t; }
		virtual void Serialize(unsigned int t) { m_ostream << t; }
		virtual void Serialize(long t) { m_ostream << t; }
		virtual void Serialize(unsigned long t) { m_ostream << t; }
		virtual void Serialize(long long t) { m_ostream << t; }
		virtual void Serialize(unsigned long long t) { m_ostream << t; }
		virtual void Serialize(float t) { m_ostream << t; }
		virtual void Serialize(double t) { m_ostream << t; }
		virtual void Serialize(bool t) { m_ostream << t; }
		virtual void Serialize(std::string t) { m_ostream << t; }
		virtual void Serialize(const char* t) { m_ostream << t; }
		virtual void Serialize(std::nullptr_t const & t) { }

		virtual void SerializeObject(ObjectPtr const & obj) = 0;
		virtual void SerializeWeakObject(std::weak_ptr<Object> const & obj) = 0;
		virtual void EndNVP() = 0;
		virtual void SerializeNameOfNVP(const char* name) = 0;
		virtual void MiddleOfNVP() = 0;

		std::ostream & m_ostream;
	};

	template <class T>
	inline void Prologue(InputArchive& archive, T const & t)
	{ }

	template <class T>
	inline void Epilogue(InputArchive& archive, T const & t)
	{ }

	template <class T>
	inline void Prologue(OutputArchive& archive, T const & t)
	{
		archive.BeginClass();
	}

	template <class T>
	inline void Epilogue(OutputArchive& archive, T const & t)
	{
		archive.EndClass();
	}
}
