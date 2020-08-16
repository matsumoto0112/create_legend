#include "src/ui/image.h"

#include "src/game/game_device.h"

namespace legend {
namespace ui {

//�R���X�g���N�^
Image::Image() {}

//�f�X�g���N�^
Image::~Image() {}

//������
bool Image::Init(
    const std::wstring& name,
    directx::descriptor_heap::heap_parameter::LocalHeapID heap_id) {
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  return sprite_.Init(resource.GetTexture().Get(name), heap_id);
}

//�`�惊�X�g�ɒǉ�
void Image::StackOwnItems(draw::SpriteRenderer& renderer) {
  renderer.AddDrawItems(&sprite_);
}

//���W��ݒ肷��
void Image::SetPosition(const math::Vector2& position) {
  sprite_.SetPosition(position);
}

//���W���擾����
math::Vector2 Image::GetPosition() const { return sprite_.GetPosition(); }

// Z-Order��ݒ肷��
void Image::SetZOrder(float z) { sprite_.SetZOrder(z); }

// Z-Order���擾����
float Image::GetZOrder() const { return sprite_.GetZOrder(); }

}  // namespace ui
}  // namespace legend