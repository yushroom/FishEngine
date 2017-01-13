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
#include <sstream>

namespace FishEngine
{
	template<typename T>
	struct emittable : std::conditional_t <
		std::is_arithmetic<T>::value || std::is_enum<T>::value,
		std::true_type, std::false_type>
	{};

	namespace YAML
	{
		enum class State
		{
			None,
			InsideDoc,
			InsideMap,
			InsideSeq,
		};

		enum class NodeType
		{
			BeginDoc,
			EndOfDoc,
			BeginMap,
			EndOfMap,
			BeginSeq,
			EndOfSeq,
			MapValue,
			Value,
			FirstElement, // first element of a map/list
			None,
		};

		enum Manipulator
		{
			Flow,
			BeginDoc,
			EndDoc,
			BeginSeq,
			EndSeq,
			BeginMap,
			EndMap,
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
				ToNextStage(NodeType::Value);
				//m_stream << t;
				std::ostringstream sout;
				sout << t;
				m_col += sout.str().size();
				m_stream << sout.str();
				return *this;
			}


			void SetManipulator (Manipulator value)
			{
				switch (value)
				{
				case Manipulator::BeginDoc:
					ToNextStage(NodeType::BeginDoc);
					break;
				case Manipulator::EndDoc:
					ToNextStage(NodeType::EndOfDoc);
					break;
				case Manipulator::BeginMap:
					ToNextStage(NodeType::BeginMap);
					break;
				case Manipulator::EndMap:
					ToNextStage(NodeType::EndOfMap);
					break;
				case Manipulator::BeginSeq:
					ToNextStage(NodeType::BeginSeq);
					break;
				case Manipulator::EndSeq:
					ToNextStage(NodeType::EndOfSeq);
					break;
				default:
					break;
				}
			}

		private:
			std::ostream & m_stream;
			int m_indentLevel = 0;
			int m_col = 0;

			// parent node of current node (if exists)
			// State m_parentNode = State::Nono;
			State m_state = State::None;
			NodeType m_expectedType = NodeType::None;
			std::stack<State> m_stateStack;
			std::stack<NodeType> m_expectedTypeStack;

			void PrepareBeginMap()
			{
				m_indentLevel++;
				m_stateStack.push(m_state);
				m_expectedTypeStack.push(m_expectedType);
				m_state = State::InsideMap;
				m_expectedType = NodeType::FirstElement;
			}

			void PrepareMapKey()
			{
				assert(m_state == State::InsideMap);
				NewLineIfNeeded();
				Indent();
				m_expectedType = NodeType::MapValue;
			}

			void PrepareMapValue()
			{
				assert(m_state == State::InsideMap);
				m_stream << ": ";
				m_expectedType = NodeType::EndOfMap;
			}
			
			void PrepareEndMap()
			{
				assert(m_state == State::InsideMap);
				if (m_indentLevel >= 1)
					m_indentLevel--;
				m_state = m_stateStack.top();
				m_stateStack.pop();
				m_expectedType = m_expectedTypeStack.top();
				m_expectedTypeStack.pop();
			}

			void PrepareBeginSeq()
			{
				m_indentLevel++;
				m_stateStack.push(m_state);
				m_expectedTypeStack.push(m_expectedType);
				m_state = State::InsideSeq;
				m_expectedType = NodeType::FirstElement;
			}

//			void PrepareSeqElement()
//			{
//				//m_stream << ", ";
//				NewLineIfNeeded();
//				Indent();
//			}

			void PrepareEndSeq()
			{
				assert(m_state == State::InsideSeq);
				if (m_indentLevel >= 1)
					m_indentLevel--;
				m_state = m_stateStack.top();
				m_stateStack.pop();
				m_expectedType = m_expectedTypeStack.top();
				m_expectedTypeStack.pop();
			}

