#include "Selection.hpp"

#include <Transform.hpp>

namespace FishEditor
{
    bool Selection::m_isActiveGameObjectLocked = false;

    std::list<std::weak_ptr<Transform>> Selection::m_transforms;

    std::weak_ptr<Object>   Selection::m_objectInInspector;

    std::weak_ptr<GameObject> Selection::m_selectedGameObjectInHierarchy;

    std::weak_ptr<Object>   Selection::m_activeAsset;

    std::weak_ptr<GameObject>   Selection::m_activeGameObject;

    std::weak_ptr<Transform>    Selection::m_activeTransform;

    Action Selection::selectionChanged;
    Action Selection::activeObjectChanged;



//    FishEngine::TransformPtr Selection::activeTransform()
//    {
//        if (m_transforms.empty())
//        {
//            return nullptr;
//        }
//        return m_transforms.front().lock();
//    }

    void Selection::setActiveGameObject(GameObjectPtr gameObject)
    {
        m_activeGameObject = gameObject;
        m_selectedGameObjectInHierarchy = gameObject;

//        for(auto const& a : selectionChanged)
//        {
//            a();
//        }
        selectionChanged();
    }

    void Selection::setActiveTransform(TransformPtr transform)
    {
        //m_activeTransform = transform;
        m_transforms.clear();
        if (transform != nullptr)
            m_transforms.push_back(transform);
    }

    FishEngine::GameObjectPtr Selection::activeGameObject()
    {
        auto const & t = activeTransform();
        if (nullptr != t)
            return t->gameObject();
        return nullptr;
    }

    void Selection::setTransforms(const std::list<std::weak_ptr<FishEngine::Transform> > &transforms)
    {
        m_transforms = transforms;
        if (!m_isActiveGameObjectLocked)
        {
            if (m_transforms.empty())
            {
                if (m_activeTransform.lock() != nullptr)
                {
                    m_activeTransform.reset();
                    activeObjectChanged();
                }
            }
            else
            {
                auto const & t = m_transforms.front();
                if (m_activeTransform.lock() != t.lock())
                {
                    m_activeTransform = t;
                    activeObjectChanged();
                }
            }
        }
        selectionChanged();
    }

    GameObjectPtr Selection::selectedGameObjectInHierarchy()
    {
        return activeGameObject();
    }

    //    void Selection::setSelectedGameObjectInHierarchy(GameObjectPtr gameObject)
    //    {
    //        m_selectedGameObjectInHierarchy = gameObject;
    //        if (!m_isActiveGameObjectLocked)
    //            m_activeGameObject = gameObject;
//    }

//    ObjectPtr Selection::activeAsset()
//    {
//        return m_activeAsset.lock();
//    }

//    void Selection::setActiveAsset(std::weak_ptr<Object> assetObject)
//    {
//        m_activeAsset = assetObject;
//        if (!m_isActiveGameObjectLocked)
//            m_objectInInspector = assetObject;
//    }

    bool Selection::Contains(FishEngine::TransformPtr transform)
    {
        for (auto const & item : m_transforms)
        {
            if (item.lock() == transform)
                return true;
        }
        return false;
    }

}
