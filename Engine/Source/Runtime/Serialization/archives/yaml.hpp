#pragma once

//#include "ReflectClass.hpp"
#include <yaml-cpp/yaml.h>
#include <boost/uuid/uuid_io.hpp>
#include <cassert>

namespace FishEngine
{
	template<typename T>
	struct emittable : std::conditional_t <
		std::is_arithmetic<T>::value || std::is_enum<T>::value,
		std::true_type, std::false_type>
	{};

	class YAMLOutputArchive;
	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, FishEngine::UUID const & t);
	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, std::nullptr_t const & t);

	class YAMLOutputArchive
	{
	public:
		YAMLOutputArchive(YAML::Emitter & emitter) : m_emitter(emitter)
		{

		}
        
        YAMLOutputArchive(YAMLOutputArchive const &) = delete;
		YAMLOutputArchive& operator = (YAMLOutputArchive const &) = delete;

		~YAMLOutputArchive() noexcept = default;
        
		template<typename T, std::enable_if_t<!emittable<T>::value, int> = 0>
		YAMLOutputArchive & operator << (T const & t)
		{
			prologue(*this, t);
			Save(*this, t);
			epilogue(*this, t);
			return *this;
		}

		template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, int> = 0>
		YAMLOutputArchive & operator << (T const & t)
		{
			m_emitter << t;
			return *this;
		}

		template<typename T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
		YAMLOutputArchive & operator << (T const & t)
		{
			m_emitter << static_cast<std::underlying_type_t<T>>(t);
			return *this;
		}


		YAMLOutputArchive & operator << (std::string const & t)
		{
			m_emitter << t;
			return *this;
		}

		YAMLOutputArchive & operator << (const char* t)
		{
			m_emitter << t;
			return *this;
		}

		YAMLOutputArchive & operator << (const wchar_t* t)
		{
			m_emitter << t;
			return *this;
		}

		void SerializeObject(std::shared_ptr<Object> const & obj)
		{
			if (obj == nullptr)
			{
				(*this) << nullptr;
				return;
			}

			//std::cout << "SerializeObject: " << obj->ClassName() << std::endl;

			auto guid = obj->GetGUID();
			auto find_result = m_serialized.find(guid);
			if (find_result != m_serialized.end() && find_result->second)
			{
				if (m_isInsideDoc)
					(*this) << guid;
				return;
			}

			if (m_isInsideDoc)
			{
				m_objectsToBeSerialized.push_back(obj);
                //std::cout << "SerializeObject: push, size=" << m_objectsToBeSerialized.size() << std::endl;
				(*this) << guid;
			}
			else
			{
                m_serialized[guid] = true;
				SetManipulator(YAML::BeginDoc);
                m_emitter << YAML::LocalTag("u", boost::uuids::to_string(guid));
                //assert(m_emitter.good());
				//SetManipulator(YAML::BeginMap);
				m_emitter << obj->ClassName();
				//this->operator<<(*obj);
				DynamicSerializeObject(*this, obj);
				//SetManipulator(YAML::EndMap);
				SetManipulator(YAML::EndDoc);
			}
		}

		void SetManipulator(YAML::EMITTER_MANIP value)
		{
            m_emitter << value;
            //assert(m_emitter.good());
            
			if (value == YAML::BeginDoc)
			{
				if (m_isInsideDoc)
					abort();
				m_isInsideDoc = true;
			}
			else if (value == YAML::EndDoc)
			{
                //std::cout << "EndDoc size=" << m_objectsToBeSerialized.size() << std::endl;
				m_isInsideDoc = false;
				if (!m_objectsToBeSerialized.empty())
				{
					auto obj = m_objectsToBeSerialized.back();
					m_objectsToBeSerialized.pop_back();
					SerializeObject(obj);
				}
			}
		}
	
	private:
		YAML::Emitter & m_emitter;
		std::map<UUID, bool> m_serialized;
		std::vector<std::shared_ptr<Object>> m_objectsToBeSerialized;
		bool m_isInsideDoc = false;
	};

	// If you want to call prologue and epilogue automatically, override Save(archive, T) for type T,
	// otherwise override operator<< directly.

	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, std::nullptr_t const & t)
	{
		archive.SetManipulator(YAML::Flow);
		archive.SetManipulator(YAML::BeginMap);
		archive << make_nvp("fileId", 0);
		archive.SetManipulator(YAML::EndMap);
		return archive;
	}

	void Save    (YAMLOutputArchive & archive, std::nullptr_t const & t) = delete;
	void prologue(YAMLOutputArchive & archive, std::nullptr_t const & t) = delete;

	template <class T>
	inline void Save(YAMLOutputArchive & archive, SizeTag<T> const & t)
	{
		// empty
	}

	template<typename T>
	static void SaveSequence(YAMLOutputArchive& archive, T const & v)
	{
		if (v.empty())
			archive.SetManipulator(YAML::Flow);
		archive.SetManipulator(YAML::BeginSeq);
		for (auto & x : v)
			archive << x;
		archive.SetManipulator(YAML::EndSeq);
	}

	template <class T>
	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, std::list<T> const & t)
	{
		SaveSequence(archive, t);
		return archive;
	}

	template <class T>
	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, std::vector<T> const & t)
	{
		SaveSequence(archive, t);
		return archive;
	}

	template <class T>
	inline void Save    (YAMLOutputArchive & archive, std::list<T>   const & t) = delete;

	template <class T>
	inline void Save    (YAMLOutputArchive & archive, std::vector<T> const & t) = delete;

	template<typename T>
	inline void prologue(YAMLOutputArchive & archive, std::list<T>   const & t) = delete;

	template <class T>
	inline void epilogue(YAMLOutputArchive & archive, std::list<T>   const & t) = delete;

	template<typename T>
	inline void prologue(YAMLOutputArchive & archive, std::vector<T> const & t) = delete;

	template <class T>
	inline void epilogue(YAMLOutputArchive & archive, std::vector<T> const & t) = delete;

