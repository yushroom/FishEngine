#pragma once

#include "Editor.hpp"

namespace FishEditor
{
	class AssetImporterInspector : public Editor
	{
	public:

		virtual EditorPtr assetEditor() const
		{
			return m_AssetEditor;
		}

		virtual void setAssetEditor(EditorPtr editor)
		{
			m_AssetEditor = editor;
		}

		virtual std::string targetTitle() const override
		{
			return FishEngine::Format("%1% Import Settings", m_AssetEditor == nullptr ? "" : m_AssetEditor->targetTitle());
		}

		//virtual int referenceTargetIndex() const override
		//virtual IPreviewable preview() const override

		virtual bool useAssetDrawPreview() const { return true; }
		virtual bool showImportedObject() const { return true; }

		//virtual void OnHeaderIconGUI(Rect const & iconRect) override

		virtual SerializedObjectPtr GetSerializedObjectInternal() override
		{
			if (this->m_SerializedObject)
		}

	private:
		unsigned long m_AssetTimeStamp = 0;
		bool m_MightHaveModified = false;
		EditorPtr m_AssetEditor;
	};
}