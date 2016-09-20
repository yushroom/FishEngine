#ifndef Matrix4x4_hpp
#define Matrix4x4_hpp

#include "Vector3.hpp"
#include "Quaternion.hpp"
#define GLM_FORCE_LEFT_HANDED
//#include <glm/glm.hpp>
//#include <glm/vec3.hpp>                 // glm::vec3
//#include <glm/vec4.hpp>                 // glm::vec4
#include <glm/mat4x4.hpp>               // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
//#include <glm/gtc/constants.hpp>        // glm::pi
#include <glm/gtx/quaternion.hpp>       // glm::quat
#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr


// Matrices are column major.
class Matrix4x4
{
public:
    union {
        float m[4][4];
        struct { Vector4 rows[4]; };
    };

    Matrix4x4() {
        m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
        m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
        m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
        m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
    }
    Matrix4x4(float mat[4][4])
    {
        memcpy(m, mat, 16 * sizeof(float));
    }

    Matrix4x4(
        float m00, float m01, float m02, float m03, // row 0
        float m10, float m11, float m12, float m13, // row 1
        float m20, float m21, float m22, float m23, // row 2
        float m30, float m31, float m32, float m33) // row 3
    {
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
        m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
    }

    Matrix4x4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3)
        : rows{row0, row1, row2, row3}
    {

    }

    Matrix4x4(const glm::mat4& glm_mat4)
    {
        memcpy(m, glm::value_ptr(glm_mat4), 16*sizeof(float));
    }

    operator glm::mat4() const {
        glm::mat4 result;
        memcpy(glm::value_ptr(result), m, 16 * sizeof(float));
        return result;
    }

    const float* data() const {
        return m[0];
    }

    float* data() {
        return m[0];
    }

    //Vector4& operator[](int index) {
    //    return rows[index];
    //}

    //const Vector4 operator[](int index) const {
    //    return rows[index];
    //}

    //friend Matrix4x4 operator*(const Matrix4x4& lhs, Matrix4x4 rhs)
    //{
    //    Matrix4x4 result;
    //    for (int i = 0; i < 4; i++) {
    //        for (int j = 0; j < 4; j++) {
    //            result.m[i][j] = lhs.m[i][0] * rhs.m[0][j] + lhs.m[i][1] * rhs.m[1][j] + lhs.m[i][2] * rhs.m[2][j] + lhs.m[i][3] * rhs.m[3][j];
    //        }
    //    }
    //    return result;
    //}

    //friend const Vector3 operator*(const Matrix4x4& lhs, const Vector4& rhs) {
    //    Vector3 result;
    //    for (int i = 0; i < 3; i++) {
    //        result[i] = lhs.m[i][0]*rhs.x + lhs.m[i][1]*rhs.y + lhs.m[i][2]*rhs.z + lhs.m[i][3] * rhs.w;
    //    }
    //    return result;
    //}

    //void operator*=(const Matrix4x4& rhs) {
    //    for (int i = 0; i < 4; i++) {
    //        float f[4];
    //        for (int j = 0; j < 4; j++) {
    //            f[j] = m[i][0] * rhs.m[0][j] + m[i][1] * rhs.m[1][j] +
    //                m[i][2] * rhs.m[2][j] + m[i][3] * rhs.m[3][j];
    //        }
    //        for (int j = 0; j < 4; j++) {
    //            m[i][j] = f[j];
    //        }
    //    }
    //}

    //friend Matrix4x4 operator*(const Matrix4x4& m, const float f)
    //{
    //    Matrix4x4 result(m);
    //    for (int i = 0; i < 4; ++i)
    //        for (int j = 0; j < 4; ++j)
    //            result.m[i][j] *= f;
    //    return result;
    //}

    Vector3 MultiplyPoint(Vector3 v)
    {
        Vector3 result;
        result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
        result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
        result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
        float num = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3];
        num = 1.f / num;
        result.x *= num;
        result.y *= num;
        result.z *= num;
        return result;
    }

    Vector3 MultiplyPoint3x4(Vector3 v)
    {
        Vector3 result;
        result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
        result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
        result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
        return result;
    }

    float determinant() const
    {
        glm::mat4 m = *this;
        return glm::determinant(m);
    }

    static float Determinant(const Matrix4x4& mat)
    {
        return mat.determinant();
    }

    Matrix4x4 inverse() const
    {
        return Matrix4x4::Inverse(*this);
    }

    static Matrix4x4 Inverse(const Matrix4x4& m) {
        return glm::inverse((glm::mat4)m);
        //float Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
        //float Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
        //float Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

        //float Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
        //float Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
        //float Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

        //float Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
        //float Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
        //float Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

        //float Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
        //float Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
        //float Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

        //float Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
        //float Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
        //float Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

        //float Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
        //float Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
        //float Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

        //Vector4 Fac0(Coef00, Coef00, Coef02, Coef03);
        //Vector4 Fac1(Coef04, Coef04, Coef06, Coef07);
        //Vector4 Fac2(Coef08, Coef08, Coef10, Coef11);
        //Vector4 Fac3(Coef12, Coef12, Coef14, Coef15);
        //Vector4 Fac4(Coef16, Coef16, Coef18, Coef19);
        //Vector4 Fac5(Coef20, Coef20, Coef22, Coef23);

        //Vector4 Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
        //Vector4 Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
        //Vector4 Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
        //Vector4 Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

        //Vector4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
        //Vector4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
        //Vector4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
        //Vector4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

        //Vector4 SignA(+1, -1, +1, -1);
        //Vector4 SignB(-1, +1, -1, +1);
        //Matrix4x4 result(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

        //Vector4 Row0(result[0][0], result[1][0], result[2][0], result[3][0]);

        //Vector4 Dot0(m[0] * Row0);
        //float Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

        //float OneOverDeterminant = 1.f / Dot1;

        //return result * OneOverDeterminant;
    }

    bool isIdentity() const;

    //Matrix4x4 transpose() const
    //{
    //    Matrix4x4 result;
    //    for (int i = 0; i < 4; ++i) {
    //        for (int j = 0; j < 4; ++j) {
    //            result.m[i][j] = m[j][i];
    //        }
    //    }
    //    return result;
    //}


    //void SetTRS(const Vector3& pos, const Quaternion& q, const Vector3& s)
    //{
    //    *this = glm::scale(glm::translate(glm::mat4(1.0f), (glm::vec3)pos) * glm::mat4_cast((glm::quat)q), (glm::vec3)s);
    //}

    //static Matrix4x4 TRS(const Vector3& pos, const Quaternion& q, const Vector3& s)
    //{
    //    Matrix4x4 mat;
    //    mat.SetTRS(pos, q, s);
    //    return mat;
    //}

    //static Matrix4x4 LookAtLH(const Vector3& eye, const Vector3& center, const Vector3 up)
    //{
    //    Vector3 const f(Vector3::Normalize(center - eye));
    //    Vector3 const s(Vector3::Normalize(Vector3::Cross(up, f)));
    //    Vector3 const u(Vector3::Cross(f, s));

    //    Matrix4x4 Result;
    //    Result[0][0] = s.x;
    //    Result[1][0] = s.y;
    //    Result[2][0] = s.z;
    //    Result[0][1] = u.x;
    //    Result[1][1] = u.y;
    //    Result[2][1] = u.z;
    //    Result[0][2] = f.x;
    //    Result[1][2] = f.y;
    //    Result[2][2] = f.z;
    //    Result[3][0] = -Vector3::Dot(s, eye);
    //    Result[3][1] = -Vector3::Dot(u, eye);
    //    Result[3][2] = -Vector3::Dot(f, eye);
    //    return Result;
    //}

public:
    static const Matrix4x4 identity;
    static const Matrix4x4 zero;
};

#endif //Matrix4x4_hpp