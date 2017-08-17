#pragma once

#include "Behaviour.hpp"
#include "Animation/WrapMode.hpp"

namespace FishEngine
{
	class FE_EXPORT Animation : public Behaviour
	{
	public:
		DefineComponent(Animation);

		Animation() = default;

		virtual void Start() override;
		virtual void Update() override;

		// the default animation
		//Meta(NonSerializable)
		AnimationClipPtr m_clip;

		bool m_isPlaying = false;

		bool m_playAutomatically = true;

		WrapMode m_wrapMode;

		// temp
		Meta(NonSerializable)
		float m_localTimer = 0.0f;

		Meta(NonSerializable)
		std::map<std::string, TransformPtr> m_skeleton;
	};
}
