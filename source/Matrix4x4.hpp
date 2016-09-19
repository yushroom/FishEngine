#ifndef Matrix4x4_hpp
#define Matrix4x4_hpp

class Matrix4x4
{
public:
    Vector4 operator[](int i) {
        return v[i];
    }

    float determinant() const {

    }

    Matrix4x4 inverse() const {

    }

    bool isIdentity() const {

    }

    Matrix4x4 transpose() const {
        
    }

public:
    static Matrix4x4 identity;
    static Matrix4x4 zero;

private:
    union {
        float m[4][4];
        struct {
            Vector4 v[4];
        }
    }
}

#endif //Matrix4x4_hpp