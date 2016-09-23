#ifndef Matrix4x4_hpp
#define Matrix4x4_hpp

#include "Vector3.hpp"
#include "Quaternion.hpp"
//#define GLM_FORCE_LEFT_HANDED
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE
//#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
//#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr

namespace FishEngine {

// Matrices are row major.
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

    //Matrix4x4(const glm::mat4& glm_mat4)
    //{
    //    memcpy(m, glm::value_ptr(glm::transpose(glm_mat4)), 16*sizeof(float));
    //}

    //operator glm::mat4() const {
    //    glm::mat4 result;
    //    memcpy(glm::value_ptr(result), transpose().m, 16 * sizeof(float));
    //    return result;
    //}

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

    friend Matrix4x4 operator*(const Matrix4x4& lhs, Matrix4x4 rhs)
    {
        //return glm::mat4(lhs) * glm::mat4(rhs);
        Matrix4x4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = lhs.m[i][0] * rhs.m[0][j] + lhs.m[i][1] * rhs.m[1][j] + lhs.m[i][2] * rhs.m[2][j] + lhs.m[i][3] * rhs.m[3][j];
            }
        }
        return result;
    }

    friend const Vector4 operator*(const Matrix4x4& lhs, const Vector4& rhs) {
        ////return glm::mat4(lhs) * glm::vec4(rhs);
        //Vector4 result;
        //for (int i = 0; i < 4; i++) {
        //    //result[i] = lhs.m[i][0] * rhs.x + lhs.m[i][1] * rhs.y + lhs.m[i][2] * rhs.z + lhs.m[i][3] * rhs.w;
        //    result[i] = Vector4::Dot(lhs.rows[i], rhs);
        //}
        //return result;
        return Vector4(
            Vector4::Dot(lhs.rows[0], rhs), 
            Vector4::Dot(lhs.rows[1], rhs), 
            Vector4::Dot(lhs.rows[2], rhs), 
            Vector4::Dot(lhs.rows[3], rhs));
    }


    //void operator*=(const Matrix4x4& rhs)
    //{
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

    friend Matrix4x4 operator*(const Matrix4x4& m, const float f);
    //{
    //    Matrix4x4 result(m);
    //    for (int i = 0; i < 4; ++i)
    //        for (int j = 0; j < 4; ++j)
    //            result.m[i][j] *= f;
    //    return result;
    //}

    // Transforms a position by this matrix (generic).
    Vector3 MultiplyPoint(float x, float y, float z)
    {
        Vector3 result;
        result.x = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3];
        result.y = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3];
        result.z = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3];
        float num = m[3][0] * x + m[3][1] * y + m[3][2] * z + m[3][3];
        num = 1.f / num;
        result.x *= num;
        result.y *= num;
        result.z *= num;
        return result;
    }

    // Transforms a position by this matrix (generic).
    Vector3 MultiplyPoint(const Vector3& v)
    {
        return MultiplyPoint(v.x, v.y, v.z);
    }

    // Transforms a position by this matrix (fast).
    Vector3 MultiplyPoint3x4(float x, float y, float z)
    {
        Vector3 result;
        result.x = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3];
        result.y = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3];
        result.z = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3];
        return result;
    }

    // Transforms a position by this matrix (fast).
    Vector3 MultiplyPoint3x4(const Vector3& v)
    {
        return MultiplyPoint3x4(v.x, v.y, v.z);
    }

    // Transforms a direction by this matrix.
    Vector3 MultiplyVector(float x, float y, float z)
    {
        Vector3 result;
        result.x = m[0][0] * x + m[0][1] * y + m[0][2] * z;
        result.y = m[1][0] * x + m[1][1] * y + m[1][2] * z;
        result.z = m[2][0] * x + m[2][1] * y + m[2][2] * z;
        return result;
    }

    // Transforms a direction by this matrix.
    Vector3 MultiplyVector(const Vector3& v)
    {
        return MultiplyVector(v.x, v.y, v.z);
    }

    //float determinant() const
    //{
    //    glm::mat4 m = *this;
    //    return glm::determinant(m);
    //}

    //static float Determinant(const Matrix4x4& mat)
    //{
    //    return mat.determinant();
    //}

    Matrix4x4 inverse() const
    {
        return Matrix4x4::Inverse(*this);
    }

    static Matrix4x4 Inverse(const Matrix4x4& m);

    bool isIdentity() const;

    Matrix4x4 transpose() const
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i][j] = m[j][i];
            }
        }
        return result;
    }


    void SetTRS(const Vector3& pos, const Quaternion& q, const Vector3& s);

    static Matrix4x4 TRS(const Vector3& pos, const Quaternion& q, const Vector3& s)
    {
        Matrix4x4 mat;
        mat.SetTRS(pos, q, s);
        return mat;
    }

    // get concatenation of transforms and its inverse
    static void TRS(
        const Vector3&      translation,
        const Quaternion&   rotation,
        const Vector3&      scale,
        Matrix4x4&          outLocalToWorld, 
        Matrix4x4&          outWorldToLocal);


    static Matrix4x4 Scale(float x, float y, float z)
    {
        return Matrix4x4(
            x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0,
            0, 0, 0, 1);
    }

    static Matrix4x4 Scale(const Vector3& scale)
    {
        return Scale(scale.x, scale.y, scale.z);
    }

    Quaternion ToRotation() const;
    static Matrix4x4 FromRotation(const Quaternion& rotation);

    static Matrix4x4 Perspective(float fovy, float aspect, float zNear, float zFar);
    
    static Matrix4x4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar);

    static Matrix4x4 LookAt(const Vector3& eye, const Vector3& center, const Vector3 up);

public:
    static const Matrix4x4 identity;
    static const Matrix4x4 zero;
};

}

#endif //Matrix4x4_hpp
