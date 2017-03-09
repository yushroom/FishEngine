#pragma once

#include <type_traits>

#define USE_yaml_cpp 1

//#include "ReflectClass.hpp"
#if USE_yaml_cpp
#include <yaml-cpp/yaml.h>
#endif
#include "Object.hpp"
#include <Serialization/NameValuePair.hpp>
#include <Serialization/helper.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <cassert>
#include <stack>
#include <memory>
#include <iostream>
#include <sstream>

#include <Archive.hpp>

namespace FishEngine
{
	template<typename T>
	struct emittable : std::conditional_t <
		std::is_arithmetic<T>::value || std::is_enum<T>::value,
		std::true_type, std::false_type>
	{};

#if !USE_yaml_cpp
	namespace YAML
	{
	//#define DebugYAML(str)	m_stream << str
	#define DebugYAML(str)
		
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
			MapKey,
			MapValue,
			Value,
			FirstElement, // first element of a map/list
			None,
		};

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

		struct LocalState
		{
			State		state;
			NodeType	nodeType;
			bool		indent;	// for unindent
			bool		flow;
		};

		class Emitter
		{
		public:
			Emitter(std::ostream & stream) : m_stream(stream)
			{
				m_stream << "%YAML 1.1\n%TAG !u! tag:FishEngine:\n";
			}

			template<class T>
			Emitter & operator << (T const & t)
			{
				ToNextStage(NodeType::Value);
				//m_stream << t;
				std::ostringstream sout;
				sout << t;
				m_col += static_cast<int>(sout.str().size());
				m_stream << sout.str();
				if (m_state == State::InsideMap && m_expectedType == NodeType::MapValue)
				{
					m_stream << ": ";
					m_col += 2;
				}
				return *this;
			}

			Emitter & operator << (EMITTER_MANIP value)
			{
				switch (value)
				{
				case EMITTER_MANIP::BeginDoc:
					DebugYAML("[BeginDoc]");
					ToNextStage(NodeType::BeginDoc);
					break;
				case EMITTER_MANIP::EndDoc:
					DebugYAML("[EndDoc]");
					ToNextStage(NodeType::EndOfDoc);
					break;
				case EMITTER_MANIP::BeginMap:
					DebugYAML("[BeginMap]");
					ToNextStage(NodeType::BeginMap);
					break;
				case EMITTER_MANIP::EndMap:
					DebugYAML("[EndMap]");
					ToNextStage(NodeType::EndOfMap);
					break;
				case EMITTER_MANIP::BeginSeq:
					DebugYAML("[BeginSeq]");
					ToNextStage(NodeType::BeginSeq);
					break;
				case EMITTER_MANIP::EndSeq:
					DebugYAML("[EndSeq]");
					ToNextStage(NodeType::EndOfSeq);
					break;
				case EMITTER_MANIP::Flow:
					DebugYAML("[Flow]");
					m_nextNodeIsFlow = true;
				default:
					break;
				}
			}

		private:
			std::ostream & m_stream;
			int m_indentLevel = 0;
			int m_col = 0;
			bool m_nextNodeIsFlow = false;
			bool m_flowMode = false;

			// parent node of current node (if exists)
			// State m_parentNode = State::Nono;
			State m_state = State::None;
			NodeType m_expectedType = NodeType::None;
			//std::stack<State> m_stateStack;
			//std::stack<NodeType> m_expectedTypeStack;
			//std::stack<bool> m_indentStack;
			LocalState m_nextState = { State::None, NodeType::None, false, false};
			LocalState m_currentState = { State::None, NodeType::None, false, false};
			std::stack<LocalState> m_localStateStack;

			void PopStates()
			{
				auto s = m_localStateStack.top();
				m_localStateStack.pop();
				m_state			= s.state;
				m_expectedType	= s.nodeType;
				m_flowMode		= s.flow;
				bool unindent	= s.indent;
				if (unindent)
				{
					m_indentLevel--;
					DebugYAML("[indent--]");
				}
			}
//			
//			void PrepareNewMapOrSeq(bool isMap)
//			{
//				
//			}

			void PrepareNewMapBegin(bool addIndent)
			{
				if (addIndent)
				{
					m_indentLevel++;
					DebugYAML("[indent++]");
				}
				m_localStateStack.emplace(LocalState{ m_state, m_expectedType, addIndent, m_flowMode });
				m_flowMode = m_nextNodeIsFlow;
				m_nextNodeIsFlow = false;
				m_state = State::InsideMap;
				m_expectedType = NodeType::FirstElement;
			}

