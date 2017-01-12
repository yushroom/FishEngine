#pragma once

//#include "ReflectClass.hpp"
//#include <yaml-cpp/yaml.h>
#include "Object.hpp"
#include "Serialization/NameValuePair.hpp"
#include "Serialization/helper.hpp"
#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <cassert>
#include <stack>
#include <memory>
#include <iostream>

namespace FishEngine
{
	template<typename T>
	struct emittable : std::conditional_t <
		std::is_arithmetic<T>::value || std::is_enum<T>::value,
		std::true_type, std::false_type>
	{};

	namespace YAML
	{
		enum EMITTER_MANIP
		{
			Flow,
			BeginDoc,
			EndDoc,
			BeginSeq,
			EndSeq,
			BeginMap,
			EndMap,
		};

		enum class YAMLNodeType
		{
			None,
			DocBegin,
			DocEnd,
			MapBegin,
			MapKey,
			MapValue,
			//MapEnd,
			SeqBegin,
			SeqInside,
			//SeqEnd,
		};

		class Emitter
		{
		public:
			Emitter(std::ostream & stream) : m_stream(stream)
			{

			}

			template<class T>
			Emitter & operator << (T const & t)
			{
				switch (m_expectedNextNodeType)
				{
				case YAMLNodeType::MapKey:
					NewLineIfNeeded();
					Intent();
					break;
				case YAMLNodeType::MapValue:
					break;
				case YAMLNodeType::SeqBegin:
					// fall through
				case YAMLNodeType::SeqInside:
					NewLineIfNeeded();
					Intent();
					m_stream << "-";
					m_col = 1;
				default:
					break;
				}
				m_stream << t;
				if (m_expectedNextNodeType == YAMLNodeType::MapKey)
				{
					m_stream << ": ";
				}
				m_col = 1;
				ToNextStage();
				return *this;
			}


			void SetManipulator (EMITTER_MANIP value)
			{
				if (value == YAML::BeginDoc)
				{
					assert(m_nodeTypeStack.empty());
					//PushState(YAMLNodeType::DocEnd);
					NewLineIfNeeded();
					m_stream << "---\n";
					m_col = 0;
					//m_expectedNextNodeType = YAMLNodeType::None;
					m_expectedNextNodeType = YAMLNodeType::DocEnd;
				}
				else if (value == YAML::EndDoc)
				{
					assert(m_expectedNextNodeType == YAMLNodeType::DocEnd);
					assert(m_nodeTypeStack.empty());
					NewLineIfNeeded();
					m_stream << "...\n";
					m_col = 0;
					m_expectedNextNodeType = YAMLNodeType::None;
				}
				else if (value == YAML::BeginMap)
				{
					if (m_expectedNextNodeType != YAMLNodeType::DocEnd)
					{
						m_indentLevel++;
					}
					PushState(m_expectedNextNodeType);
					PushState(YAMLNodeType::MapBegin);
					NewLineIfNeeded();
					//m_intentLevel++;
					m_expectedNextNodeType = YAMLNodeType::MapKey;
				}
				else if (value == YAML::EndMap)
				{
					assert(m_expectedNextNodeType == YAMLNodeType::MapKey);
					PopLastStateAndCheck(YAMLNodeType::MapBegin);
					m_expectedNextNodeType = PopLastState();
					m_indentLevel--;
					if (m_indentLevel < 0)
						m_indentLevel = 0;
				}
				else if (value == YAML::BeginSeq)
				{
					//if (m_expectedNextNodeType != YAMLNodeType::DocBegin)
						m_indentLevel++;
					PushState(m_expectedNextNodeType);
					PushState(YAMLNodeType::SeqBegin);
					NewLineIfNeeded();
					//m_intentLevel++;
					m_expectedNextNodeType = YAMLNodeType::SeqInside;
				}
				else if (value == YAML::EndSeq)
				{
					if (m_expectedNextNodeType == YAMLNodeType::SeqBegin)
					{
						Intent();
						m_stream << "[]";
						m_col = 1;
					}
					else
					{
						assert(m_expectedNextNodeType == YAMLNodeType::SeqInside);
					}
					//assert(PopLastState() == YAMLNodeType::MapBegin);
					PopLastStateAndCheck(YAMLNodeType::SeqBegin);
					m_expectedNextNodeType = PopLastState();
					m_indentLevel--;
				}
			}

