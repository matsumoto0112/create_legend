#include "src/physics/plane.h"

#include "src/directx/vertex.h"

namespace legend {
namespace physics {
//�R���X�g���N�^
Plane::Plane()
    : position_(math::Vector3::kZeroVector), normal_(math::Vector3(0, 1, 0)) {}

//������
bool Plane::Initialize(directx::DirectX12Device& device) {
  float front = GetPosition().z - 1;
  float back = GetPosition().z + 1;
  float left = GetPosition().x - 1;
  float right = GetPosition().x + 1;

  const std::vector<directx::BoundingBox> vertices{
      {{left, 0, front}},
      {{right, 0, front}},
      {{right, 0, back}},
      {{left, 0, back}},
  };

  //���_�o�b�t�@�쐬
  if (!vertex_buffer_.Init(device, sizeof(directx::BoundingBox),
                           static_cast<u32>(vertices.size()),
                           L"Plane_VertexBuffer")) {
    return false;
  }
  if (!vertex_buffer_.WriteBufferResource(vertices)) {
    return false;
  }

  const std::vector<u16> indices{0, 1, 1, 2, 2, 3, 3, 0};
  //�C���f�b�N�X�o�b�t�@�쐬
  if (!index_buffer_.InitAndWrite(device, indices,
                                  directx::PrimitiveTopology::LineList,
                                  L"Plane_IndexBuffer")) {
    return false;
  }

  if (!transform_constant_buffer_.Init(
          device, 0,
          device.GetLocalHeapHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
          L"Transform ConstantBuffer")) {
    return false;
  }

  math::Vector3 position = math::Vector3::kZeroVector;
  transform_constant_buffer_.GetStagingRef().world =
      math::Matrix4x4::CreateTranslate(position);
  transform_constant_buffer_.UpdateStaging();

  return true;
}

//�`��
void Plane::Draw(directx::DirectX12Device& device) {
  transform_constant_buffer_.SetToHeap(device);
  device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);
  vertex_buffer_.SetGraphicsCommandList(device);
  index_buffer_.SetGraphicsCommandList(device);
  index_buffer_.Draw(device);
}

//�ʒu�̐ݒ�
void Plane::SetPosition(math::Vector3 position) { position_ = position; }

//�@���x�N�g���̐ݒ�
void Plane::SetNormal(math::Vector3 normal) { normal_ = normal; }

//�ʒu�̎擾
math::Vector3 Plane::GetPosition() { return position_; }

//�@���x�N�g���̎擾
math::Vector3 Plane::GetNormal() { return normal_; }
}  // namespace physics
}  // namespace legend