#include "src/bullet/collider.h"

namespace legend {
namespace bullet {

Collider::Collider() {}
Collider::~Collider() {}
bool Collider::Update() { return false; }

btRigidBody* Collider::GetRigidBody()
{
    return rigid_body_.get(); 
}

void Collider::ApplyCentralImpulse(btVector3 impulse)
{
    if (rigid_body_ == nullptr) {
        return;
    }

    rigid_body_->applyCentralImpulse(impulse);
}

btVector3 Collider::GetVelocity()
{
    if (rigid_body_ == nullptr) {
        return btVector3(0, 0, 0);
    }
    return rigid_body_->getLinearVelocity();
}

void Collider::SetVelocity(btVector3 velocity)
{
    if (rigid_body_ == nullptr) {
        return;
    }
    rigid_body_->setLinearVelocity(velocity);
}

btVector3 Collider::GetAngularVelocity()
{
    if (rigid_body_ == nullptr) {
        return btVector3(0, 0, 0);
    }
    return rigid_body_->getAngularVelocity();
}

void Collider::SetAngularVelocity(btVector3 velocity)
{
    if (rigid_body_ == nullptr) {
        return;
    }
    rigid_body_->setAngularVelocity(velocity);
}

}  // namespace bullet
}  // namespace legend