			void PrepareNewSeqBegin(bool addIndent)
			{
				if (addIndent)
				{
					m_indentLevel++;
					DebugYAML("[indent++]");
				}
				m_localStateStack.emplace(LocalState{ m_state, m_expectedType, addIndent, m_flowMode });
				m_flowMode = m_nextNodeIsFlow;
				m_nextNodeIsFlow = false;
				m_state = State::InsideSeq;
				m_expectedType = NodeType::FirstElement;
			}

			void PrepareNoneNode(NodeType input)
			{
				assert(m_expectedType == NodeType::None);
				switch (input)
				{
				case NodeType::BeginDoc:
					NewLineIfNeeded();
					m_stream << "---\n";
					m_col = 0;
					m_state = State::InsideDoc;
					m_expectedType = NodeType::EndOfDoc;
					break;
				case NodeType::BeginMap:
					PrepareNewMapBegin(false);
					break;
				case NodeType::BeginSeq:
					PrepareNewSeqBegin(false);
					break;
				default:
					abort();
				}
			}

			void PrepareDocNode(NodeType input)
			{
				assert(m_expectedType == NodeType::EndOfDoc);
				switch (input)
				{
				case NodeType::EndOfDoc:
					// EmitEndDoc();
					NewLineIfNeeded();
					m_stream << "...\n";
					m_col = 0;
					m_state = State::None;
					m_expectedType = NodeType::None;
					m_flowMode = false;
					break;
				case NodeType::BeginMap:
					PrepareNewMapBegin(false);
					break;
				case NodeType::BeginSeq:
					PrepareNewSeqBegin(false);
					break;
				default:
					abort();
				}
			}

