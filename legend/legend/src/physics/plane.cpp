#include "src/physics/plane.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"

namespace legend {
namespace physics {
//�R���X�g���N�^
Plane::Plane()
    : position_(math::Vector3::kZeroVector), normal_(math::Vector3::kUpVector) {
  right_ = (normal_ == math::Vector3::kRightVector ||
            normal_ == math::Vector3::kLeftVector)
               ? 0
               : position_.x + 1;
  left_ = (normal_ == math::Vector3::kRightVector ||
           normal_ == math::Vector3::kLeftVector)
              ? 0
              : position_.x - 1;
  forward_ = (normal_ == math::Vector3::kForwardVector ||
              normal_ == math::Vector3::kBackwardVector)
                 ? 0
                 : position_.z - 1;
  back_ = (normal_ == math::Vector3::kForwardVector ||
           normal_ == math::Vector3::kBackwardVector)
              ? 0
              : position_.z + 1;
  up_ = (normal_ == math::Vector3::kUpVector ||
         normal_ == math::Vector3::kDownVector)
            ? 0
            : position_.y + 1;
  down_ = (normal_ == math::Vector3::kUpVector ||
           normal_ == math::Vector3::kDownVector)
              ? 0
              : position_.y - 1;
}

Plane::Plane(math::Vector3 position, math::Vector3 normal)
    : position_(position), normal_(normal) {
  right_ = (normal_ == math::Vector3::kRightVector ||
            normal_ == math::Vector3::kLeftVector)
               ? 0
               : position_.x + 1;
  left_ = (normal_ == math::Vector3::kRightVector ||
           normal_ == math::Vector3::kLeftVector)
              ? 0
              : position_.x - 1;
  forward_ = (normal_ == math::Vector3::kForwardVector ||
              normal_ == math::Vector3::kBackwardVector)
                 ? 0
                 : position_.z - 1;
  back_ = (normal_ == math::Vector3::kForwardVector ||
           normal_ == math::Vector3::kBackwardVector)
              ? 0
              : position_.z + 1;
  up_ = (normal_ == math::Vector3::kUpVector ||
         normal_ == math::Vector3::kDownVector)
            ? 0
            : position_.y + 1;
  down_ = (normal_ == math::Vector3::kUpVector ||
           normal_ == math::Vector3::kDownVector)
              ? 0
              : position_.y - 1;
}

Plane::~Plane() {}

//������
bool Plane::Initialize(directx::DirectX12Device& device) {
  const std::vector<directx::PhysicsVertex> vertices{
      {{left_, down_, forward_}},
      {{right_, down_, forward_}},
      {{right_, up_, back_}},
      {{left_, up_, back_}},
  };

  //���_�o�b�t�@�쐬
  if (!vertex_buffer_.Init(device, sizeof(directx::PhysicsVertex),
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
          device, directx::shader::ConstantBufferRegisterID::Transform,
          device.GetLocalHeapHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
          L"Transform ConstantBuffer")) {
    return false;
  }

  math::Vector3 position = GetPosition();
  transform_constant_buffer_.GetStagingRef().world =
      math::Matrix4x4::CreateTranslate(position);
  transform_constant_buffer_.UpdateStaging();

  if (!world_constant_buffer_.Init(
          device, directx::shader::ConstantBufferRegisterID::WorldContext,
          device.GetLocalHeapHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
          L"WorldContext ConstantBuffer")) {
    return false;
  }

  world_constant_buffer_.GetStagingRef().view = math::Matrix4x4::CreateView(
      math::Vector3(0, 1, -1), math::Vector3(0, 0, 0),
      math::Vector3::kUpVector);
  const float aspect = 1280.0f / 720.0f;
  world_constant_buffer_.GetStagingRef().projection =
      math::Matrix4x4::CreateProjection(45.0f, aspect, 0.1f, 100.0);
  world_constant_buffer_.UpdateStaging();

  std::filesystem::path path = util::Path::GetInstance()->shader();
  std::filesystem::path vertex_shader_path = path / L"physics" / L"obb_vs.cso";
  std::filesystem::path pixel_shader_path = path / L"physics" / L"obb_ps.cso";
  std::vector<D3D12_INPUT_ELEMENT_DESC> elements{
      {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0},
      {"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0}};

  //���_�V�F�[�_�[
  std::shared_ptr<directx::shader::VertexShader> vertex_shader =
      std::make_shared<directx::shader::VertexShader>();
  if (!vertex_shader->Init(game::GameDevice::GetInstance()->GetDevice(),
                           vertex_shader_path, elements)) {
    return false;
  }

  //�s�N�Z���V�F�[�_�[
  std::shared_ptr<directx::shader::PixelShader> pixel_shader =
      std::make_shared<directx::shader::PixelShader>();
  if (!pixel_shader->Init(game::GameDevice::GetInstance()->GetDevice(),
                          pixel_shader_path)) {
    return false;
  }

  if (!pipeline_state_.Init(game::GameDevice::GetInstance()->GetDevice())) {
    return false;
  }

  //�p�C�v���C���쐬�J�n
  pipeline_state_.SetRootSignature(device.GetDefaultRootSignature());
  pipeline_state_.SetVertexShader(vertex_shader);
  pipeline_state_.SetPixelShader(pixel_shader);
  device.GetRenderResourceManager().WriteRenderTargetInfoToPipeline(
      device, directx::render_target::RenderTargetID::BACK_BUFFER,
      &pipeline_state_);
  pipeline_state_.SetBlendDesc(
      directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT, 0);
  pipeline_state_.SetPrimitiveTopology(
      D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);

  if (!pipeline_state_.CreatePipelineState(
          game::GameDevice::GetInstance()->GetDevice())) {
    return false;
  }

  return true;
}

//�`��
void Plane::Draw(directx::DirectX12Device& device) {
  pipeline_state_.SetGraphicsCommandList(device);
  world_constant_buffer_.SetToHeap(device);
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
math::Vector3 Plane::GetPosition() const { return position_; }

//�@���x�N�g���̎擾
math::Vector3 Plane::GetNormal() const { return normal_; }

//�������擾
float Plane::GetDistance(math::Vector3 position) const {
  return math::util::Abs(math::Vector3::Dot(position, GetNormal()));
}
}  // namespace physics
}  // namespace legend