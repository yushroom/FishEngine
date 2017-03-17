#pragma once

#include <type_traits>
#include <sstream>

#include "Object.hpp"
#include "ReflectClass.hpp"
#include "Serialization/NameValuePair.hpp"
#include "GUID.hpp"
#include "Path.hpp"

namespace FishEngine
{	
	class Meta(NonSerializable) InputArchive
	{
	public:
		
		InputArchive(std::istream & is) : m_istream(is) { }

		virtual ~InputArchive() = default;
		//virtual int ArchiveID() = 0;

		InputArchive & operator >> (short & t)
		{
			this->Deserialize(t);
			return *this;
		}

		InputArchive & operator >> (unsigned short & t)
		{
			this->Deserialize(t);
			return *this;
		}

		InputArchive & operator >> (int & t)
		{
			this->Deserialize(t);
			return *this;
		}

		InputArchive & operator >> (unsigned int & t)
		{
			this->Deserialize(t);
			return *this;
		}

		InputArchive & operator >> (long & t)
		{
			this->Deserialize(t);
			return *this;
		}

		InputArchive & operator >> (unsigned long & t)
		{
			this->Deserialize(t);
			return *this;
		}

		InputArchive & operator >> (long long & t)
		{
			this->Deserialize(t);
			return *this;
		}

		InputArchive & operator >> (unsigned long long & t)
		{
			this->Deserialize(t);
			return *this;
		}

		InputArchive & operator >> (float & t)
		{
			this->Deserialize(t);
			return *this;
		}

		InputArchive & operator >> (double & t)
		{
			this->Deserialize(t);
			return *this;
		}

		InputArchive & operator >> (bool & t)
		{
			this->Deserialize(t);
			return *this;
		}

		//InputArchive & operator >> (std::nullptr_t const & t)
		//{
		//	this->Serialize(t);
		//	return *this;
		//}
		
		template<typename T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
		InputArchive & operator >> (T & t)
		{
			std::underlying_type_t<T> temp;
			(*this) >> temp;
			t = static_cast<T>(temp);
			return *this;
		}
		
		InputArchive & operator >> (std::string & t)
		{
			this->Deserialize(t);
			return *this;
		}

		InputArchive & operator >> (GUID & t)
		{
			std::string guidStr;
			(*this) >> guidStr;
			std::istringstream sin(guidStr);
			sin >> t;
			return *this;
		}
		
		InputArchive & operator >> (FishEngine::Path & path)
		{
			std::string pathStr;
			(*this) >> pathStr;
			path = pathStr;
			return *this;
		}

		template<class T, std::enable_if_t<std::is_base_of<Object, T>::value, int> = 0>
		InputArchive & operator >> (std::shared_ptr<T> & obj)
		{
			DeserializeObject(obj);
			return *this;
		}

		//OutputArchive & operator << (ObjectPtr const & obj)
		//{
		//	SerializeObject(obj);
		//	return *this;
		//}

		template<class T, std::enable_if_t<std::is_base_of<Object, T>::value, int> = 0>
		InputArchive & operator >> (std::weak_ptr<T> & obj)
		{
			DeserializeWeakObject(obj);
			return *this;
		}

		template<class T>
		InputArchive & operator >> (NameValuePair<T> && nvp)
		{
			NameOfNVP(nvp.name);
			MiddleOfNVP();
			(*this) >> nvp.value;
			EndNVP();
			return *this;
		}

		template<class T, std::enable_if_t<std::is_base_of<Object, T>::value, int> = 0>
		InputArchive & operator >> (NameValuePair<const std::shared_ptr<T> &> && nvp)
		{ 
			NameOfNVP(nvp.name);
			MiddleOfNVP();
			DeserializeObject(nvp.value);
			EndNVP();
			return *this;
		}

		template<class T>
		InputArchive & operator >> (std::vector<T> & t)
		{
			//for (auto & item : t)
			//{
			//	(*this) << item;
			//}
			auto size = BeginSequence();
			t.resize(size);
			//abort();
			for (auto & x : t)
			{
				(*this) >> x;
			}
			EndSequence();
			return *this;
		}

		template<class T>
		InputArchive & operator >> (std::list<T> & t)
		{
			//BeginSequence(t.size());
			//for (auto & item : t)
			//{
			//	(*this) << item;
			//}
			//EndSequence();
			abort();
			return *this;
		}

		template<class T, class B>
		InputArchive & operator >> (std::map<T, B> & t)
		{
			t.clear();
			auto size = GetSizeTag();
			BeginMap();
			auto hint = t.begin();
			for (size_t i = 0; i < size; ++i)
			{
				T key;
				B value;
				(*this) >> key >> value;
				hint = t.emplace_hint(hint, std::move(key), std::move(value));
			}
			EndMap();
			return *this;
		}

		virtual void BeginClass() {}
		virtual void EndClass() {}
		
		virtual void BeginMap() {}
		//virtual void GetMapItem() {}
		virtual void EndMap() {}
		
		virtual std::size_t BeginSequence() = 0;
		virtual void EndSequence() = 0;
		
	protected:
		std::istream & m_istream;

		virtual void Deserialize(short & t) { m_istream >> t; }
		virtual void Deserialize(unsigned short & t) { m_istream >> t; }
		virtual void Deserialize(int & t) { m_istream >> t; }
		virtual void Deserialize(unsigned int & t) { m_istream >> t; }
		virtual void Deserialize(long & t) { m_istream >> t; }
		virtual void Deserialize(unsigned long & t) { m_istream >> t; }
		virtual void Deserialize(long long & t) { m_istream >> t; }
		virtual void Deserialize(unsigned long long & t) { m_istream >> t; }
		virtual void Deserialize(float & t) { m_istream >> t; }
		virtual void Deserialize(double & t) { m_istream >> t; }
		virtual void Deserialize(bool & t) { m_istream >> t; }
		virtual void Deserialize(std::string & t) { m_istream >> t; }
		//virtual void Serialize(const char* t) { m_istream >> t; }
		//virtual void Serialize(std::nullptr_t const & t) { }

		virtual void DeserializeObject(ObjectPtr const & obj) = 0;
		virtual void DeserializeWeakObject(std::weak_ptr<Object> const & obj) = 0;
		
		virtual std::size_t GetSizeTag() = 0;

		virtual void EndNVP() = 0;
		virtual void NameOfNVP(const char* name) = 0;
		virtual void MiddleOfNVP() = 0;
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

		OutputArchive & operator << (GUID const & t)
		{
			(*this) << ToString(t);
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
			BeginMap(t.size());
			for (auto & item : t)
			{
				(*this) << item.first << item.second;
			}
			EndMap();
			return *this;
		}

		virtual void BeginClass() {}
		virtual void EndClass() {}

		virtual void BeginMap(std::size_t mapSize) {}
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

		//template<class T>
		//void SerializeNVP (NameValuePair<T> const & nvp)
		//{
		//	SerializeNameOfNVP(nvp.name);
		//	MiddleOfNVP();
		//	(*this) << nvp.value;
		//	EndNVP();
		//}

		//template<class T, std::enable_if_t<std::is_base_of<Object, T>::value, int> = 0>
		//void SerializeNVP (NameValuePair<const std::shared_ptr<T> &> const & nvp)
		//{
		//	SerializeNameOfNVP(nvp.name);
		//	MiddleOfNVP();
		//	SerializeObject(nvp.value);
		//	EndNVP();
		//}

		virtual void EndNVP() = 0;
		virtual void SerializeNameOfNVP(const char* name) = 0;
		virtual void MiddleOfNVP() = 0;

		std::ostream & m_ostream;
	};
}
