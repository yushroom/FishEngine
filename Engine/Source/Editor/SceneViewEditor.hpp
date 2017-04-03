#pragma once

#include "FishEditor.hpp"
#include <Vector3.hpp>
#include <Vector2.hpp>
#include <IntVector.hpp>
#include <Input.hpp>
#include <ReflectClass.hpp>

namespace FishEditor
{
	enum class TransformToolType
	{
		None,
		Translate,
		Rotate,
		Scale,
	};

	enum class TransformPivot
	{
		Pivot,
		Center,
	};

	enum class TransformSpace
	{
		Global,
		Local,
	};
	
	enum class ShadingMode
	{
		Shaded,
		Wireframe,
		ShadedWireframe,
	};

	class Meta(NonSerializable) SceneViewEditor
	{
	public:
		//FishEngine::Vector2             m_position;
		FishEngine::Int2                m_size{128, 128};

		FishEngine::RenderTargetPtr     m_sceneViewRenderTarget;
		FishEngine::ColorBufferPtr      m_colorBuffer;
		FishEngine::DepthBufferPtr      m_depthBuffer;
		//FishEngine::RenderTexturePtr    m_sceneViewRenderTexture;

		FishEngine::RenderTargetPtr     m_selectionOutlineRT;
		FishEngine::DepthBufferPtr      m_selectionOutlineDepthBuffer;
		FishEngine::RenderTargetPtr     m_selectionOutlineRT2;
		FishEngine::ColorBufferPtr      m_selectionOutlineColorBuffer2;

		void Init();

		void Update();

		void Render();

		void Clean();

		void Resize(int width, int height);

		FishEngine::CameraPtr camera()
		{
			return m_camera;
		}
		
		TransformPivot transformPivot() const
		{
			return m_transformPivot;
		}

		void setTransformPivot(TransformPivot pivot)
		{
			m_transformPivot = pivot;
		}

		TransformSpace transformSpace() const
		{
			return m_transformSpace;
		}

		void setTransformSpace(TransformSpace space)
		{
			m_transformSpace = space;
		}

		TransformToolType transformToolType()
		{
			return m_transformToolType;
		}

		void setTransformToolType(TransformToolType type)
		{
			m_transformToolType = type;
		}

		// this scene view is focused, so it may receive keyboard event.
		bool focused() const
		{
			return m_focused;
		}

		// mouse position is inside of this scene view, so it may receive mouse event.
		bool isMouseHovered() const
		{
			return m_isMouseHovered;
		}
		
		bool highlightSelections() const
		{
			return m_highlightSelections;
		}
		
		void setHighlightSelections(bool value)
		{
			m_highlightSelections = value;
		}

		void FrameSelected();

	private:
		friend class EditorGUI;

		FishEngine::CameraPtr m_camera;
		FishEngine::GameObjectPtr m_cameraGameObject;

		bool        m_isWireFrameMode;
		bool        m_useGammaCorrection;
		bool        m_showShadowMap;
		bool        m_highlightSelections   = true;

		bool        m_mouseEventHandled     = false;
		
		ShadingMode m_shadingMode           = ShadingMode::Shaded;
		bool        m_showGizmos            = true;
		

		// this scene view is focused, so it may receive keyboard event.
		bool        m_focused               = false;

		// mouse position is inside of this scene view, so it may receive mouse event.
		bool        m_isMouseHovered        = false;

		TransformToolType m_transformToolType = TransformToolType::Translate;
		TransformPivot m_transformPivot		= TransformPivot::Center;
		TransformSpace m_transformSpace     = TransformSpace::Global;

		int         m_selectedAxis          = -1; // temp
		std::weak_ptr<FishEngine::GameObject> m_lastSelectedGameObject; // temp
		bool m_enableGizmoOperation = true;
		void DrawTranslateGizmo();
		void DrawRotateGizmo();
		void DrawScaleGizmo();
		void DrawSceneGizmo();
	};

	typedef std::shared_ptr<SceneViewEditor> SceneViewEditorPtr;
}
