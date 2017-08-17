#include <FishEngine/Matrix4x4.hpp>
#include <cassert>

namespace FishEngine
{

	Matrix4x4 Matrix4x4::Inverse(const Matrix4x4& m)
	{
		float p10_21 = m.m[1][0] * m.m[2][1] - m.m[2][0] * m.m[1][1];
		float p10_22 = m.m[1][0] * m.m[2][2] - m.m[2][0] * m.m[1][2];
		float p10_23 = m.m[1][0] * m.m[2][3] - m.m[2][0] * m.m[1][3];
		float p11_22 = m.m[1][1] * m.m[2][2] - m.m[2][1] * m.m[1][2];
		float p11_23 = m.m[1][1] * m.m[2][3] - m.m[2][1] * m.m[1][3];
		float p12_23 = m.m[1][2] * m.m[2][3] - m.m[2][2] * m.m[1][3];
		float p10_31 = m.m[1][0] * m.m[3][1] - m.m[3][0] * m.m[1][1];
		float p10_32 = m.m[1][0] * m.m[3][2] - m.m[3][0] * m.m[1][2];
		float p10_33 = m.m[1][0] * m.m[3][3] - m.m[3][0] * m.m[1][3];
		float p11_32 = m.m[1][1] * m.m[3][2] - m.m[3][1] * m.m[1][2];
		float p11_33 = m.m[1][1] * m.m[3][3] - m.m[3][1] * m.m[1][3];
		float p12_33 = m.m[1][2] * m.m[3][3] - m.m[3][2] * m.m[1][3];
		float p20_31 = m.m[2][0] * m.m[3][1] - m.m[3][0] * m.m[2][1];
		float p20_32 = m.m[2][0] * m.m[3][2] - m.m[3][0] * m.m[2][2];
		float p20_33 = m.m[2][0] * m.m[3][3] - m.m[3][0] * m.m[2][3];
		float p21_32 = m.m[2][1] * m.m[3][2] - m.m[3][1] * m.m[2][2];
		float p21_33 = m.m[2][1] * m.m[3][3] - m.m[3][1] * m.m[2][3];
		float p22_33 = m.m[2][2] * m.m[3][3] - m.m[3][2] * m.m[2][3];
		Matrix4x4 A;
		A.m[0][0] = +(m.m[1][1] * p22_33 - m.m[1][2] * p21_33 + m.m[1][3] * p21_32);
		A.m[1][0] = -(m.m[1][0] * p22_33 - m.m[1][2] * p20_33 + m.m[1][3] * p20_32);
		A.m[2][0] = +(m.m[1][0] * p21_33 - m.m[1][1] * p20_33 + m.m[1][3] * p20_31);
		A.m[3][0] = -(m.m[1][0] * p21_32 - m.m[1][1] * p20_32 + m.m[1][2] * p20_31);
		A.m[0][1] = -(m.m[0][1] * p22_33 - m.m[0][2] * p21_33 + m.m[0][3] * p21_32);
		A.m[1][1] = +(m.m[0][0] * p22_33 - m.m[0][2] * p20_33 + m.m[0][3] * p20_32);
		A.m[2][1] = -(m.m[0][0] * p21_33 - m.m[0][1] * p20_33 + m.m[0][3] * p20_31);
		A.m[3][1] = +(m.m[0][0] * p21_32 - m.m[0][1] * p20_32 + m.m[0][2] * p20_31);
		A.m[0][2] = +(m.m[0][1] * p12_33 - m.m[0][2] * p11_33 + m.m[0][3] * p11_32);
		A.m[1][2] = -(m.m[0][0] * p12_33 - m.m[0][2] * p10_33 + m.m[0][3] * p10_32);
		A.m[2][2] = +(m.m[0][0] * p11_33 - m.m[0][1] * p10_33 + m.m[0][3] * p10_31);
		A.m[3][2] = -(m.m[0][0] * p11_32 - m.m[0][1] * p10_32 + m.m[0][2] * p10_31);
		A.m[0][3] = -(m.m[0][1] * p12_23 - m.m[0][2] * p11_23 + m.m[0][3] * p11_22);
		A.m[1][3] = +(m.m[0][0] * p12_23 - m.m[0][2] * p10_23 + m.m[0][3] * p10_22);
		A.m[2][3] = -(m.m[0][0] * p11_23 - m.m[0][1] * p10_23 + m.m[0][3] * p10_21);
		A.m[3][3] = +(m.m[0][0] * p11_22 - m.m[0][1] * p10_22 + m.m[0][2] * p10_21);

		float det = 0;
		det += m.m[0][0] * A.m[0][0];
		det += m.m[0][1] * A.m[1][0];
		det += m.m[0][2] * A.m[2][0];
		det += m.m[0][3] * A.m[3][0];
		float inv_det = 1.f / det;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				A.m[i][j] *= inv_det;
			}
		}

		//auto I = A*m;
		//if (!I.isIdentity()) {
		//    abort();
		//}
		return A;
	}

	bool Zero(float f) {
		return (f < 1e-4f) && (f > -1e-4f);
	}
	
	bool One(float f) {
		return (f-1 < 1e-4f) && (f-1 > -1e-4f);
	}
	

	bool Matrix4x4::isIdentity() const
	{
		return
		One(m[0][0]) && Zero(m[0][1]) && Zero(m[0][2]) && Zero(m[0][3]) &&
		Zero(m[1][0]) && One(m[1][1]) && Zero(m[1][2]) && Zero(m[1][3]) &&
		Zero(m[2][0]) && Zero(m[2][1]) && One(m[2][2]) && Zero(m[2][3]) &&
		Zero(m[3][0]) && Zero(m[3][1]) && Zero(m[3][2]) && One(m[3][3]);
	}


	float Matrix4x4::Determinant(const Matrix4x4& m)
	{
		float SubFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
		float SubFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
		float SubFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
		float SubFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
		float SubFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
		float SubFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];

		Vector4 DetCof(
			+(m[1][1] * SubFactor00 - m[1][2] * SubFactor01 + m[1][3] * SubFactor02),
			-(m[1][0] * SubFactor00 - m[1][2] * SubFactor03 + m[1][3] * SubFactor04),
			+(m[1][0] * SubFactor01 - m[1][1] * SubFactor03 + m[1][3] * SubFactor05),
			-(m[1][0] * SubFactor02 - m[1][1] * SubFactor04 + m[1][2] * SubFactor05));

		return
			m[0][0] * DetCof[0] + m[0][1] * DetCof[1] +
			m[0][2] * DetCof[2] + m[0][3] * DetCof[3];
	}


	FishEngine::Matrix4x4 Matrix4x4::TRS(const Vector3& pos, const Quaternion& q, const Vector3& s)
	{
		Matrix4x4 mat = Matrix4x4::FromRotation(q);

		mat.m[0][3] = pos.x;
		mat.m[1][3] = pos.y;
		mat.m[2][3] = pos.z;
		mat.m[0][0] *= s.x;
		mat.m[0][1] *= s.y;
		mat.m[0][2] *= s.z;
		mat.m[1][0] *= s.x;
		mat.m[1][1] *= s.y;
		mat.m[1][2] *= s.z;
		mat.m[2][0] *= s.x;
		mat.m[2][1] *= s.y;
		mat.m[2][2] *= s.z;
		mat.m[3][3] = 1.f;
		return mat;
	}


	void Matrix4x4::TRS(
		const Vector3&      translation,
		const Quaternion&   rotation,
		const Vector3&      scale,
		Matrix4x4&          outLocalToWorld, 
		Matrix4x4&          outWorldToLocal)
	{
		// outLocalToWorld = TRS
		// outWorldToLocal = inverse(outLocalToWorld) = (S^-1)(R')(T^-1)
		outLocalToWorld = Matrix4x4::FromRotation(rotation);
		
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				outWorldToLocal.m[i][j] = outLocalToWorld.m[j][i];
			}
		}

		outLocalToWorld.m[0][3] = translation.x;
		outLocalToWorld.m[1][3] = translation.y;
		outLocalToWorld.m[2][3] = translation.z;
		
		Vector4 col3(-translation.x, -translation.y, -translation.z, 0); // use 0 instead 1, since we have not yet cleared the last column of outWorldToLocal.
		float m03 = Vector4::Dot(outWorldToLocal.rows[0], col3);
		float m13 = Vector4::Dot(outWorldToLocal.rows[1], col3);
		float m23 = Vector4::Dot(outWorldToLocal.rows[2], col3);
		outWorldToLocal.m[0][3] = m03;
		outWorldToLocal.m[1][3] = m13;
		outWorldToLocal.m[2][3] = m23;

		outLocalToWorld.m[0][0] *= scale.x;
		outLocalToWorld.m[0][1] *= scale.y;
		outLocalToWorld.m[0][2] *= scale.z;
		outLocalToWorld.m[1][0] *= scale.x;
		outLocalToWorld.m[1][1] *= scale.y;
		outLocalToWorld.m[1][2] *= scale.z;
		outLocalToWorld.m[2][0] *= scale.x;
		outLocalToWorld.m[2][1] *= scale.y;
		outLocalToWorld.m[2][2] *= scale.z;

		float inv_sx = 1.0f / scale.x;
		float inv_sy = 1.0f / scale.y;
		float inv_sz = 1.0f / scale.z;
		outWorldToLocal.m[0][0] *= inv_sx;
		outWorldToLocal.m[0][1] *= inv_sx;
		outWorldToLocal.m[0][2] *= inv_sx;
		outWorldToLocal.m[1][0] *= inv_sy;
		outWorldToLocal.m[1][1] *= inv_sy;
		outWorldToLocal.m[1][2] *= inv_sy;
		outWorldToLocal.m[2][0] *= inv_sz;
		outWorldToLocal.m[2][1] *= inv_sz;
		outWorldToLocal.m[2][2] *= inv_sz;

		outLocalToWorld.m[3][3] = 1.f;
		outWorldToLocal.m[3][3] = 1.f;

		// TODO: remove later
		//auto test = outWorldToLocal * outLocalToWorld;
		//if (!test.isIdentity()) {
		//    abort();
		//}
	}


	void Matrix4x4::Decompose(
		const Matrix4x4&    transformation, 
		Vector3*            outTranslation,
		Quaternion*         outRotation, 
		Vector3*            outScale)
	{
		auto& m = transformation;
		outScale->x = sqrtf(m.m[0][0]*m.m[0][0] + m.m[1][0]*m.m[1][0] + m.m[2][0]*m.m[2][0]);
		outScale->y = sqrtf(m.m[0][1]*m.m[0][1] + m.m[1][1]*m.m[1][1] + m.m[2][1]*m.m[2][1]);
		outScale->z = sqrtf(m.m[0][2]*m.m[0][2] + m.m[1][2]*m.m[1][2] + m.m[2][2]*m.m[2][2]);
		
		outTranslation->Set(m.m[0][3], m.m[1][3], m.m[2][3]);
		Matrix4x4 rot_mat;
		rot_mat.m[0][0] = m.m[0][0] / outScale->x;
		rot_mat.m[1][0] = m.m[1][0] / outScale->x;
		rot_mat.m[2][0] = m.m[2][0] / outScale->x;
		rot_mat.m[3][0] = 0;
		rot_mat.m[0][1] = m.m[0][1] / outScale->y;
		rot_mat.m[1][1] = m.m[1][1] / outScale->y;
		rot_mat.m[2][1] = m.m[2][1] / outScale->y;
		rot_mat.m[3][1] = 0;
		rot_mat.m[0][2] = m.m[0][2] / outScale->z;
		rot_mat.m[1][2] = m.m[1][2] / outScale->z;
		rot_mat.m[2][2] = m.m[2][2] / outScale->z;
		rot_mat.m[3][2] = 0;
		rot_mat.m[0][3] = 0;
		rot_mat.m[1][3] = 0;
		rot_mat.m[2][3] = 0;
		rot_mat.m[3][3] = 1;
		*outRotation = rot_mat.ToRotation();
	}


	Quaternion Matrix4x4::ToRotation() const
	{
		float fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
		float fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
		float fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
		float fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];

		int biggestIndex = 0;
		float fourBiggestSquaredMinus1 = fourWSquaredMinus1;
		if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourXSquaredMinus1;
			biggestIndex = 1;
		}
		if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourYSquaredMinus1;
			biggestIndex = 2;
		}
		if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourZSquaredMinus1;
			biggestIndex = 3;
		}

		float biggestVal = sqrtf(fourBiggestSquaredMinus1 + 1.f) * 0.5f;
		float mult = 0.25f / biggestVal;

		Quaternion result;
		switch (biggestIndex)
		{
		case 0:
			result.w = biggestVal;
			result.x = (m[2][1] - m[1][2]) * mult;
			result.y = (m[0][2] - m[2][0]) * mult;
			result.z = (m[1][0] - m[0][1]) * mult;
			break;
		case 1:
			result.w = (m[2][1] - m[1][2]) * mult;
			result.x = biggestVal;
			result.y = (m[1][0] + m[0][1]) * mult;
			result.z = (m[0][2] + m[2][0]) * mult;
			break;
		case 2:
			result.w = (m[0][2] - m[2][0]) * mult;
			result.x = (m[1][0] + m[0][1]) * mult;
			result.y = biggestVal;
			result.z = (m[2][1] + m[1][2]) * mult;
			break;
		case 3:
			result.w = (m[1][0] - m[0][1]) * mult;
			result.x = (m[0][2] + m[2][0]) * mult;
			result.y = (m[2][1] + m[1][2]) * mult;
			result.z = biggestVal;
			break;

		default:					// Silence a -Wswitch-default warning in GCC. Should never actually get here. Assert is just for sanity.
			assert(false);
			break;
		}
		result.NormalizeSelf();
		return result;
	}
	
	Matrix4x4 Matrix4x4::FromRotation(const Quaternion& rotation)
	{
		//rotation should be unit quaternion

		// Real-time rendering 3rd, p76
		auto& q = rotation;
		Matrix4x4 result;
		float x = 2.0f * rotation.x;
		float y = 2.0f * rotation.y;
		float z = 2.0f * rotation.z;
		float qxx = q.x * x;
		float qyy = q.y * y;
		float qzz = q.z * z;
		float qxy = q.x * y;
		float qxz = q.x * z;
		float qyz = q.y * z;
		float qwx = q.w * x;
		float qwy = q.w * y;
		float qwz = q.w * z;
		
		result.m[0][0] = 1.f - (qyy + qzz);
		result.m[1][0] = qxy + qwz;
		result.m[2][0] = qxz - qwy;
		
		result.m[0][1] = qxy - qwz;
		result.m[1][1] = 1.f - (qxx + qzz);
		result.m[2][1] = qyz + qwx;
		
		result.m[0][2] = qxz + qwy;
		result.m[1][2] = qyz - qwx;
		result.m[2][2] = 1.f - (qxx + qyy);
		return result;
	}

	FishEngine::Matrix4x4 Matrix4x4::Perspective(const float fovy, const float aspect, const float zNear, const float zFar)
	{
		//return glm::perspectiveLH(glm::radians(fovy), aspect, zNear, zFar);
		assert(fabsf(aspect - std::numeric_limits<float>::epsilon()) > 0.f);

		float const tanHalfFovy = tan(Mathf::Radians(fovy) / 2.f);

		Matrix4x4 result;
		result.m[0][0] = 1.f / (aspect * tanHalfFovy);
		result.m[1][1] = 1.f / (tanHalfFovy);
		result.m[3][2] = 1.f;

//#		if GLM_DEPTH_CLIP_SPACE == GLM_DEPTH_ZERO_TO_ONE
//        result.m[2][2] = zFar / (zFar - zNear);
//        result.m[2][3] = -(zFar * zNear) / (zFar - zNear);

//#		else
		result.m[2][2] = (zFar + zNear) / (zFar - zNear);
		result.m[2][3] = -(2.f * zFar * zNear) / (zFar - zNear);
//#		endif
		result.m[3][3] = 0.f;
		return result;
	}

	

	FishEngine::Matrix4x4 Matrix4x4::Ortho(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar)
	{
		//return glm::orthoLH(left, right, bottom, top, zNear, zFar);
		Matrix4x4 result;
		result.m[0][0] = 2.f / (right - left);
		result.m[1][1] = 2.f / (top - bottom);
		result.m[0][3] = -(right + left) / (right - left);
		result.m[1][3] = -(top + bottom) / (top - bottom);

//#		if GLM_DEPTH_CLIP_SPACE == GLM_DEPTH_ZERO_TO_ONE
//        result.m[2][2] = 1.f / (zFar - zNear);
//        result.m[2][3] = -zNear / (zFar - zNear);
//#		else
		result.m[2][2] = 2.f / (zFar - zNear);
		result.m[2][3] = -(zFar + zNear) / (zFar - zNear);
//#		endif

		return result;
	}

	Matrix4x4 Matrix4x4::LookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
	{
		Vector3 f = Vector3::Normalize(target - eye);
		Vector3 s = Vector3::Normalize(Vector3::Cross(up, f));
		Vector3 u = Vector3::Cross(f, s);

		Matrix4x4 result;
		result.m[0][0] = s.x;
		result.m[0][1] = s.y;
		result.m[0][2] = s.z;
		result.m[1][0] = u.x;
		result.m[1][1] = u.y;
		result.m[1][2] = u.z;
		result.m[2][0] = f.x;
		result.m[2][1] = f.y;
		result.m[2][2] = f.z;
		result.m[0][3] = -Vector3::Dot(s, eye);
		result.m[1][3] = -Vector3::Dot(u, eye);
		result.m[2][3] = -Vector3::Dot(f, eye);
		return result;
	}

	const Matrix4x4 Matrix4x4::identity(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	const Matrix4x4 Matrix4x4::zero(
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0);
}
