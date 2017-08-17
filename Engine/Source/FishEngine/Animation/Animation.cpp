#include <FishEngine/Animation.hpp>

#include <deque>
#include <boost/algorithm/string.hpp>

#include <FishEngine/Transform.hpp>
#include <FishEngine/AnimationClip.hpp>
#include <FishEngine/Time.hpp>
#include <FishEngine/Avatar.hpp>

using namespace FishEngine;

void GetSkeleton(
	const TransformPtr &					t,
	std::string const &						path,
	std::map<std::string, TransformPtr>&	skeleton,
	std::map<std::string, int> const &		boneToIndex)
{
	std::string current_path;
	if (path != "")
	{
		current_path = path + "/";
	}

	auto name = t->name();
	if (boneToIndex.find(name) != boneToIndex.end())
		current_path += name;

	if (current_path != "")
		skeleton[current_path] = t;
	for (auto & child : t->children())
	{
		GetSkeleton(child, current_path, skeleton, boneToIndex);
	}
}

void Animation::Start()
{
	if (m_clip == nullptr)
		return;
	auto t = transform();
	GetSkeleton(t, "", m_skeleton, m_clip->m_avatar->m_boneToIndex);
}

TransformPtr GetBone(std::string const & path, std::map<std::string, TransformPtr> const & skeleton)
{
	auto it = skeleton.find(path);
	if (it == skeleton.end())
	{
		//abort();
		LogWarning(Format("Bone [%1%] not found", path));
		return nullptr;
	}
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
		if (t != nullptr)
		{
			auto v = curve.curve.Evaluate(m_localTimer, true);
			t->setLocalPosition(v);
		}
	}
	for (auto & curve : m_clip->m_rotationCurves)
	{
		auto t = GetBone(curve.path, m_skeleton);
		if (t != nullptr)
		{
			auto v = curve.curve.Evaluate(m_localTimer, true);
			v.NormalizeSelf();
			t->setLocalRotation(v);
		}
	}
//	for (auto & curve : m_clip->m_eulersCurves)
//	{
//		auto t = GetBone(curve.path, m_skeleton);
//		auto v = curve.curve.Evaluate(m_localTimer, true);
//		assert(!(isnan(v.x) || isnan(v.y) || isnan(v.z)));
//		t->setLocalRotation(Quaternion::Euler(RotationOrder::XYZ, v));
//	}
	for (auto & curve : m_clip->m_scaleCurves)
	{
		auto t = GetBone(curve.path, m_skeleton);
		if (t != nullptr)
		{
			auto v = curve.curve.Evaluate(m_localTimer, true);
			t->setLocalScale(v);
		}
	}
}
