#include "SerializedObject.hpp"
#include <Archive.hpp>

namespace FishEditor
{
	class SerializedObjectArchive : public FishEngine::OutputArchive
	{
	public:
		
		SerializedObjectArchive(std::vector<std::pair<std::string, SerializedPropertyPtr>> & properties) : m_properties(properties)
		{
			
		}
		
		
	protected:
		
		virtual void SerializeObject(FishEngine::ObjectPtr const & obj) override
		{
		}
		
		virtual void SerializeWeakObject(std::weak_ptr<FishEngine::Object> const & obj) override
		{
		}

		virtual void Serialize(short t) override
		{
			
		}
		
		virtual void Serialize(unsigned short t) override
		{
			
		}
		
		virtual void Serialize(int t) override
		{
			
		}
		
		virtual void Serialize(unsigned int t) override
		{
			
		}
		
		virtual void Serialize(long t) override
		{
			
		}
		
		virtual void Serialize(unsigned long t) override
		{
			
		}
		
		virtual void Serialize(long long t) override
		{
			
		}
		
		virtual void Serialize(unsigned long long t) override
		{
			
		}
		
		virtual void Serialize(float t) override
		{
			
		}
		
		virtual void Serialize(double t) override
		{
			
		}
		
		virtual void Serialize(bool t) override
		{
			
		}
		
		virtual void Serialize(std::string const & t) override
		{
			
		}
		
		virtual void Serialize(const char* t) override
		{
			
		}
		
		virtual void Serialize(std::nullptr_t const & t) override
		{
			
		}
		
		virtual void EndNVP() override
		{
			m_properties.push_back({currentKey, nullptr});
		}
		
		virtual void SerializeNameOfNVP(const char* name) override
		{
			currentKey = name;
		}
		
		virtual void MiddleOfNVP() override
		{
			
		}
		
	private:
		std::string currentKey;
		std::vector<std::pair<std::string, SerializedPropertyPtr>> & m_properties;
	};
	
	SerializedObject::SerializedObject(FishEngine::ObjectPtr obj)
		: m_targetObject(obj)
	{
		SerializedObjectArchive archive(m_properties);
	}
}