#if 1
	template <class T, class B>
	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, std::map<T, B> const & t)
	{
		if (t.empty())
			archive.SetManipulator(YAML::Flow);
		archive.SetManipulator(YAML::BeginMap);
		for (auto & p : t)
		{
			archive << p.first << p.second;
		}
		archive.SetManipulator(YAML::EndMap);
		return archive;
	}

	template <class T, class B>
	inline void Save(YAMLOutputArchive & archive, std::map<T, B> const & t) = delete;

	template <class T, class B>
	inline void epilogue(YAMLOutputArchive & archive, std::map<T, B> const & t) = delete;

	template <class T, class B>
	inline void prologue(YAMLOutputArchive & archive, std::map<T, B> const & t) = delete;
#endif

	/************************************************************************/
	/* std::shared_ptr                                                      */
	/************************************************************************/
	template<typename T, std::enable_if_t<std::is_base_of<Object, T>::value, int> = 0>
	static void Save(YAMLOutputArchive& archive, std::shared_ptr<T> const & t)
	{
		archive.SerializeObject(t);
	}

	template<typename T, std::enable_if_t<!std::is_base_of<Object, T>::value, int> = 0>
	static void Save(YAMLOutputArchive& archive, std::shared_ptr<T> const & t) = delete;

	/************************************************************************/
	/* std::weak_ptr                                                        */
	/************************************************************************/
	template<typename T>
	inline void Save(YAMLOutputArchive& archive, std::weak_ptr<T> const & v)
	{
		auto t = v.lock();
		archive.SerializeObject(t);
	}

	/************************************************************************/
	/* UUID                                                                 */
	/************************************************************************/
	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, FishEngine::UUID const & t)
	{
		archive.SetManipulator(YAML::Flow);
		archive.SetManipulator(YAML::BeginMap);
		archive << make_nvp("fileId", boost::uuids::to_string(t));
		archive.SetManipulator(YAML::EndMap);
		return archive;
	}

	// explicitly delete these function
	void Save    (YAMLOutputArchive & archive, FishEngine::UUID const & t) = delete;
	void prologue(YAMLOutputArchive & archive, FishEngine::UUID const & t) = delete;
	void epilogue(YAMLOutputArchive & archive, FishEngine::UUID const & t) = delete;

	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, Path const & t)
	{
		archive << "path[TODO]";
		return archive;
	}

	template <class T>
	inline void prologue(YAMLOutputArchive & archive, T const & t)
	{
		//archive.m_emitter << YAML::Block;
		archive.SetManipulator(YAML::BeginMap);
	}

	template <class T>
	inline void epilogue(YAMLOutputArchive& archive, T const & t)
	{
		archive.SetManipulator(YAML::EndMap);
	}

	/************************************************************************/
	/* BaseClassWrapper                                                     */
	/************************************************************************/
	template <class T>
	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, BaseClassWrapper<T> const & t)
	{
		// call Save() directly to avoid prologue and epilogue
		Save(archive, t.base_ref);
		return archive;
	}
	
	template <class T>
	void Save    (YAMLOutputArchive & archive, BaseClassWrapper<T> const & t) = delete;
    
    template <class T>
	void prologue(YAMLOutputArchive & archive, BaseClassWrapper<T> const & t) = delete;

    template <class T>
	void epilogue(YAMLOutputArchive & archive, BaseClassWrapper<T> const & t) = delete;


	inline void prologue(YAMLOutputArchive& archive, Vector3 const & t)
	{
		archive.SetManipulator(YAML::Flow);
		archive.SetManipulator(YAML::BeginMap);
	}

	inline void prologue(YAMLOutputArchive& archive, Vector4 const & t)
	{
		archive.SetManipulator(YAML::Flow);
		archive.SetManipulator(YAML::BeginMap);
	}

	inline void prologue(YAMLOutputArchive& archive, Quaternion const & t)
	{
		archive.SetManipulator(YAML::Flow);
		archive.SetManipulator(YAML::BeginMap);
	}

	template <class T>
	inline void prologue(YAMLOutputArchive & archive, NameValuePair<T> const & t)
	{
		//archive.m_emitter << YAML::BeginMap;
	}

	template <class T>
	inline void epilogue(YAMLOutputArchive& archive, NameValuePair<T> const & t)
	{
		//archive.m_emitter << YAML::EndMap;
	}

	template <class T>
	inline void prologue(YAMLOutputArchive & archive, std::weak_ptr<T> const & t)
	{
		//archive.m_emitter << YAML::BeginMap;
	}

	template <class T>
	inline void epilogue(YAMLOutputArchive& archive, std::weak_ptr<T> const & t)
	{
		//archive.m_emitter << YAML::EndMap;
	}

	template <class T>
	inline void prologue(YAMLOutputArchive & archive, std::shared_ptr<T> const & t)
	{
		//archive.m_emitter << YAML::BeginMap;
	}

	template <class T>
	inline void epilogue(YAMLOutputArchive& archive, std::shared_ptr<T> const & t)
	{
		//archive.m_emitter << YAML::EndMap;
	}
}
