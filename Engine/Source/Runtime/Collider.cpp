#include "Collider.hpp"
#include "Transform.hpp"
#include "GameObject.hpp"
#include "Rigidbody.hpp"
#include "PhysicsSystem.hpp"

using namespace physx;
extern physx::PxPhysics*    gPhysics;
extern physx::PxScene*      gScene;
extern physx::PxMaterial*   gMaterial;

namespace FishEngine
{
    void Collider::Start()
    {
        //m_physxShape = gPhysics->createShape(PxSphereGeometry(m_radius), *gMaterial);
        CreatePhysicsShape();
        auto rigidbody = gameObject()->GetComponent<Rigidbody>();
        if (rigidbody == nullptr)
		{
            //PxRigidStatic* rigidStatic = PxCreatePlane(*gPhysics, PxPlane(0,1,0,0), *gMaterial);
            const auto& t = transform();
            auto p = t->position();
            auto q = t->rotation();
            auto rigidStatic = PxCreateStatic(*gPhysics, PxTransform(p.x, p.y, p.z, PxQuat(q.x, q.y, q.z, q.w)), *m_physxShape);
            gScene->addActor(*rigidStatic);
        }
//        else {
//            rigidbody->Initialize(m_physxShape);
//        }
    }
}
