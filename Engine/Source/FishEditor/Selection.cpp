#include "Selection.hpp"

#include <FishEngine/Transform.hpp>

namespace FishEditor
{
	bool Selection::m_isActiveGameObjectLocked = false;

	std::weak_ptr<Object> Selection::s_activeObject;

	std::list<std::weak_ptr<Transform>> Selection::s_transforms;

	std::weak_ptr<GameObject>   Selection::s_activeGameObject;

	std::weak_ptr<Transform>    Selection::s_activeTransform;

	Action Selection::selectionChanged;
	Action Selection::activeObjectChanged;

	void Selection::setActiveGameObject(GameObjectPtr gameObject)
	{
		s_activeGameObject = gameObject;
		selectionChanged();
	}

	void Selection::setActiveTransform(TransformPtr transform)
	{
		//m_activeTransform = transform;
		s_transforms.clear();
		if (transform != nullptr)
			s_transforms.push_back(transform);
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
		s_transforms = transforms;
		if (!m_isActiveGameObjectLocked)
		{
			if (s_transforms.empty())
			{
				if (s_activeTransform.lock() != nullptr)
				{
					s_activeTransform.reset();
					activeObjectChanged();
				}
			}
			else
			{
				auto const & t = s_transforms.front();
				if (s_activeTransform.lock() != t.lock())
				{
					s_activeTransform = t;
					activeObjectChanged();
				}
			}
		}
		selectionChanged();
	}

	bool Selection::Contains(FishEngine::TransformPtr transform)
	{
		for (auto const & item : s_transforms)
		{
			if (item.lock() == transform)
				return true;
		}
		return false;
	}

}
