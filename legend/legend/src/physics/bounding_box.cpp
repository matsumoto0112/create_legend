#include "src/physics/bounding_box.h"

#include "src/directx/vertex.h"

namespace legend {
namespace physics {

//�R���X�g���N�^
BoundingBox::BoundingBox()
    : position_(math::Vector3::kZeroVector), directions_(3), lengthes_(3) {
  directions_[0] = math::Vector3::kRightVector;
  directions_[1] = math::Vector3::kUpVector;
  directions_[2] = math::Vector3::kForwardVector;

  lengthes_[0] = 1.0f;
  lengthes_[1] = 1.0f;
  lengthes_[2] = 1.0f;
}

bool BoundingBox::Initialize(directx::DirectX12Device& device,
                             const std::wstring name,
                             util::loader::GLBLoader& loader) {
  const u32 vertex_num = loader.GetVertexNum();
  std::vector<directx::Vertex> vertices(vertex_num);

  //���_���W
  {
    const std::vector<float> position_list = loader.GetPosition();
    const u32 position_component_size = loader.GetPositionComponentSize();

    if (position_component_size == 3) {
      for (u32 i = 0; i < vertex_num; i++) {
        vertices[i].position.x = position_list[i * position_component_size + 0];
        vertices[i].position.y = position_list[i * position_component_size + 1];
        vertices[i].position.z = position_list[i * position_component_size + 2];
      }
    } else {
      MY_LOG(L"���_���W���̊i�[�Ɏ��s���܂����B���\�[�X����%s�ł�",
             name.c_str());
    }
  }

  //�@��
  {
    const std::vector<float> normal_list = loader.GetNormal();
    const u32 normal_component_size = loader.GetNormalComponentSize();
    if (normal_component_size == 3) {
      for (u32 i = 0; i < vertex_num; i++) {
        vertices[i].normal.x = normal_list[i * normal_component_size + 0];
        vertices[i].normal.y = normal_list[i * normal_component_size + 1];
        vertices[i].normal.z = normal_list[i * normal_component_size + 2];
      }
    } else {
      MY_LOG(L"�@�����̊i�[�Ɏ��s���܂����B���\�[�X����%s�ł�", name.c_str());
    }
  }

  //���_�o�b�t�@�쐬
  if (!vertex_buffer_.Init(device, sizeof(directx::Vertex), vertex_num,
                           name + L"_VertexBuffer")) {
    return false;
  }
  if (!vertex_buffer_.WriteBufferResource(vertices)) {
    return false;
  }

  const std::vector<u16> index = loader.GetIndex();
  //�C���f�b�N�X�o�b�t�@�쐬
  if (!index_buffer_.InitAndWrite(device, index,
                                  directx::PrimitiveTopology::LineList,
                                  name + L"_IndexBuffer")) {
    return false;
  }

  if (!transform_constant_buffer_.Init(device, 0,
                                       L"Transform ConstantBuffer")) {
    return false;
  }

  math::Vector3 position = math::Vector3::kZeroVector;
  math::Vector3 scale = math::Vector3::kUnitVector * 1.0f;
  transform_constant_buffer_.GetStagingRef().world =
      math::Matrix4x4::CreateScale(scale) *
      math::Matrix4x4::CreateTranslate(position);
  transform_constant_buffer_.UpdateStaging();

  return true;
}

void BoundingBox::Update() {
  math::Vector3 position = this->GetPosition();
  math::Vector3 scale = math::Vector3::kUnitVector * 1.0f;
  transform_constant_buffer_.GetStagingRef().world =
      math::Matrix4x4::CreateScale(scale) *
      math::Matrix4x4::CreateTranslate(position);
  transform_constant_buffer_.UpdateStaging();
}

void BoundingBox::Draw(directx::DirectX12Device& device) {
  transform_constant_buffer_.SetToHeap(device);
  device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);
  vertex_buffer_.SetGraphicsCommandList(device);
  index_buffer_.SetGraphicsCommandList(device);
  index_buffer_.Draw(device);
}

//�����x�N�g�����擾
math::Vector3 BoundingBox::GetDirection(i32 direction_num) {
  return directions_[direction_num];
}

//�������擾
float BoundingBox::GetLength(i32 length_num) { return lengthes_[length_num]; }

//���݂̈ʒu���擾
math::Vector3 BoundingBox::GetPosition() { return position_; }

//�e�����x�N�g���̐ݒ�
void BoundingBox::SetDirection(math::Vector3 direction_x,
                               math::Vector3 direction_y,
                               math::Vector3 direction_z) {
  directions_[0] = direction_x;
  directions_[1] = direction_y;
  directions_[2] = direction_z;
}

//�e�����̒����̐ݒ�
void BoundingBox::SetLength(float length_x, float length_y, float length_z) {
  lengthes_[0] = length_x;
  lengthes_[1] = length_y;
  lengthes_[2] = length_z;
}

//���S���W�̍X�V
void BoundingBox::SetPosition(math::Vector3 position) { position_ = position; }

}  // namespace physics
}  // namespace legend