			void PrepareMapNode(NodeType input)
			{
				if (m_expectedType == NodeType::EndOfMap || m_expectedType == NodeType::FirstElement)
				{
					// end of map
					if (input == NodeType::EndOfMap)
					{
						// output empty map
						if (m_expectedType == NodeType::FirstElement)
						{
//							if (!m_flowMode)
//							{
//								NewLineIfNeeded();
//								OutputIndent();
//							}
							m_stream << "{}";
							m_col += 2;
						}
						else
						{
							if (m_flowMode) {
								m_stream << "}";
								m_col += 2;
							}
						}
						PopStates();
						//m_flowMode = false;
					}
					switch (input)
					{
					case NodeType::EndOfMap:
						break;
					case NodeType::Value:
						//PrepareMapKey();
						if (!m_flowMode)
						{
							NewLineIfNeeded();
							OutputIndent();
						}
						else
						{
							if (m_expectedType == NodeType::FirstElement)
								m_stream << "{";
							else
								m_stream << ", ";
							m_col += 2;
						}
						m_expectedType = NodeType::MapValue;
						break;
					case NodeType::BeginMap:
						PrepareNewMapBegin(true);
						break;
					case NodeType::BeginSeq:
						PrepareNewSeqBegin(true);
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
						//PrepareMapValue();
						m_expectedType = NodeType::EndOfMap;
						break;
					case NodeType::BeginMap:
						PrepareNewMapBegin(true);
						break;
					case NodeType::BeginSeq:
						PrepareNewSeqBegin(true);
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

			void PrepareSeqNode(NodeType input)
			{
				if (m_expectedType == NodeType::EndOfSeq || m_expectedType == NodeType::FirstElement)
				{
					if (input == NodeType::EndOfSeq)
					{
						// output empty list
						if (m_expectedType == NodeType::FirstElement)
						{
//							if (!m_flowMode)
//							{
//								NewLineIfNeeded();
//								OutputIndent();
//							}
							m_stream << "[]";
							m_col += 2;
						}
						PopStates();
						//m_flowMode = false;
					}

					switch (input)
					{
					case NodeType::EndOfSeq:
						//PrepareEndSeq();
						break;
					case NodeType::Value:
						//PrepareSeqElement();
						if (!m_flowMode)
						{
							NewLineIfNeeded();
							OutputIndent();
						}
						m_expectedType = NodeType::EndOfSeq;
						//OutputIndent2();
						//m_stream << "- ";
						break;
					case NodeType::BeginMap:
						if (!m_flowMode)
						{
							NewLineIfNeeded();
							OutputIndent();
						}
						m_expectedType = NodeType::EndOfSeq;
						PrepareNewMapBegin(false);
						break;
					case NodeType::BeginSeq:
						if (!m_flowMode)
						{
							NewLineIfNeeded();
							OutputIndent();
						}
						m_expectedType = NodeType::EndOfSeq;
						PrepareNewSeqBegin(false);
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

			void ToNextStage(NodeType input)
			{
				if (m_state == State::None)
				{
					PrepareNoneNode(input);
				}
				else if (m_state == State::InsideDoc)
				{
					PrepareDocNode(input);
				}
				else if (m_state == State::InsideMap)
				{
					PrepareMapNode(input);
				}
				else if (m_state == State::InsideSeq)
				{
					PrepareSeqNode(input);
				}
			}
			
			void AfterOutputNode()
			{
				
			}

			void OutputIndent()
			{
				assert(m_col == 0);
				// 1. element is arithmetic or a string
				// 2. key of a map && parent node is a list
				bool isBeginOfAListElement =
					(m_state == State::InsideSeq) ||
					(m_state ==State::InsideMap && m_expectedType != NodeType::EndOfMap && !m_localStateStack.empty() && m_localStateStack.top().state == State::InsideSeq);
				
				if (m_indentLevel == 0)
				{
					return;
				}

				for (int i = 0; i < m_indentLevel-1; ++i)
				{
					m_stream << "  ";
					m_col += 2;
				}
				
				if (isBeginOfAListElement)
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
		
#undef DebugYAML
	}
#endif
	 
	class YAMLOutputArchive;
	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, boost::uuids::uuid const & t);
	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, std::nullptr_t const & t);

	template <>
	constexpr int ArchiveID<YAMLOutputArchive>()
	{
		return 1;
	}

	class YAMLOutputArchive : public OutputArchive
	{
	public:
		
		//virtual int ArchiveID()
		//{
		//	return FishEngine::ArchiveID<YAMLOutputArchive>();
		//}
		
		YAMLOutputArchive(std::ostream & emitter) : OutputArchive(emitter), m_emitter(emitter)
		{
			
		}
		
		YAMLOutputArchive(YAMLOutputArchive const &) = delete;
		YAMLOutputArchive& operator = (YAMLOutputArchive const &) = delete;

		~YAMLOutputArchive() = default;
		
//		template<typename T, std::enable_if_t<!emittable<T>::value, int> = 0>
//		YAMLOutputArchive & operator << (T const & t)
//		{
//			prologue(*this, t);
//			//Save(*this, t);
//			//operator<<(T, t);
//			t.Serialize(*this);
//			epilogue(*this, t);
//			return *this;
//		}

		YAMLOutputArchive & operator << (short t)
		{
			m_emitter << t;
			return *this;
		}

		YAMLOutputArchive & operator << (unsigned short t)
		{
			m_emitter << t;
			return *this;
		}

		YAMLOutputArchive & operator << (int t)
		{
			m_emitter << t;
			return *this;
		}

		YAMLOutputArchive & operator << (unsigned int t)
		{
			m_emitter << t;
			return *this;
		}

		YAMLOutputArchive & operator << (long t)
		{
			m_emitter << t;
			return *this;
		}

		YAMLOutputArchive & operator << (unsigned long t)
		{
			m_emitter << t;
			return *this;
		}

		YAMLOutputArchive & operator << (long long t)
		{
			m_emitter << t;
			return *this;
		}

		YAMLOutputArchive & operator << (unsigned long long t)
		{
			m_emitter << t;
			return *this;
		}

		YAMLOutputArchive & operator << (float t)
		{
			m_emitter << t;
			return *this;
		}

		YAMLOutputArchive & operator << (double t)
		{
			m_emitter << t;
			return *this;
		}

		YAMLOutputArchive & operator << (bool t)
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

		//YAMLOutputArchive & operator << (const wchar_t* t)
		//{
		//	m_emitter << t;
		//	return *this;
		//}

		void SerializeObject(std::shared_ptr<Object> const & obj);

		void SetManipulator(YAML::EMITTER_MANIP value)
		{
			//m_emitter.SetManipulator(value);
			m_emitter << value;
			
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
		std::map<int, bool> m_serialized;	// instanceID
		std::vector<std::shared_ptr<Object>> m_objectsToBeSerialized;
		bool m_isInsideDoc = false;
	};


	// If you want to call prologue and epilogue automatically, override Save(archive, T) for type T,
	// otherwise override operator<< directly.

	template <class T>
	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, NameValuePair<T> const & t)
	{
		//archive.SetManipulator(YAML::BeginMap);
		archive << t.name << t.value;
		//archive.SetManipulator(YAML::EndMap);
		return archive;
	}
	
	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, std::nullptr_t const &)
	{
		archive.SetManipulator(::YAML::Flow);
		archive.SetManipulator(::YAML::BeginMap);
		archive << make_nvp("fileID", 0);
		archive.SetManipulator(::YAML::EndMap);
		return archive;
	}


	template<typename T>
	static void SaveSequence(YAMLOutputArchive& archive, T const & v)
	{
		if (v.empty())
			archive.SetManipulator(::YAML::Flow);
		archive.SetManipulator(::YAML::BeginSeq);
		for (auto & x : v)
			archive << x;
		archive.SetManipulator(::YAML::EndSeq);
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

	template <class T, class B>
	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, std::map<T, B> const & t)
	{
		if (t.empty())
			archive.SetManipulator(::YAML::Flow);
		archive.SetManipulator(::YAML::BeginMap);
		for (auto & p : t)
		{
			archive << p.first << p.second;
		}
		archive.SetManipulator(::YAML::EndMap);
		return archive;
	}

	/************************************************************************/
	/* std::shared_ptr                                                      */
	/************************************************************************/
	template<typename T, std::enable_if_t<std::is_base_of<Object, T>::value, int> = 0>
	static void operator << (YAMLOutputArchive& archive, std::shared_ptr<T> const & t)
	{
		archive.SerializeObject(t);
	}

	template<typename T, std::enable_if_t<!std::is_base_of<Object, T>::value, int> = 0>
	static void operator <<(YAMLOutputArchive& archive, std::shared_ptr<T> const & t) = delete;

	/************************************************************************/
	/* std::weak_ptr                                                        */
	/************************************************************************/
	template<typename T>
	inline YAMLOutputArchive & operator << (YAMLOutputArchive& archive, std::weak_ptr<T> const & v)
	{
		auto t = v.lock();
		archive.SerializeObject(t);
		return archive;
	}

	/************************************************************************/
	/* UUID                                                                 */
	/************************************************************************/
	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, boost::uuids::uuid const & t)
	{
		//archive.SetManipulator(YAML::Flow);
		//archive.SetManipulator(YAML::BeginMap);
		//archive << make_nvp("fileID", boost::uuids::to_string(t));
		//archive.SetManipulator(YAML::EndMap);
		archive << boost::uuids::to_string(t);
		return archive;
	}

	inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, boost::filesystem::path const & t)
	{
//#if USE_yaml_cpp
//		archive << "path(todo)";
//#else
		archive << t.string();
//#endif
		return archive;
	}

	template <class T>
	inline void Prologue(YAMLOutputArchive & archive, T const & t)
	{
		//archive.m_emitter << YAML::Block;
		archive.SetManipulator(YAML::BeginMap);
	}

	template <class T>
	inline void Epilogue(YAMLOutputArchive& archive, T const & t)
	{
		archive.SetManipulator(YAML::EndMap);
	}

	//template <class T, std::enable_if_t<std::is_base_of<Object, T>::value, int> = 0>
	//inline YAMLOutputArchive & operator << (YAMLOutputArchive & archive, T const & t)
	//{
	//	t.Serialize(archive);
	//	return archive;
	//}


	inline void Prologue(YAMLOutputArchive& archive, Vector3 const & t)
	{
		archive.SetManipulator(::YAML::Flow);
		archive.SetManipulator(::YAML::BeginMap);
	}

	inline void Prologue(YAMLOutputArchive& archive, Vector4 const & t)
	{
		archive.SetManipulator(::YAML::Flow);
		archive.SetManipulator(::YAML::BeginMap);
	}

	inline void Prologue(YAMLOutputArchive& archive, Quaternion const & t)
	{
		archive.SetManipulator(::YAML::Flow);
		archive.SetManipulator(::YAML::BeginMap);
	}
}
