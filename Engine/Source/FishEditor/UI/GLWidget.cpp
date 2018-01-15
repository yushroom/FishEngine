#include "GLWidget.hpp"

//#include <FishEngine/Pipeline.hpp>
//#include <FishEngine/Material.hpp>
//#include <FishEngine/Mesh.hpp>
//#include <FishEngine/Graphics.hpp>
#include <FishEngine/Debug.hpp>
#include <FishEngine/RenderSystem.hpp>
#include <FishEngine/Screen.hpp>
#include <FishEngine/RenderBuffer.hpp>

#include "../MainEditor.hpp"
#include "../SceneViewEditor.hpp"

#include <FishGUI/FishGUI.hpp>

using namespace FishEditor;

GLWidget::GLWidget(const char* name) : FishGUI::IMWidget(name)
{
	//		FishEngine::RenderSystem::InitializeGL();
	FishEngine::Screen::set(474, 399);
	FishEditor::MainEditor::Init();
}

void GLWidget::DrawImpl()
{
	auto frameBuffer = FishEditor::MainEditor::m_mainSceneViewEditor->m_colorBuffer->GetNativeTexturePtr();
	auto r = m_rect;
	r.height = r.height;	// flip vertically
	FishGUI::Image(frameBuffer, r, true);
//	glCheckError();
}

void GLWidget::DrawScene()
{
	static FishGUI::Rect r{0, 0, 0, 0};
	if (m_rect.width != r.width || m_rect.height != r.height)
	{
		r = m_rect;
		Resize(r.width, r.height);
	}
	FishEngine::RenderSystem::ResetGLState();
	FishEditor::MainEditor::Run();
//	glCheckError();
}

void GLWidget::Resize(int width, int height)
{
	LogInfo(FishEngine::Format( "resize w=%1% h =%2%", width, height ));
	float ratio = 2.0;
//		int ratio = QApplication::desktop()->devicePixelRatio();
	LogInfo(FishEngine::Format("ratio = %1%", ratio ));
	FishEngine::Screen::setPixelsPerPoint(ratio);
	FishEditor::MainEditor::Resize(width, height);
	//m_mainSceneViewEditor->Resize(width*ratio, height*ratio);
}