			void ToNextStage(NodeType input)
			{
				if (m_state == State::None)
				{
					assert(m_expectedType == NodeType::None);
					switch (input)
					{
					case NodeType::BeginDoc:
						NewLineIfNeeded();
						m_stream << "---";
						m_col += 3;
						m_state = State::InsideDoc;
						m_expectedType = NodeType::EndOfDoc;
						break;
					case NodeType::BeginMap:
						PrepareBeginMap();
						break;
					case NodeType::BeginSeq:
						PrepareBeginSeq();
						break;
					default:
						abort();
					}
				}
				else if (m_state == State::InsideDoc)
				{
					assert(m_expectedType == NodeType::EndOfDoc);
					switch (input)
					{
					case NodeType::EndOfDoc:
						// EmitEndDoc();
						NewLineIfNeeded();
						m_stream << "...";
						m_col += 3;
						m_state = State::None;
						m_expectedType = NodeType::None;
						break;
					case NodeType::BeginMap:
						PrepareBeginMap();
						break;
					case NodeType::BeginSeq:
						PrepareBeginSeq();
						break;
					default:
						abort();
					}
				}
				else if (m_state == State::InsideMap)
				{
					if (m_expectedType == NodeType::EndOfMap || m_expectedType == NodeType::FirstElement)
					{
						if (input == NodeType::EndOfMap)
						{
							// PrepareEndMap();
							if (m_expectedType == NodeType::FirstElement)
							{
								//m_expectedType = NodeType::EndOfMap;
								NewLineIfNeeded();
								Indent();
								m_stream << "{}";
								m_col += 2;
							}
							if (m_indentLevel >= 1)
								m_indentLevel--;
							m_state = m_stateStack.top();
							m_stateStack.pop();
							m_expectedType = m_expectedTypeStack.top();
							m_expectedTypeStack.pop();
						}
						switch (input)
						{
							case NodeType::EndOfMap:
								break;
							case NodeType::Value:
								PrepareMapKey();
								break;
							case NodeType::BeginMap:
								PrepareBeginMap();
								break;
							case NodeType::BeginSeq:
								PrepareBeginSeq();
								break;
							default:
								abort();
						}
					}
					else if (m_expectedType == NodeType::MapValue)
					{
						m_expectedType = NodeType::EndOfMap;
						switch (input)
						{
						case NodeType::Value:
							PrepareMapValue();
							break;
						case NodeType::BeginMap:
                            m_stream << ": ";
                            m_col += 2;
							PrepareBeginMap();
							break;
						case NodeType::BeginSeq:
							m_stream << ": ";
							m_col += 2;
							PrepareBeginSeq();
							break;
						default:
							abort();
						}
					}
					else
					{
						abort();
					}
				}
				else if (m_state == State::InsideSeq)
				{
					if (m_expectedType == NodeType::EndOfSeq || m_expectedType == NodeType::FirstElement)
					{
						m_expectedType = NodeType::EndOfSeq;
						if (input == NodeType::EndOfSeq)
						{
							if (m_expectedType == NodeType::FirstElement)
							{
								NewLineIfNeeded();
								Indent();
								m_stream << "[]";
								m_col += 2;
							}
							if (m_indentLevel >= 1)
								m_indentLevel--;
							m_state = m_stateStack.top();
							m_stateStack.pop();
							m_expectedType = m_expectedTypeStack.top();
							m_expectedTypeStack.pop();
						}
						switch (input)
						{
						case NodeType::EndOfSeq:
							//PrepareEndSeq();
							break;
						case NodeType::Value:
							//PrepareSeqElement();
							NewLineIfNeeded();
							Indent();
							//m_stream << "- ";
							//m_col += 2;
							break;
						case NodeType::BeginMap:
							PrepareBeginMap();
							//NewLineIfNeeded();
							//Indent();
							//m_stream << "- ";
							//m_col += 2;
							break;
						default:
							abort();
						}
					}
					else
					{
						abort();
					}
				}
			}

			void Indent()
			{
				assert(m_col == 0);
				if (m_indentLevel == 0)
				{
					return;
				}

				for (int i = 0; i < m_indentLevel-1; ++i)
				{
					m_stream << "  ";
					m_col += 2;
				}
				// parent node is a list
				if (!m_stateStack.empty() && m_stateStack.top() == State::InsideSeq)
				{
					m_stream << "- ";
					m_col += 2;
				}
				else
				{
					m_stream << "  ";
					m_col += 2;
				}
			}

			void NewLineIfNeeded()
			{
				if (m_col != 0)
				{
					m_stream << "\n";
					m_col = 0;
				}
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

		void SetManipulator(YAML::Manipulator value)
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
