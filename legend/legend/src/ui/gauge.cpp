#include "src/ui/gauge.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace ui {

//�R���X�g���N�^
Gauge::Gauge() {}

//�f�X�g���N�^
Gauge::~Gauge() {}

//������
bool Gauge::Init(
    const std::wstring& name,
    directx::descriptor_heap::heap_parameter::LocalHeapID heap_id) {
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  if (!sprite_.Init(resource.GetTexture().Get(name), heap_id)) {
    return false;
  }

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  return constant_buffer_.Init(device, device.GetLocalHandle(heap_id),
                               name + L"_GaugeConstantBuffer");
}

//���W��ݒ肷��
void Gauge::SetPosition(const math::Vector2& position) {
  sprite_.SetPosition(position);
}

//���W���擾����
math::Vector2 Gauge::GetPosition() const { return sprite_.GetPosition(); }

// Z-Order��ݒ肷��
void Gauge::SetZOrder(float z) { sprite_.SetZOrder(z); }

// Z-Order���擾����
float Gauge::GetZOrder() const { return sprite_.GetZOrder(); }

//�`�惊�X�g�ɒǉ�
void Gauge::StackOwnItems(draw::SpriteRenderer& renderer) {
  //�`��O�ɌĂԊ֐���`
  //�ʏ�X�v���C�g�ł͂Ȃ��A�Q�[�W�p�}�e���A���ł̕`��ɂ��A�R���X�^���g�o�b�t�@���Z�b�g����
  auto pre_call_func = [&]() {
    auto& device = game::GameDevice::GetInstance()->GetDevice();
    auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
    auto& resource = game::GameDevice::GetInstance()->GetResource();
    resource.GetPipeline()
        .Get(util::resource::resource_names::pipeline::GAUGE)
        ->SetCommandList(command_list);
    constant_buffer_.GetStagingRef() = param_;
    constant_buffer_.UpdateStaging();
    constant_buffer_.RegisterHandle(
        device, directx::shader::ConstantBufferRegisterID::GAUGE_PARAMETER);
  };

  renderer.AddDrawItems(&sprite_, pre_call_func);
}

//�Q�[�W�̒l��ݒ肷��
void Gauge::SetValue(float value) { param_.value = value; }

//�Q�[�W�̒l���擾����
float Gauge::GetValue() const { return param_.value; }

}  // namespace ui
}  // namespace legend