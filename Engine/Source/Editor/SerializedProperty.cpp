#include "SerializedProperty.hpp"
#include <cassert>


int FishEditor::SerializedProperty::intValue()
{
	assert(m_propertyType == SerializedPropertyType::Integer);
	return boost::any_cast<int>(m_value);
}

long FishEditor::SerializedProperty::longValue()
{
	assert(m_propertyType == SerializedPropertyType::Integer);
	return boost::any_cast<long>(m_value);
}

bool FishEditor::SerializedProperty::boolValue()
{
	assert(m_propertyType == SerializedPropertyType::Boolean);
	return boost::any_cast<bool>(m_value);
}

float FishEditor::SerializedProperty::floatValue()
{
	assert(m_propertyType == SerializedPropertyType::Float);
	return boost::any_cast<float>(m_value);
}

double FishEditor::SerializedProperty::doubleValue()
{
	assert(m_propertyType == SerializedPropertyType::Float);
	return boost::any_cast<double>(m_value);
}

std::string	FishEditor::SerializedProperty::stringValue()
{
	assert(m_propertyType == SerializedPropertyType::String);
	return boost::any_cast<std::string>(m_value);
}

void FishEditor::SerializedProperty::Dispose()
{
	
}