		private:
			std::ostream & m_stream;
			int m_indentLevel = 0;
			//int m_row;
			int m_col = 0;
			YAMLNodeType m_expectedNextNodeType = YAMLNodeType::None;
			std::stack<YAMLNodeType> m_nodeTypeStack;

			void ToNextStage()
			{
				switch (m_expectedNextNodeType)
				{
				case YAMLNodeType::MapKey:
					//PushState(YAMLNodeType::MapKey);
					m_expectedNextNodeType = YAMLNodeType::MapValue;
					break;
				case YAMLNodeType::MapValue:
					NewLineIfNeeded();
					//assert(PopLastState() == YAMLNodeType::MapKey);
					m_expectedNextNodeType = YAMLNodeType::MapKey;
					//PushState(YAMLNodeType::MapKey);
					break;
				case YAMLNodeType::SeqBegin:
					//PushState(m_expectedNextNodeType);
					m_expectedNextNodeType = YAMLNodeType::SeqInside;
					break;
				case YAMLNodeType::SeqInside:
					// the first element of the sequence
					if (m_nodeTypeStack.top() == YAMLNodeType::SeqBegin)
					{
						PushState(YAMLNodeType::SeqInside);
					}
					//m_nextNodeType = YAMLNodeType::SeqInside;
					break;
				default:
					m_expectedNextNodeType = YAMLNodeType::None;
					break;
				}
			}

			void Intent()
			{
				assert(m_col == 0);
				for (int i = 0; i < m_indentLevel; ++i)
					m_stream << "  ";
			}

			void NewLineIfNeeded()
			{
				if (m_col != 0)
				{
					m_stream << "\n";
					m_col = 0;
				}
			}

			void PushState(YAMLNodeType state)
			{
				if (state == YAMLNodeType::MapValue)
					state = YAMLNodeType::MapKey;
				if (state != YAMLNodeType::None)
				{
					if (state == YAMLNodeType::SeqBegin)
					{
						int i = 1;
					}
					if (state == YAMLNodeType::SeqInside)
					{
						int i = 1;
					}
					//std::cout << "\n" << static_cast<std::underlying_type_t<YAMLNodeType>>(state) << "\n";
					m_nodeTypeStack.push(state);
				}
			}

			YAMLNodeType PopLastState()
			{
				YAMLNodeType ret = YAMLNodeType::None;
				if (!m_nodeTypeStack.empty())
				{
					ret = m_nodeTypeStack.top();
					m_nodeTypeStack.pop();
				}
				return ret;
			}

			void PopLastStateAndCheck(YAMLNodeType expected)
			{
				auto state = m_nodeTypeStack.top();
				auto size = m_nodeTypeStack.size();
				m_nodeTypeStack.pop();
				if (state != expected)
					abort();
			}

		};
	}

	class YAMLOutputArchive;
	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, boost::uuids::uuid const & t);
	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, std::nullptr_t const & t);

	class YAMLOutputArchive
	{
	public:
        YAMLOutputArchive(std::ostream & emitter) : m_emitter(emitter)
		{

		}
        
        YAMLOutputArchive(YAMLOutputArchive const &) = delete;
		YAMLOutputArchive& operator = (YAMLOutputArchive const &) = delete;

		~YAMLOutputArchive() = default;
        
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

		void SerializeObject(std::shared_ptr<Object> const & obj);

		void SetManipulator(YAML::EMITTER_MANIP value)
		{
            m_emitter.SetManipulator(value);
            
			if (value == YAML::BeginDoc)
			{
				assert(!m_isInsideDoc);
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
		YAML::Emitter m_emitter;
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
	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, boost::uuids::uuid const & t)
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

	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, boost::filesystem::path const & t)
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
