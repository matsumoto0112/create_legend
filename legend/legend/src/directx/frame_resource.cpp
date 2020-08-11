#include "src/directx/frame_resource.h"

namespace legend {
namespace directx {

//�R���X�g���N�^
FrameResource::FrameResource() {}

//�f�X�g���N�^
FrameResource::~FrameResource() {}

//������
bool FrameResource::Init(device::IDirectXAccessor& accessor,
                         D3D12_COMMAND_LIST_TYPE command_list_type) {
  if (!command_lists_.Init(accessor, command_list_type)) {
    return false;
  }

  if (!command_lists_.Close()) {
    return false;
  }
  fence_value_ = 0;
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
