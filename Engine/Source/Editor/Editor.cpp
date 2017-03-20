#include "Editor.hpp"
#include "SerializedObject.hpp"
#include "SerializedProperty.hpp"
#include "EditorGUI.hpp"

using namespace FishEditor;
using namespace FishEngine;

bool FishEditor::Editor::m_AllowMultiObjectAccess = true;

FishEditor::SerializedObjectPtr FishEditor::Editor::GetSerializedObjectInternal()
{
	if (this->m_SerializedObject == nullptr)
	{
		//this->m_SerializedObject = new SerializedObject(this->targets);
		this->m_SerializedObject = std::make_shared<SerializedObject>(this->m_Target);
	}
	return this->m_SerializedObject;
}


bool FishEditor::Editor::DoDrawDefaultInspector(SerializedObjectPtr obj)
{
	//EditorGUI::BeginChangeCheck();
	obj->Update();
	SerializedPropertyPtr iterator = obj->GetIterator();
	bool enterChildren = true;
	while (iterator->NextVisible(enterChildren))
	{
//		using (new EditorGUI::DisabledScope("m_Script" == iterator.propertyPath))
//		{
//			EditorGUILayout::PropertyField(iterator, true, new GUILayoutOption[0]);
//		}
		enterChildren = false;
	}
	obj->ApplyModifiedProperties();
	//return EditorGUI::EndChangeCheck();
	return false;
}
