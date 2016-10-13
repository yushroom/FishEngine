#ifndef Transform_hpp
#define Transform_hpp

#include "GLEnvironment.hpp"
#include "Component.hpp"
#include "Quaternion.hpp"
#include "Matrix4x4.hpp"

NAMESPACE_FISHENGINE_BEGIN

enum class Space {
    World,
    Self
};

class Transform : public Component
{
public:
    InjectClassName(Transform)
    
    Transform();
    
    ~Transform();

    
    //virtual void OnInspectorGUI() override;

    uint32_t childCount() const {
        return (uint32_t)m_children.size();
    }
    
    // The position of the transform in world space.
    Vector3 position() const {
        Update();
        //return m_localToWorldMatrix * Vector4(0, 0, 0, 1);
        return m_localToWorldMatrix.MultiplyPoint(0, 0, 0);
    }

    // The global scale of the object(Read Only).
    Vector3 lossyScale() const {
        auto p = m_parent.lock();
        if (p != nullptr)
            return m_localScale * p->lossyScale();
        return m_localScale;
    }

    // The rotation of the transform in world space stored as a Quaternion.
    Quaternion rotation() const {
        Update();
        return m_rotation;
    }

    // The rotation as Euler angles in degrees.
    Vector3 eulerAngles() const {
        return rotation().eulerAngles();
    }

    // Position of the transform relative to the parent transform.
    Vector3 localPosition() const {
        return m_localPosition;
    }

    // The scale of the transform relative to the parent.
    Vector3 localScale() const {
        return m_localScale;
    }

    void setLocalPosition(const Vector3& position) {
        m_localPosition = position;
        MakeDirty();
    }

    void setLocalPosition(const float x, const float y, const float z) {
        m_localPosition.x = x;
        m_localPosition.y = y;
        m_localPosition.z = z;
        MakeDirty();
    }

    void setLocalScale(const Vector3& scale) {
        m_localScale = scale;
        MakeDirty();
    }

    void setLocalScale(const float x, const float y, const float z) {
        m_localScale.x = x;
        m_localScale.y = y;
        m_localScale.z = z;
        MakeDirty();
    }

    // The rotation of the transform relative to the parent transform's rotation.
    Quaternion localRotation() const {
        return m_localRotation;
    }
    
    void setLocalRotation(const Quaternion& rotation) {
        m_localRotation = rotation;
        MakeDirty();
    }

    // The rotation as Euler angles in degrees relative to the parent transform's rotation.
    Vector3 localEulerAngles() const {
        m_localEulerAngles = m_localRotation.eulerAngles();
        return m_localEulerAngles;
    }

    void setLocalEulerAngles(const Vector3& eulerAngles) {
        m_localRotation.setEulerAngles(eulerAngles);
        MakeDirty();
    }
    
    void setLocalEulerAngles(const float x, const float y, const float z) {
        setLocalEulerAngles(Vector3(x, y, z));
    }

    // position is in world space
    void setPosition(const Vector3& position) {
        if (!m_parent.expired()) {
            m_localPosition = parent()->worldToLocalMatrix().MultiplyPoint(position);
        } else {
            m_localPosition = position;
        }
        MakeDirty();
    }

    void setPosition(const float x, const float y, const float z) {
        setPosition(Vector3(x, y, z));
    }

    void setRotation(const Quaternion& new_rotation)
    {
        if (m_parent.expired()) {
            m_localRotation = new_rotation;
        } else {
            m_localRotation = Quaternion::Inverse(parent()->rotation()) * new_rotation;
        }
        MakeDirty();
    }

    void setEulerAngles(const Vector3& eulerAngles);

    // Matrix that transforms a point from local space into world space (Read Only).
    Matrix4x4 localToWorldMatrix() const {
        Update();
        return m_localToWorldMatrix;
    }
    
    void setLocalToWorldMatrix(const Matrix4x4& localToWorld) {
        m_localToWorldMatrix = localToWorld;
        Matrix4x4::Decompose(localToWorld, &m_localPosition, &m_localRotation, &m_localScale);
        MakeDirty();
    }

    Matrix4x4 worldToLocalMatrix() const {
        Update();
        return m_worldToLocalMatrix;
    }


