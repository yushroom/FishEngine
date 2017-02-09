#pragma once

#include <boost/lexical_cast.hpp>

namespace FishEngine
{
	class YAMLInputArchive
	{
	public:
		YAMLInputArchive(std::string const & filename)
		{
			m_nodes = YAML::LoadAllFromFile(filename);
		}
		
		YAMLInputArchive(YAMLInputArchive const &) = delete;
		
		YAMLInputArchive operator=(YAMLInputArchive const &) = delete;

		YAML::Node CurrentNode()
		{
			if (m_node == nullptr)
				return m_nodes[m_nodeIndex];
			return *m_node;
		}

		template<class T, std::enable_if_t<std::is_base_of<Object, T>::value, int> = 0>
		std::shared_ptr<T> DeserializeObject()
		{
			assert(CurrentNode().IsMap());
			auto node = CurrentNode()[T::StaticClassName()];
			m_node = &node;
			auto p = std::make_shared<T>();
			(*this) >> *p;
			m_node = nullptr;
			return p;
		}

		void ToNextNode()
		{
			m_nodeIndex++;
		}

	private:
		std::vector<YAML::Node> m_nodes;
		uint32_t m_nodeIndex = 0;
		YAML::Node * m_node = nullptr;
	};

	template<class T>
	inline YAMLInputArchive& operator >> (YAMLInputArchive& archive, T & t)
	{
		Load(archive, t);
		return archive;
	}

	template<class T, std::enable_if_t<std::is_arithmetic<T>::value, int> = 0>
	void convert(YAML::Node const & node, T & t)
	{
		t = node.as<T>();
	}

	inline void convert(YAML::Node const & node, std::string & t)
	{
		t = node.as<std::string>();
		//std::cout << "as string: " << t << std::endl;
	}

	inline void convert(YAML::Node const & node, boost::uuids::uuid & t)
	{
		assert(node.IsMap());
		t = boost::lexical_cast<boost::uuids::uuid>(node["fileID"].as<std::string>());
		//std::istringstream sin(node["fileID"].as<std::string>());
		//sin >> t;
	}

	template<class T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
	void convert(YAML::Node const & node, T & t)
	{
		//std::cout << node.Scalar() << std::endl;
		t = static_cast<T>(node.as<std::underlying_type_t<T>>());
	}

	template<class T>
	inline YAMLInputArchive& operator >> (YAMLInputArchive& archive, NameValuePair<T>  && nvp)
	{
		//std::cout << nvp.name << endl;
		//nvp.value = archive.CurrentNode()[nvp.name].as<T>();
		convert(archive.CurrentNode()[nvp.name], nvp.value);
		return archive;
	}

	template<class T>
	YAMLInputArchive& operator >> (YAMLInputArchive& archive, BaseClassWrapper<T> t)
	{
		archive >> const_cast<T&>(t.base_ref);
		return archive;
	}
}
