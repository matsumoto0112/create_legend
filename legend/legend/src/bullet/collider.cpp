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
    rigid_body_->applyCentralImpulse(impulse);
}

}  // namespace bullet
}  // namespace legend
