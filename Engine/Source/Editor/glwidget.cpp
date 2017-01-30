#include "glwidget.h"
#include <QMouseEvent>
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>

#include <Debug.hpp>
#include <Camera.hpp>
#include <GameObject.hpp>
#include <CameraController.hpp>
#include <ModelImporter.hpp>
#include <Scene.hpp>
#include <Screen.hpp>
#include <Graphics.hpp>
#include <Light.hpp>
#include <RenderSettings.hpp>
#include <QualitySettings.hpp>
#include <MeshRenderer.hpp>
#include <Material.hpp>

#include "SceneViewEditor.hpp"
#include "Selection.hpp"

#include <boost/filesystem.hpp>

#include <iostream>

using namespace FishEngine;
using namespace FishEditor;
using namespace std;

Q_DECLARE_METATYPE(std::weak_ptr<FishEngine::Transform>);


GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    FishEngine::Debug::Log("GLWidget::ctor");
    //Init();
    m_mainSceneViewEditor = std::make_shared<SceneViewEditor>();
}


void DefaultScene()
{
#if 1
    cout << "CWD: " << boost::filesystem::initial_path() << endl;

    auto camera = Camera::Create();
    auto camera_go = Scene::CreateGameObject("Main Camera");
    camera_go->AddComponent(camera);
    camera_go->AddComponent<CameraController>();
    camera_go->transform()->setLocalPosition(0, 0, 5);
    camera_go->transform()->setLocalPosition(0, 1, -10);
    //camera_go->transform()->LookAt(0, 0, 0);
    camera_go->setTag("MainCamera");
    //camera_go->AddComponent<TakeScreenShot>();

    auto light_go = Scene::CreateGameObject("Directional Light");
    light_go->transform()->setPosition(0, 3, 0);
    light_go->transform()->setLocalEulerAngles(50, -30, 0);
    light_go->AddComponent(Light::Create());

    auto material = Material::InstantiateBuiltinMaterial("SkyboxProcedural");
    material->SetFloat("_AtmosphereThickness", 1.0);
    //material->SetFloat("_SunDisk", 2);
    material->SetFloat("_SunSize", 0.04f);
    material->SetVector4("_SkyTint", Vector4(0.5f, 0.5f, 0.5f, 1));
    material->SetVector4("_GroundColor", Vector4(.369f, .349f, .341f, 1));
    material->SetFloat("_Exposure", 1.3f);
    RenderSettings::setSkybox(material);

    auto cube = Scene::CreateGameObject("Cube");
    cube->transform()->SetParent(camera->transform());

#if 1

    //auto go = GameObject::CreatePrimitive(PrimitiveType::Cube);
    QualitySettings::setShadowDistance(20);
    {
#if FISHENGINE_PLATFORM_WINDOWS
    const std::string root_dir = R"(D:\program\FishEngine\Example\CascadedShadowMapping\)";
#else
    const std::string root_dir = "/Users/yushroom/program/graphics/FishEngine/Example/CascadedShadowMapping/";
#endif
    ModelImporter importer;
    auto model = importer.LoadFromFile(root_dir + "Terrain.obj");
    auto terrainGO = model->CreateGameObject();
    auto material = Material::InstantiateBuiltinMaterial("DebugCSM");
    //auto material = Material::defaultMaterial();
    //material->EnableKeyword(ShaderKeyword::Shadow);

    //TextureImporter texture_importer;
    //auto bakedAO = texture_importer.FromFile(root_dir + "bakedAO.jpg");
    //material->setMainTexture(bakedAO);
    terrainGO->GetComponent<MeshRenderer>()->SetMaterial(material);
    }
#endif

#endif
}

void GLWidget::Init()
{
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    //Init();
    Screen::set(width(), height());

    RenderSystem::Init();

    m_mainSceneViewEditor->Init();
    DefaultScene();

//    for (auto &go : Scene::GameObjects())
//    {
//        //QStandardItem* item = new QStandardItem(QString(go->name().c_str()));
//        auto item = CreateHierarchyItem(go->transform());
//        m_hierarchyModel->appendRow( item );
//        //item->appendRow(new QStandardItem("1"));
//    }
    //UpdateHierarchyModel();
    //QStandardItem* item = new QStandardItem("GameObject");
    //m_hierarchyModel->appendRow(item);
    //item->appendRow(new QStandardItem("1"));

    glClearColor(1.0f, 0.0f, 0.0f, 1);

    auto timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000 / 30.0f); // 30 fps
}

void GLWidget::paintGL()
{
    //Input::Update();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_mainSceneViewEditor->Render();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);   // Qt's verion
    //Graphics::Blit()
    glViewport(0, 0, Screen::width(), Screen::height());
    auto quad = Model::builtinMesh(PrimitiveType::Quad);
    auto mtl = Material::builtinMaterial("DrawQuad");
    mtl->setMainTexture(m_mainSceneViewEditor->m_colorBuffer);
    Graphics::DrawMesh(quad, mtl);
    //Debug::Log("paintGL");

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
    m_mainSceneViewEditor->Resize(width*ratio, height*ratio);
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
    auto delta = 1.0f * event->angleDelta().y() / QWheelEvent::DefaultDeltasPerStep;
    //Debug::LogWarning("mouse scroll: %lf", delta);
    Input::UpdateAxis(Axis::MouseScrollWheel, delta);
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{

}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{

}
