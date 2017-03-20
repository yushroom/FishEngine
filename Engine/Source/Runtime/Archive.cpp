#include "Archive.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"
#include "Matrix4x4.hpp"

//#include "generate/Class_Serialization.hpp"

void FishEngine::OutputArchive::Serialize(Vector2 const & value)
{
	this->BeginClass();
	(*this) << FishEngine::make_nvp("x", value.x); // float
	(*this) << FishEngine::make_nvp("y", value.y); // float
	this->EndClass();
}

void FishEngine::OutputArchive::Serialize(Vector3 const & t)
{
	//(*this) << t;
	this->BeginClass();
	(*this) << FishEngine::make_nvp("x", t.x); // float
	(*this) << FishEngine::make_nvp("y", t.y); // float
	(*this) << FishEngine::make_nvp("z", t.z); // float
	this->EndClass();
}


void FishEngine::OutputArchive::Serialize(Vector4 const & value)
{
	this->BeginClass();
	(*this) << FishEngine::make_nvp("x", value.x); // float
	(*this) << FishEngine::make_nvp("y", value.y); // float
	(*this) << FishEngine::make_nvp("z", value.z); // float
	(*this) << FishEngine::make_nvp("w", value.w); // float
	this->EndClass();
}


void FishEngine::OutputArchive::Serialize(Quaternion const & value)
{
	this->BeginClass();
	(*this) << FishEngine::make_nvp("x", value.x); // float
	(*this) << FishEngine::make_nvp("y", value.y); // float
	(*this) << FishEngine::make_nvp("z", value.z); // float
	(*this) << FishEngine::make_nvp("w", value.w); // float
	this->EndClass();
}


void FishEngine::OutputArchive::Serialize(Matrix4x4 const & value)
{
	this->BeginClass();
	(*this) << FishEngine::make_nvp("rows[0]", value.rows[0]); // FishEngine::Vector4
	(*this) << FishEngine::make_nvp("rows[1]", value.rows[1]); // FishEngine::Vector4
	(*this) << FishEngine::make_nvp("rows[2]", value.rows[2]); // FishEngine::Vector4
	(*this) << FishEngine::make_nvp("rows[3]", value.rows[3]); // FishEngine::Vector4
	this->EndClass();
}

