#pragma once

#include "FishEditor.hpp"
#include <Rect.hpp>

namespace FishEditor
{
	class IPreviewable
	{
	public:
		FishEngine::ObjectPtr target()
		{
			return m_target;
		}

		void Initialize(std::vector<FishEngine::ObjectPtr> targets);

		bool MoveNextTarget();

		void ResetTarget();

		bool HasPreviewGUI();

		//GUIContent GetPreviewTitle();

		void DrawPreview(FishEngine::Rect const & previewArea);

		//void OnPreviewGUI(Rect r, GUIStyle background);

		//void OnInteractivePreviewGUI(Rect r, GUIStyle background);

		void OnPreviewSettings();

		std::string GetInfoString();

		void ReloadPreviewInstances();

	protected:
		FishEngine::ObjectPtr m_target;
	};
}
