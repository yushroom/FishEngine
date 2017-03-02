#ifndef Transform_hpp
#define Transform_hpp

//#include "GLEnvironment.hpp"
#include "Component.hpp"
#include "Quaternion.hpp"
#include "Matrix4x4.hpp"
#include "Bounds.hpp"
#include "ReflectClass.hpp"

namespace FishEngine
{
    enum class Space
    {
        World,
        Self
    };

    class FE_EXPORT Transform : public Component
    {
    public:
        InjectClassName(Transform);

        Transform();

        ~Transform();
		
		// The position of the transform in world space.
		Vector3 position() const
		{
			UpdateMatrix();
			//return m_localToWorldMatrix.MultiplyPoint(0, 0, 0);
			auto& l2w = m_localToWorldMatrix;
			return Vector3(l2w.m[0][3], l2w.m[1][3], l2w.m[2][3]);
		}
		
		// position is in world space
		void setPosition(const Vector3& position)
		{
			if (!m_parent.expired())
			{
				m_localPosition = parent()->worldToLocalMatrix().MultiplyPoint(position);
			}
			else
			{
				m_localPosition = position;
			}
			MakeDirty();
		}
		
		void setPosition(const float x, const float y, const float z)
		{
			setPosition(Vector3(x, y, z));
		}
		
		
		// Position of the transform relative to the parent transform.
		Vector3 localPosition() const
		{
			return m_localPosition;
		}
		
		void setLocalPosition(const Vector3& position)
		{
			m_localPosition = position;
			MakeDirty();
		}
		
		void setLocalPosition(const float x, const float y, const float z)
		{
			m_localPosition.Set(x, y, z);
			MakeDirty();
		}

		
		// The rotation as Euler angles in degrees.
		Vector3 eulerAngles() const
		{
			return rotation().eulerAngles();
		}
		
		void setEulerAngles(const Vector3& eulerAngles);
		
		
		// The rotation as Euler angles in degrees relative to the parent transform's rotation.
		Vector3 localEulerAngles() const
		{
			return m_localRotation.eulerAngles();
		}
		
		void setLocalEulerAngles(const Vector3& eulerAngles)
		{
			m_localRotation.setEulerAngles(eulerAngles);
			MakeDirty();
		}
		
		void setLocalEulerAngles(const float x, const float y, const float z)
		{
			setLocalEulerAngles(Vector3(x, y, z));
		}

		
		// direction (1, 0, 0) in world space.
		Vector3 right() const
		{
			return Vector3::Normalize(rotation() * Vector3::right);
		}
		
		// direction (0, 1, 0) in world space
		Vector3 up() const
		{
			return Vector3::Normalize(rotation() * Vector3::up);
		}
		
		// direction (0, 0, 1) in world space.
		Vector3 forward() const
		{
			return Vector3::Normalize(rotation() * Vector3::forward);
		}
		

		// The rotation of the transform in world space stored as a Quaternion.
		Quaternion rotation() const
		{
			UpdateMatrix();
			return m_localToWorldMatrix.ToRotation();
		}
		
		void setRotation(const Quaternion& new_rotation)
		{
			if (m_parent.expired())
			{
				m_localRotation = new_rotation;
			}
			else
			{
				m_localRotation = Quaternion::Inverse(parent()->rotation()) * new_rotation;
			}
			MakeDirty();
		}
		
		
		// The rotation of the transform relative to the parent transform's rotation.
		Quaternion localRotation() const
		{
			return m_localRotation;
		}
		
		void setLocalRotation(const Quaternion& rotation)
		{
			m_localRotation = rotation;
			MakeDirty();
		}
		
		// The scale of the transform relative to the parent.
		Vector3 localScale() const
		{
			return m_localScale;
		}
		
		void setLocalScale(const Vector3& scale)
		{
			m_localScale = scale;
			MakeDirty();
		}
		
		void setLocalScale(const float x, const float y, const float z)
		{
			m_localScale.Set(x, y, z);
			MakeDirty();
		}
		
		void setLocalScale(const float scale)
		{
			m_localScale.x = m_localScale.y = m_localScale.z = scale;
			MakeDirty();
		}

		// The parent of the transform.
		TransformPtr parent() const
		{
			return m_parent.lock();
		}
		
		void setParent(TransformPtr const & parent, bool worldPositionStays = true)
		{
			SetParent(parent, worldPositionStays);
		}

		
		const Matrix4x4 & worldToLocalMatrix() const
		{
			UpdateMatrix();
			return m_worldToLocalMatrix;
		}
		
		
		// Matrix that transforms a point from local space into world space (Read Only).
		Matrix4x4 localToWorldMatrix() const
		{
			UpdateMatrix();
			return m_localToWorldMatrix;
		}
		
//		Matrix4x4 localToWorldMatrixFast() const
//		{
//			m_localToWorldMatrix = Matrix4x4::TRS(m_localPosition, m_localRotation, m_localScale);
//			if (!m_parent.expired()) {
//				m_localToWorldMatrix = m_parent.lock()->localToWorldMatrix() * m_localToWorldMatrix;
//			}
//			return m_localToWorldMatrix;
//		}
		
