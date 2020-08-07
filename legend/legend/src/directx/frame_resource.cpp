#include "src/directx/frame_resource.h"

namespace legend {
namespace directx {

//�R���X�g���N�^
FrameResource::FrameResource() {}

//�f�X�g���N�^
FrameResource::~FrameResource() {}

//������
bool FrameResource::Init(device::IDirectXAccessor& accessor) {
  if (!command_lists_.Init(
          accessor, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    return false;
  }

  if (!command_lists_.Close()) {
    return false;
  }

  return true;
}

//���\�[�X����
bool FrameResource::Ready() {
  if (!command_lists_.Reset()) {
    return false;
  }

  return true;
}

//�j��
void FrameResource::Destroy() {}

}  // namespace directx
}  // namespace legend
