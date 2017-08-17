#include "SerializedObject.hpp"

#include "SerializedProperty.hpp"

#include <FishEngine/Serialization/Archive.hpp>
#include <boost/any.hpp>

namespace FishEditor
{
	class SerializedObjectArchive : public FishEngine::OutputArchive
	{
	public:
		
		SerializedObjectArchive(std::vector<std::pair<std::string, SerializedPropertyPtr>> & properties) : m_properties(properties)
		{
			
		}
		
		void Process(FishEngine::ObjectPtr obj)
		{
			obj->Serialize(*this);
		}
		
		
	protected:

		virtual void Serialize(short t) override
		{
			m_propertyType = SerializedPropertyType::Integer;
			m_currentValue = static_cast<int>(t);
		}
		
		virtual void Serialize(unsigned short t) override
		{
			m_propertyType = SerializedPropertyType::Integer;
			m_currentValue = static_cast<int>(t);
		}
		
		virtual void Serialize(int t) override
		{
			m_propertyType = SerializedPropertyType::Integer;
			m_currentValue = static_cast<int>(t);
		}
		
		virtual void Serialize(unsigned int t) override
		{
			m_propertyType = SerializedPropertyType::Integer;
			m_currentValue = static_cast<int>(t);
		}
		
		virtual void Serialize(long t) override
		{
			m_propertyType = SerializedPropertyType::Integer;
			m_currentValue = static_cast<int>(t);
		}
		
		virtual void Serialize(unsigned long t) override
		{
			m_propertyType = SerializedPropertyType::Integer;
			m_currentValue = static_cast<int>(t);
		}
		
		virtual void Serialize(long long t) override
		{
			m_propertyType = SerializedPropertyType::Integer;
			m_currentValue = static_cast<int>(t);
			abort();
		}
		
		virtual void Serialize(unsigned long long t) override
		{
			m_propertyType = SerializedPropertyType::Integer;
			m_currentValue = static_cast<int>(t);
			abort();
		}
		
		virtual void Serialize(float t) override
		{
			m_propertyType = SerializedPropertyType::Float;
			m_currentValue = static_cast<double>(t);
		}
		
		virtual void Serialize(double t) override
		{
			m_propertyType = SerializedPropertyType::Float;
			m_currentValue = t;
		}
		
		virtual void Serialize(bool t) override
		{
			m_propertyType = SerializedPropertyType::Boolean;
			m_currentValue = t;
		}
		
		virtual void Serialize(std::string const & t) override
		{
			m_propertyType = SerializedPropertyType::String;
			m_currentValue = t;
		}
		
		virtual void Serialize(const char* t) override
		{
			Serialize(std::string(t));
		}
		
		virtual void Serialize(std::nullptr_t const & t) override
		{
			abort();
		}
		
		virtual void SerializeObject(FishEngine::ObjectPtr const & obj) override
		{
			m_propertyType = SerializedPropertyType::ObjectReference;
			m_currentValue = obj;
		}
		
		virtual void SerializeWeakObject(std::weak_ptr<FishEngine::Object> const & obj) override
		{
			abort();
		}
		
		virtual void EndNVP() override
		{
			auto property = std::make_shared<SerializedProperty>();
			property->m_propertyPath = m_currentKey;
			property->m_value = m_currentValue;
			property->m_propertyType = m_propertyType;
			m_properties.push_back({m_currentKey, property});
		}
		
		virtual void SerializeNameOfNVP(const char* name) override
		{
			m_currentKey = name;
		}
		
		virtual void MiddleOfNVP() override
		{
			
		}
		
	private:
		std::string m_currentKey;
		SerializedPropertyType m_propertyType;
		boost::any  m_currentValue;
		std::vector<std::pair<std::string, SerializedPropertyPtr>> & m_properties;
	};
	
	SerializedObject::SerializedObject(FishEngine::ObjectPtr obj)
		: m_targetObject(obj)
	{
		SerializedObjectArchive archive(m_properties);
		archive.Process( obj );
	}
	
	void SerializedObject::Dispose()
	{
		
	}
	
	SerializedPropertyPtr SerializedObject::GetIterator()
	{
		return nullptr;
	}
	
	SerializedPropertyPtr SerializedObject::FindProperty(std::string const & propertyPath)
	{
		for (auto & x : m_properties)
		{
			if (x.first == propertyPath)
			{
				return x.second;
			}
		}
		return nullptr;
	}
}
