#pragma once

#include <FishEngine/ScriptableObject.hpp>
#include <FishEngine/Mathf.hpp>
#include <FishEngine/Debug.hpp>
#include "IPreviewable.hpp"
#include "InspectorMode.hpp"


namespace FishEditor
{
	// Base class to derive custom Editors from. Use this to create your own custom inspectors and editors for your objects.
	class Meta(NonSerializable) Editor : public FishEngine::ScriptableObject, public IPreviewable
	{
	public:
		InjectClassName(Editor);
		
		SerializedObjectPtr m_SerializedObject = nullptr;
		InspectorMode m_InspectorMode = InspectorMode::Normal;
		static constexpr float kLineHeight = 16.f;
		bool hideInspector = false;
		static bool m_AllowMultiObjectAccess;

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
		
	protected:
//		virtual void OnHeaderGUI()
//		{
//			Editor::DrawHeaderGUI(this, this->targetTitle);
//		}

	public:
		virtual std::string targetTitle() const
		{
			std::string result;
			// ...
			return result;
		}
			
		SerializedObjectPtr serializedObject()
		{
			if (Editor::m_AllowMultiObjectAccess)
			{
				LogError("The serializedObject should not be used inside OnSceneGUI or OnPreviewGUI. Use the target property directly instead.");
			}
			return this->GetSerializedObjectInternal();
		}
		
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
		
		//OptimizedGUIBlock m_OptimizedBlock;
		
		
		//static Editor.Styles s_Styles;
		static constexpr float kImageSectionWidth = 44.f;
		//bool canEditMultipleObjects;
		
		int referenceTargetIndex()
		{
			//return FishEngine::Mathf::Clamp<int>(this->m_ReferenceTargetIndex, 0, this->m_Targets.size() - 1);
			return 0;
		}
		
		virtual SerializedObjectPtr GetSerializedObjectInternal();
		
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
