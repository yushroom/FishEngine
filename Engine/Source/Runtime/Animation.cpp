#include "Animation.hpp"
#include <deque>
#include <Transform.hpp>
#include <AnimationClip.hpp>
#include <Time.hpp>

using namespace FishEngine;

void GetSkeleton(const TransformPtr & t, std::string const & path, std::map<std::string, TransformPtr>& skeleton)
{
	auto current_path = path + "/" + t->name();

	// TODO: remove this hack
	if (path == "")	// root node
	{
		current_path = "unitychan_RUN00_F/group1";
	}
	skeleton[current_path] = t;
	for (auto & child : t->children())
	{
		GetSkeleton(child, current_path, skeleton);
	}
}

void Animation::Start()
{
	auto t = transform();
	GetSkeleton(t, "", m_skeleton);
}

TransformPtr GetBone(std::string const & path, std::map<std::string, TransformPtr> const & skeleton)
{
	auto it = skeleton.find(path);
	if (it == skeleton.end())
		abort();
	return it->second;
}

void Animation::Update()
{
	if (m_clip == nullptr)
		return;
	m_localTimer += Time::deltaTime();
	for (auto & curve : m_clip->m_positionCurve)
	{
		auto t = GetBone(curve.path, m_skeleton);
		auto v = curve.curve.Evaluate(m_localTimer, true);
		assert(!(isnan(v.x) || isnan(v.y) || isnan(v.z)));
		t->setLocalPosition(v);
	}
	//for (auto & curve : m_clip->m_rotationCurves)
	//{
	//	auto t = GetBone(curve.path, m_skeleton);
	//	auto v = curve.curve.Evaluate(m_localTimer, true);
	//	if (isnan(v.x) || isnan(v.y) || isnan(v.z) || isnan(v.w))
	//	{
	//		abort();
	//	}
	//	v.NormalizeSelf();
	//	t->setLocalRotation(v);
	//}
	for (auto & curve : m_clip->m_eulersCurves)
	{
		auto t = GetBone(curve.path, m_skeleton);
		auto v = curve.curve.Evaluate(m_localTimer, true);
		assert(!(isnan(v.x) || isnan(v.y) || isnan(v.z)));
		//t->setLocalEulerAngles(v);
		t->setLocalRotation(Quaternion::Euler(RotationOrder::ZXY, v));
	}
	for (auto & curve : m_clip->m_scaleCurves)
	{
		auto t = GetBone(curve.path, m_skeleton);
		auto v = curve.curve.Evaluate(m_localTimer, true);
		assert(!(isnan(v.x) || isnan(v.y) || isnan(v.z)));
		t->setLocalScale(v);
	}
}
