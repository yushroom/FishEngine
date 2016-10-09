#include "SceneView.hpp"
#include <Screen.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>
#include <CameraController.hpp>

using namespace FishEngine;

std::shared_ptr<Camera> FishEditor::SceneView::m_camera = nullptr;
std::shared_ptr<GameObject> FishEditor::SceneView::m_cameraGameObject;

void FishEditor::SceneView::Init()
{
    m_camera = Camera::Create(60.0, Screen::aspect(), 0.1f, 1000.f, CameraType::SceneView);
    m_cameraGameObject = std::make_shared<GameObject>("EditorCamera");
    m_cameraGameObject->transform()->m_gameObject = m_cameraGameObject;
    //auto camera_go = Scene::CreateGameObject("MainCamera");
    m_cameraGameObject->AddComponent<CameraController>();
    m_cameraGameObject->AddComponent(m_camera);
    m_cameraGameObject->transform()->setLocalPosition(5, 5, 5);
    m_cameraGameObject->transform()->LookAt(0, 0, 0);
    //m_cameraGameObject->SetTag("MainCamera");
}

void FishEditor::SceneView::Update()
{
    m_cameraGameObject->GetScript<CameraController>()->Update();
}
