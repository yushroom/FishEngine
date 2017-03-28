#pragma once

#include <Script.hpp>
#include <Transform.hpp>

// class Rotator;

// template<>
// constexpr int FishEngine::ClassID<Rotator>() { return ClassID<FishEngine::Script>(); }

#define DefineScript(T) \
	static constexpr const char * StaticClassName() { return #T; }  \
	virtual const std::string ClassName() const override { return StaticClassName(); }

class Rotator : public FishEngine::Script
{
public:
	//DefineComponent(Rotator);
	DefineScript(Rotator);

	virtual void Update() override
	{
		auto eulerAngles = transform()->localEulerAngles();
		eulerAngles.y += 2.0f;
		transform()->setLocalEulerAngles(eulerAngles);
	}
};