		void setLocalToWorldMatrix(const Matrix4x4& localToWorld)
		{
			m_localToWorldMatrix = localToWorld;
			Matrix4x4::Decompose(localToWorld, &m_localPosition, &m_localRotation, &m_localScale);
			MakeDirty();
		}


        uint32_t childCount() const
        {
            return (uint32_t)m_children.size();
        }


        // The global scale of the object(Read Only).
        Vector3 lossyScale() const
        {
            auto p = parent();
            if (p != nullptr)
                return m_localScale * p->lossyScale();
            return m_localScale;
        }


		bool hasChanged();

		void Translate(const Vector3& translation, Space relativeTo = Space::Self);
		
		void Translate(float x, float y, float z, Space relativeTo = Space::Self)
		{
			Translate(Vector3(x, y, z), relativeTo);
		}
		
		void Translate(const Vector3 & translation, const TransformPtr & relativeTo)
		{
			if (relativeTo != nullptr)
			{
				setPosition(position() + relativeTo->TransformDirection(translation));
			}
			else
			{
				setPosition(position() + translation);
			}
		}
		
		void Translate(float x, float y, float z, const TransformPtr & relativeTo)
		{
			Translate(Vector3(x, y, z), relativeTo);
		}
		
		
		// Applies a rotation of eulerAngles.z degrees around the z axis, eulerAngles.x degrees around the x axis, and eulerAngles.y degrees around the y axis (in that order).
		void Rotate(const Vector3 & eulerAngles, Space relativeTo = Space::Self);
		
		// Applies a rotation of zAngle degrees around the z axis, xAngle degrees around the x axis, and yAngle degrees around the y axis (in that order)
		void Rotate(float xAngle, float yAngle, float zAngle, Space relativeTo = Space::Self)
		{
			this->Rotate(Vector3(xAngle, yAngle, zAngle), relativeTo);
		}
		
		// Rotates the transform about axis passing through point in world coordinates by angle degrees.
		void RotateAround(const Vector3& point, const Vector3& axis, float angle);
		
		
		// Rotates the transform so the forward vector points at /target/'s current position.
		void LookAt(const Vector3& target, const Vector3& worldUp = Vector3::up);
		
		void LookAt(float x, float y, float z)
		{
			LookAt(Vector3(x, y, z));
		}


        // Transforms direction from local space to world space.
        Vector3 TransformDirection(const Vector3& direction) const;

        // Transforms a direction from world space to local space. The opposite of Transform.TransformDirection.
        Vector3 InverseTransformDirection(const Vector3& direction) const;

        // Transforms bounds(AABB) from local space to world space.
        Bounds TransformBounds(const Bounds& bounds) const;
        
        // Transforms bounds(AABB) from world space to local space. The opposite of TransformBounds.
        Bounds InverseTransformBounds(const Bounds& bounds) const;


		// parent: The parent Transform to use.
		// worldPositionStays: If true, the parent-relative position, scale and rotation are modified such that the object keeps the same world space position, rotation and scale as before.
        void SetParent(TransformPtr const & parent, bool worldPositionStays = true);

        //========== Public Functions ==========//

        // Unparents all children.
        void DetachChildren();

        // Finds a child by name and returns it.
        TransformPtr Find(const std::string& name) const;

        /* Returns a transform child by index.
         * @param index	Index of the child transform to return. Must be smaller than Transform.childCount.
         * @return: Transform Transform child by index.
         */
        TransformPtr GetChild(const size_t index);


        const std::list<std::weak_ptr<Transform>>& children() const
        {
            return m_children;
        }

        // Returns the component of Type type if the game object has one attached, null if it doesn't.
    //    template<typename T>
    //    std::shared_ptr<T> GetComponent() const {
    //        return gameObject()->GetComponent<T>();
    //    }
		
		
		
		//void setForward(const Vector3& forward) {
		//    auto rot = fromToRotation(this->forward(), forward);
		//    //m_localRotation = rot * m_localRotation;
		//    m_isDirty = true;
		//}
		
		void UpdateMatrix() const;
		//void UpdateFast() const;
		
		
    private:
        friend class FishEditor::Inspector;
        friend class Scene;

        Vector3                             m_localPosition;
        Vector3                             m_localScale;
        Quaternion                          m_localRotation;

        Meta(HideInInspector)
        std::weak_ptr<Transform>            m_parent;
        
        Meta(HideInInspector)
        std::list<std::weak_ptr<Transform>> m_children;

        Meta(NonSerializable)
        mutable bool                        m_isDirty;

        Meta(NonSerializable)
        mutable Matrix4x4                   m_localToWorldMatrix; // localToWorld

        Meta(NonSerializable)
        mutable Matrix4x4                   m_worldToLocalMatrix; // worldToLocal

        //bool dirtyInHierarchy() const;
        void MakeDirty() const;
    };

	/************************************************************************/
	/* Transform Serialization                                              */
	/************************************************************************/
}

#endif // Transform_hpp
