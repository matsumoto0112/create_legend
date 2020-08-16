#include "src/ui/quarter_gauge.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace ui {

//�R���X�g���N�^
QuarterGauge::QuarterGauge() : Gauge() {}

//�f�X�g���N�^
QuarterGauge::~QuarterGauge() {}

//������
bool QuarterGauge::Init(
    const std::wstring& name,
    directx::descriptor_heap::heap_parameter::LocalHeapID heap_id) {
  return Gauge::Init(name, heap_id);
}

//�`�惊�X�g�ɒǉ�
void QuarterGauge::StackOwnItems(draw::SpriteRenderer& renderer) {
  //�قƂ��Gauge�Ɠ��������A�g�p����V�F�[�_�[���Ⴄ

  //�`��O�ɌĂԊ֐���`
  //�ʏ�X�v���C�g�ł͂Ȃ��A�Q�[�W�p�}�e���A���ł̕`��ɂ��A�R���X�^���g�o�b�t�@���Z�b�g����
  auto pre_call_func = [&]() {
    auto& device = game::GameDevice::GetInstance()->GetDevice();
    auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
    auto& resource = game::GameDevice::GetInstance()->GetResource();
    resource.GetPipeline()
        .Get(util::resource::resource_names::pipeline::QUARTER_GAUGE)
        ->SetCommandList(command_list);
    constant_buffer_.GetStagingRef() = param_;
    constant_buffer_.UpdateStaging();
    constant_buffer_.RegisterHandle(
        device, directx::shader::ConstantBufferRegisterID::GAUGE_PARAMETER);
  };

  renderer.AddDrawItems(&sprite_, pre_call_func);
}

}  // namespace ui
}  // namespace legend