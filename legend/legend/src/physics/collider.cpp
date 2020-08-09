#include "src/physics/collider.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"

namespace legend {
namespace physics {

Collider::Collider(const util::Transform& transform, const std::wstring& name)
    : name_(name), transform_(transform) {}

Collider::~Collider() {}

bool Collider::Init() { return true; }

void Collider::Update() {}

}  // namespace physics
}  // namespace legend