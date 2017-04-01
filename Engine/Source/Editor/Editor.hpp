#pragma once

#include <ScriptableObject.hpp>
#include <Mathf.hpp>
#include <Debug.hpp>
#include "IPreviewable.hpp"
#include "InspectorMode.hpp"


namespace FishEditor
{
	class Meta(NonSerializable) Editor : public FishEngine::ScriptableObject, public IPreviewable
	{
	public:
		InjectClassName(Editor);
		
		// Draw the built-in inspector.
		bool DrawDefaultInspector()
		{
			return this->DoDrawDefaultInspector();
		}
		
		void DrawHeader()
		{
//			if (EditorGUIUtility::hierarchyMode)
//			{
//				this->DrawHeaderFromInsideHierarchy();
//			}
//			else
//			{
//				this->OnHeaderGUI();
//			}
		}
		
		// Implement this function to make a custom inspector.
		virtual void OnInspectorGUI()
		{
			this->DrawDefaultInspector();
		}
		
//	protected:
//		virtual void OnHeaderGUI()
//		{
//			Editor::DrawHeaderGUI(this, this->targetTitle);
//		}
		
	private:
		class Styles
		{
			
		};
		
		//std::vector<FishEngine::ObjectPtr> m_Targets;
		FishEngine::ObjectPtr m_Target;
		int m_IsDirty;
		int m_ReferenceTargetIndex = 0;
		// PropertyHandlerCache m_PropertyHandlerCache = new PropertyHandlerCache();
		IPreviewable m_DummyPreview;
		SerializedObjectPtr m_SerializedObject = nullptr;
		//OptimizedGUIBlock m_OptimizedBlock;
		InspectorMode m_InspectorMode = InspectorMode::Normal;
		static constexpr float kLineHeight = 16.f;
		bool hideInspector = false;
		static bool m_AllowMultiObjectAccess;
		//static Editor.Styles s_Styles;
		static constexpr float kImageSectionWidth = 44.f;
		//bool canEditMultipleObjects;
		
		int referenceTargetIndex()
		{
			//return FishEngine::Mathf::Clamp<int>(this->m_ReferenceTargetIndex, 0, this->m_Targets.size() - 1);
			return 0;
		}
		
		virtual SerializedObjectPtr GetSerializedObjectInternal();
		
		SerializedObjectPtr serializedObject()
		{
			if (Editor::m_AllowMultiObjectAccess)
			{
				LogError("The serializedObject should not be used inside OnSceneGUI or OnPreviewGUI. Use the target property directly instead.");
			}
			return this->GetSerializedObjectInternal();
		}
		
		
		// internal
		bool DoDrawDefaultInspector()
		{
			return Editor::DoDrawDefaultInspector(this->m_SerializedObject);
		}
		
		// internal
		static bool DoDrawDefaultInspector(SerializedObjectPtr obj);
		
		// internal
//		static FishEngine::Rect DrawHeaderGUI(Editor & editor, std::string const & header)
//		{
//			return Editor::DrawHeaderGUI(editor, header, 0.f);
//		}
	};
}
