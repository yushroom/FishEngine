#include <Script.hpp>
#include <GameObject.hpp>
#include <CapsuleCollider.hpp>
#include <Rigidbody.hpp>
#include <Debug.hpp>
#include <Input.hpp>

using namespace FishEngine;

class UnityChanControlScriptWithRgidBody : public FishEngine::Script
{
public:
	float animSpeed = 1.5f;
	float lookSmoother = 3.0f;
	bool useCurves = true;
	float useCurvesHeight = 0.5f;

	float forwardSpeed = 7.0f;
	float backwardSpeed = 2.0f;
	float rotateSpeed = 2.0f;
	float jumpPower = 3.0f;

private:
	CapsuleColliderPtr col;
	RigidbodyPtr rb;
	Vector3 velocity;
	float orgColHight;
	Vector3 orgVectColCenter;

	//Animator anim;
	//AnimatorStateInfo currentBaseState;
	GameObjectPtr cameraObject;

public:
	virtual void Start() override
	{
		//anim = GetComponent<Animator>();
		col = GetComponent<CapsuleCollider>();
		rb = GetComponent<Rigidbody>();
		//cameraObject = GameObject.FindWithTag("MainCamera");

		//orgColHight = col->height();
		//orgVectColCenter = col->center();
	}

	virtual void Update() override
	{
		auto pos = transform()->position();
		if (Input::GetKeyDown(KeyCode::W))
		{
			LogWarning("UnityChanControlScriptWithRgidBody");
			pos.z += 0.1f;
			transform()->setPosition(pos);
		}
		if (Input::GetKeyDown(KeyCode::S))
		{
			LogWarning("UnityChanControlScriptWithRgidBody");
			pos.z -= 0.1f;
			transform()->setPosition(pos);
		}
		if (Input::GetKeyDown(KeyCode::A))
		{
			LogWarning("UnityChanControlScriptWithRgidBody");
			pos.x -= 0.1f;
			transform()->setPosition(pos);
		}
		if (Input::GetKeyDown(KeyCode::D))
		{
			LogWarning("UnityChanControlScriptWithRgidBody");
			pos.x += 0.1f;
			transform()->setPosition(pos);
		}
	}
};