    // direction (1, 0, 0) in world space.
    Vector3 right() const {
        return Vector3::Normalize(rotation() * Vector3(1, 0, 0));
    }

    // direction (0, 1, 0) in world space
    Vector3 up() const {
        return Vector3::Normalize(rotation() * Vector3(0, 1, 0));
    }

    // direction (0, 0, 1) in world space.
    Vector3 forward() const {
        return Vector3::Normalize(rotation() * Vector3(0, 0, 1));
    }

    //void setForward(const Vector3& forward) {
    //    auto rot = fromToRotation(this->forward(), forward);
    //    //m_localRotation = rot * m_localRotation;
    //    m_isDirty = true;
    //}

    void Update() const;

    // Rotates the transform so the forward vector points at /target/'s current position.
    void LookAt(const Vector3& target, const Vector3& worldUp = Vector3(0, 1, 0));

    void LookAt(float x, float y, float z) {
        LookAt(Vector3(x, y, z));
    }

    // Transforms direction from local space to world space.
    Vector3 TransformDirection(const Vector3& direction) const;

    // Transforms a direction from world space to local space. The opposite of Transform.TransformDirection.
    Vector3 InverseTransformDirection(const Vector3& direction) const;

    void Translate(const Vector3& translation, Space relativeTo = Space::Self);

    void Translate(float x, float y, float z, Space relativeTo = Space::Self) {
        Translate(Vector3(x, y, z), relativeTo);
    }
     
    // Applies a rotation of eulerAngles.z degrees around the z axis, eulerAngles.x degrees around the x axis, and eulerAngles.y degrees around the y axis (in that order).
    void Rotate(Vector3 eulerAngles, Space relativeTo = Space::Self);
    
    // Applies a rotation of zAngle degrees around the z axis, xAngle degrees around the x axis, and yAngle degrees around the y axis (in that order)
    void Rotate(float xAngle, float yAngle, float zAngle, Space relativeTo = Space::Self) {
        this->Rotate(Vector3(xAngle, yAngle, zAngle), relativeTo);
    }

    // Rotates the transform about axis passing through point in world coordinates by angle degrees.
    void RotateAround(const Vector3& point, const Vector3& axis, float angle);

    // The parent of the transform.
    std::shared_ptr<Transform> parent() const {
        return m_parent.lock();
    }

    void SetParent(std::shared_ptr<Transform> parent);
    
    //========== Public Functions ==========//
    
    // Unparents all children.
    void DetachChildren();
    
    // Finds a child by name and returns it.
    std::shared_ptr<Transform> Find(const std::string& name) const;
    
    /* Returns a transform child by index.
     * @param index	Index of the child transform to return. Must be smaller than Transform.childCount.
     * @return: Transform Transform child by index.
     */
    std::shared_ptr<Transform> GetChild(const size_t index);

    
    const std::list<std::weak_ptr<Transform>>& children() const {
        return m_children;
    }
    
    // Returns the component of Type type if the game object has one attached, null if it doesn't.
//    template<typename T>
//    std::shared_ptr<T> GetComponent() const {
//        return gameObject()->GetComponent<T>();
//    }
    
private:
    friend class FishEditor::EditorGUI;
    friend class Scene;

    Vector3         m_localPosition;
    Vector3         m_localScale;
    Quaternion      m_localRotation;
    mutable Quaternion  m_rotation; // cache, do not use its value directly, use rotation() instead.
    mutable Vector3     m_localEulerAngles;
    mutable bool        m_isDirty;

    std::weak_ptr<Transform>            m_parent;
    std::list<std::weak_ptr<Transform>> m_children;
    
    mutable Matrix4x4 m_localToWorldMatrix; // localToWorld
    mutable Matrix4x4 m_worldToLocalMatrix; // worldToLocal

    bool dirtyInHierarchy() const;
    void MakeDirty() const;
    
    friend class boost::serialization::access;
    template<class Archive>
    inline void serialize(Archive& ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(m_localPosition)
        & BOOST_SERIALIZATION_NVP(m_localRotation)
        & BOOST_SERIALIZATION_NVP(m_localScale);
    }
};

NAMESPACE_FISHENGINE_END

#endif // Transform_hpp
