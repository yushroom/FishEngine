#include <GLEnvironment.hpp>
#include "glwidget.h"
#include <QMouseEvent>
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>

#include <Debug.hpp>
#include <Screen.hpp>
#include <Input.hpp>

#include "MainEditor.hpp"

using namespace FishEngine;
using namespace FishEditor;
//using namespace std;

Q_DECLARE_METATYPE(std::weak_ptr<FishEngine::Transform>);


GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    FishEngine::Debug::Log("GLWidget::ctor");
    //Init();

}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

#if FISHENGINE_PLATFORM_WINDOWS
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	auto err = glewInit();
	if (err != GLEW_OK)
	{
	    Debug::LogError("%s", glewGetErrorString(err));
	}
	else
	{
	    Debug::Log("GlEW initialized");
	}
#endif

    Screen::set(width(), height());
    MainEditor::Init();

    auto timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000 / 30.0f); // 30 fps
}

void GLWidget::paintGL()
{
    MainEditor::Run();
	
	Input::Update();

    auto globalCursorPos = QCursor::pos();
    auto localCursorPos = mapFromGlobal(globalCursorPos);
    // top-left -> bottom-left
    float x = localCursorPos.x() * Screen::pixelsPerPoint() / static_cast<float>(Screen::width());
    float y =  1.0f - localCursorPos.y() * Screen::pixelsPerPoint() / static_cast<float>(Screen::height());
    //Debug::Log("x = %lf, y = %lf", x, y);
    Input::UpdateMousePosition(x, y);
}


void GLWidget::resizeGL(int width, int height)
{
    Debug::Log("resize w=%d h =%d", width, height);
    int ratio = QApplication::desktop()->devicePixelRatio();
    Debug::LogWarning("ratio: %d", ratio);
    Screen::setPixelsPerPoint(ratio);
    MainEditor::Resize(width, height);
    //m_mainSceneViewEditor->Resize(width*ratio, height*ratio);
}

//void GLWidget::mouseMoveEvent(QMouseEvent *event)
//{
//    auto p = event->localPos();
//    float x = p.x() / Screen::width();
//    float y = -p.y() / Screen::height();
//    //Debug::Log("x : %f, y: %f", x, y);
//    Input::UpdateMousePosition(x, y);
//}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    int button = 0;
    auto b = event->button();
    if (b == Qt::LeftButton)
        button = 0;
    else if (b == Qt::RightButton)
        button = 1;
    else if (b == Qt::MiddleButton)
        button = 2;
    else
    {
        Debug::LogWarning("GLWidget::mousePressEvent: unknown mousebutton");
        return;
    }
    //Debug::LogWarning("mouse down: %d", button);
    Input::UpdateMouseButtonState(button, MouseButtonState::Down);
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    int button = 0;
    auto b = event->button();
    if (b == Qt::LeftButton)
        button = 0;
    else if (b == Qt::RightButton)
        button = 1;
    else if (b == Qt::MiddleButton)
        button = 2;
    else
    {
        Debug::LogWarning("GLWidget::mousePressEvent: unknown mousebutton");
        return;
    }
    //Debug::LogWarning("mouse up: %d", button);
    Input::UpdateMouseButtonState(button, MouseButtonState::Up);
}


void GLWidget::wheelEvent(QWheelEvent *event)
{
    auto delta = 5.0f * event->angleDelta().y() / QWheelEvent::DefaultDeltasPerStep;
    //Debug::LogWarning("mouse scroll: %lf", delta);
    Input::UpdateAxis(Axis::MouseScrollWheel, delta);
}

void GLWidget::keyPressEvent(QKeyEvent *)
{

}

void GLWidget::keyReleaseEvent(QKeyEvent *)
{

}
