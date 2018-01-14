#pragma once

#include "FishEditor.hpp"
#include "InspectorMode.hpp"
#include "PropertyModification.hpp"

namespace FishEditor
{
	class SerializedObject
	{
	public:
		SerializedObject(FishEngine::ObjectPtr obj);

//		SerializedObject(std::vector<FishEngine::ObjectPtr> const & obj)
//			: m_targetObjects(obj)
//		{
//			// TODO
//		}

		~SerializedObject()
		{
			this->Dispose();
		}


		FishEngine::ObjectPtr targetObject() const
		{
//			if (m_targetObjects.size() > 0)
//				return m_targetObjects.front();
//			return nullptr;
			return m_targetObject;
		}

//		std::vector<FishEngine::ObjectPtr> const & targetObjects() const
//		{
//			return m_targetObjects;
//		}
		
		bool isEditingMultipleObjects() const;
		int maxArraySizeForMultiEditing() const;
		void Update() { };
		void SetIsDifferentCacheDirty();
		void UpdateIfDirtyOrScript();
		void Dispose();
		
		SerializedPropertyPtr GetIterator();
		
		SerializedPropertyPtr FindProperty(std::string const & propertyPath);
		
		bool ApplyModifiedProperties() { return true; };
		bool ApplyModifiedPropertiesWithoutUndo();
		void CopyFromSerializedProperty(SerializedPropertyPtr prop);

	private:
		bool hasModifiedProperties() const;
		InspectorMode inspectorMode() const;
		
		void Cache(int instanceID);
		static SerializedObjectPtr LoadFromCache(int instanceID);
		PropertyModification ExtractPropertyModification(std::string const & propertyPath);
		
	private:
		FishEngine::ObjectPtr 				m_targetObject;
		//std::vector<FishEngine::ObjectPtr> m_targetObjects;
		std::vector<std::pair<std::string, SerializedPropertyPtr>> m_properties;
		//std::map<std::string, SerializedPropertyPtr> m_properties;
	};
}
