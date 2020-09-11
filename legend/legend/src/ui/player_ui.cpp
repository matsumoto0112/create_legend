#include "src/ui/player_ui.h"

#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace ui {
namespace resource_name = util::resource::resource_names;

//�R���X�g���N�^
PlayerUI::PlayerUI() {
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  if (!player_sprite_.Init(
          resource.GetTexture().Get(
              resource_name::texture::UI_PLAYERFORM_PLAYER),
          directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID)) {
    MY_LOG(L"�v���C���[�摜�̏������Ɏ��s���܂����B");
  }
  player_sprite_.SetPosition(math::Vector2(125.0f, 130.0f));
  player_sprite_.SetScale(math::Vector2::kUnitVector);
  player_sprite_.SetRect(math::Rect(0, 0, 1, 1));
  player_sprite_.SetZOrder(0.5f);
}

//�f�X�g���N�^
PlayerUI::~PlayerUI() {}

//������
void PlayerUI::Init() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  directx::device::CommandList command_list;
  if (!command_list.Init(
          device, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    return;
  }

  if (!command_list.Close()) {
    return;
  }
  device.ExecuteCommandList({command_list});
  device.WaitExecute();
}

//�X�V
void PlayerUI::Update() {
  i32 i = 0;
  for (auto&& sprite : equipment_sprites_) {
    math::Vector2 pos = player_sprite_.GetPosition();
    float z_order;
    float width = sprite.GetContentSize().x;
    float height = sprite.GetContentSize().y;
    //�����X�L�����X�g���猈�܂����ʒu�ɉ摜��z�u
    if (i == 0) {
      pos +=
          (math::Vector2(0, -height) * 0.2f) - (math::Vector2(width, 0) * 0.1f);
      z_order = 0.43f;
    } else if (i == 1) {
      pos += (math::Vector2(-width, 0) * 0.25f) -
             (math::Vector2(0, height) * 0.05f);
      z_order = 0.55f;
    } else if (i == 2) {
      pos +=
          (math::Vector2(width, 0) * 0.25f) + (math::Vector2(0, height) * 0.1f);
      z_order = 0.45f;
    } else if (i == 3) {
      pos +=
          (math::Vector2(0, -height) * 0.4f) - (math::Vector2(width, 0) * 0.1f);
      z_order = 0.48f;

    } else {
      pos +=
          (math::Vector2(0, -height) * 0.3f) + (math::Vector2(width, 0) * 0.2f);
      z_order = 0.4f;
    }

    sprite.SetPosition(pos);
    sprite.SetZOrder(z_order);
    i++;
  }
}

//�`��
void PlayerUI::Draw() {
  draw::SpriteRenderer& sprite_renderer =
      game::GameDevice::GetInstance()->GetSpriteRenderer();
  sprite_renderer.AddDrawItems(&player_sprite_);

  for (auto&& sprite : equipment_sprites_) {
    sprite_renderer.AddDrawItems(&sprite);
  }
}

//�摜�X�V
void PlayerUI::AddEquipmentUI(const skill::Skill* skill) {
  std::shared_ptr<directx::buffer::Texture2D> tex =
      std::make_shared<directx::buffer::Texture2D>();
  tex = skill->GetEquipmentTexture();
  draw::Sprite2D sprite;
  if (!sprite.Init(
          tex,
          directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID)) {
    MY_LOG(L"�����p�e�N�X�`���̏������Ɏ��s���܂���");
  }

  sprite.SetScale(math::Vector2::kUnitVector);
  sprite.SetRect(math::Rect(0, 0, 1, 1));

  equipment_sprites_.push_back(sprite);
}

//�摜�폜
void PlayerUI::RemoveEquipmentUI(i32 remove_num) {
  equipment_sprites_.erase(equipment_sprites_.begin() + remove_num);
}
}  // namespace ui
}  // namespace legend