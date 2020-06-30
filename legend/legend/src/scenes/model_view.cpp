#include "src/scenes/model_view.h"

#include "src/directx/vertex.h"
#include "src/game/game_device.h"
#include "src/libs/stb_image.h"
#include "src/util/loader/glb_loader.h"
#include "src/util/path.h"

namespace legend {
namespace scenes {

//�R���X�g���N�^
ModelView::ModelView(ISceneChange* scene_change) : Scene(scene_change) {}

//������
void ModelView::Initialize() {
  util::loader::GLBLoader loader;
  std::filesystem::path model_path =
      util::Path::GetInstance()->exe() / L"assets" / L"maru_UV_TexUmekomi.glb";
  util::loader::LoadedMeshData mesh_data = loader.Load(model_path);

  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  std::vector<directx::Vertex> vertices(mesh_data.vertex_num);
  //���_���W��(x,y,z)�ł���K�v������
  if (mesh_data.position_size == 3) {
    for (u32 i = 0; i < mesh_data.vertex_num; i++) {
      vertices[i].position.x = mesh_data.positions[i * 3 + 0];
      vertices[i].position.y = mesh_data.positions[i * 3 + 1];
      vertices[i].position.z = mesh_data.positions[i * 3 + 2];
    }
  } else {
    MY_LOG(L"���_���W�̊i�[�Ɏ��s���܂����B���\�[�X����%s�ł�", mesh_data.name);
  }

  //�@������(x,y,z)�ł���K�v������
  if (mesh_data.normal_size == 3) {
    for (u32 i = 0; i < mesh_data.vertex_num; i++) {
      vertices[i].normal.x = mesh_data.normals[i * 3 + 0];
      vertices[i].normal.y = mesh_data.normals[i * 3 + 1];
      vertices[i].normal.z = mesh_data.normals[i * 3 + 2];
    }
  } else {
    MY_LOG(L"�@�����̊i�[�Ɏ��s���܂����B���\�[�X����%s�ł�", mesh_data.name);
  }

  // UV����(u,v)�ł���K�v������
  if (mesh_data.uv_size == 2) {
    for (u32 i = 0; i < mesh_data.vertex_num; i++) {
      vertices[i].uv.x = mesh_data.uvs[i * 2 + 0];
      vertices[i].uv.y = mesh_data.uvs[i * 2 + 1];
    }
  } else {
    MY_LOG(L"UV���̊i�[�Ɏ��s���܂����B���\�[�X����%s�ł�", mesh_data.name);
  }

  //���_�o�b�t�@�쐬
  if (!vertex_buffer_.Init(device, sizeof(directx::Vertex),
                           mesh_data.vertex_num,
                           mesh_data.name + L"_VertexBuffer")) {
    return;
  }
  if (!vertex_buffer_.WriteBufferResource(vertices)) {
    return;
  }

  //�C���f�b�N�X�o�b�t�@�쐬
  if (!index_buffer_.Init(device, static_cast<u32>(mesh_data.indices.size()),
                          directx::PrimitiveTopology::TriangleList,
                          mesh_data.name + L"_IndexBuffer")) {
    return;
  }
  if (!index_buffer_.WriteBufferResource(mesh_data.indices)) {
    return;
  }

  //���[�g�V�O�l�`���쐬
  root_signature_ = std::make_shared<directx::shader::RootSignature>();
  if (!root_signature_->Init(game::GameDevice::GetInstance()->GetDevice(),
                             L"Global Root Signature")) {
    return;
  }

  //���_�V�F�[�_�[
  std::filesystem::path path = util::Path::GetInstance()->shader();
  std::filesystem::path vertex_shader_path =
      path / L"modelview" / L"model_view_vs.cso";
  std::filesystem::path pixel_shader_path =
      path / L"modelview" / L"model_view_ps.cso";
  std::vector<D3D12_INPUT_ELEMENT_DESC> elements{
      {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0},
      {"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0},
      {"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0},
      {"TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0}};

  //���_�V�F�[�_�[
  std::shared_ptr<directx::shader::VertexShader> vertex_shader =
      std::make_shared<directx::shader::VertexShader>();
  if (!vertex_shader->Init(game::GameDevice::GetInstance()->GetDevice(),
                           vertex_shader_path, elements)) {
    return;
  }

  //�s�N�Z���V�F�[�_�[
  std::shared_ptr<directx::shader::PixelShader> pixel_shader =
      std::make_shared<directx::shader::PixelShader>();
  if (!pixel_shader->Init(game::GameDevice::GetInstance()->GetDevice(),
                          pixel_shader_path)) {
    return;
  }

  if (!pipeline_state_.Init(game::GameDevice::GetInstance()->GetDevice())) {
    return;
  }

  //�p�C�v���C���쐬�J�n
  pipeline_state_.SetRootSignature(root_signature_);
  pipeline_state_.SetVertexShader(vertex_shader);
  pipeline_state_.SetPixelShader(pixel_shader);

  if (!pipeline_state_.CreatePipelineState(
          game::GameDevice::GetInstance()->GetDevice())) {
    return;
  }

  if (!transform_cb_.Init(device, 0, L"Transform ConstantBuffer")) {
    return;
  }
  rotation_ = math::Vector3::kZeroVector;
  scale_ = math::Vector3::kUnitVector * 15.0f;
  transform_cb_.GetStagingRef().world =
      math::Matrix4x4::CreateScale(scale_) *
      math::Matrix4x4::CreateRotation(rotation_);
  transform_cb_.UpdateStaging();

  if (!world_cb_.Init(device, 1, L"WorldContext ConstantBuffer")) {
    return;
  }

  world_cb_.GetStagingRef().view = math::Matrix4x4::CreateView(
      math::Vector3(0, 10, -10), math::Vector3(0, 0, 0),
      math::Vector3::kUpVector);
  const float aspect = 1280.0f / 720.0f;
  world_cb_.GetStagingRef().projection =
      math::Matrix4x4::CreateProjection(45.0f, aspect, 0.1f, 100.0);
  world_cb_.UpdateStaging();

  int w, h, comp;
  const u32 size = static_cast<u32>(mesh_data.image.size());
  u8* begin =
      stbi_load_from_memory(mesh_data.image.data(), size, &w, &h, &comp, 4);
  std::vector<u8> image(begin, begin + w * h * 4);
  if (!texture_.Init(device, 0, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, w, h,
                     L"Image")) {
    return;
  }
  texture_.WriteResource(device, image.data());
}

//�X�V
void ModelView::Update() {
  rotation_.y += 0.01f;
  rotation_.z += 0.005f;
  scale_ = math::Vector3::kUnitVector * 15.0f;
  transform_cb_.GetStagingRef().world =
      math::Matrix4x4::CreateScale(scale_) *
      math::Matrix4x4::CreateRotation(rotation_);
  transform_cb_.UpdateStaging();
}

//�`��
void ModelView::Draw() {
  Scene::Draw();

  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();
  root_signature_->SetGraphicsCommandList(device);
  pipeline_state_.SetGraphicsCommandList(device);
  device.GetHeapManager().SetGraphicsCommandList(device);
  transform_cb_.SetToHeap(device);
  world_cb_.SetToHeap(device);
  texture_.SetToHeap(device);
  device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);

  vertex_buffer_.SetGraphicsCommandList(device);
  index_buffer_.SetGraphicsCommandList(device);
  index_buffer_.Draw(device);
}
}  // namespace scenes
}  // namespace legend
