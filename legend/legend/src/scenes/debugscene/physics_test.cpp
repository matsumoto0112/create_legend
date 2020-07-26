#include "src/scenes/debugscene/physics_test.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/physics/collision.h"
#include "src/util/path.h"

namespace legend {
namespace scenes {
namespace debugscene {

//コンストラクタ
PhysicsTest::PhysicsTest(ISceneChange* scene_change) : Scene(scene_change) {}

//デストラクタ
PhysicsTest::~PhysicsTest() {
  game::GameDevice::GetInstance()->GetDevice().WaitForGPU();
}

//初期化
bool PhysicsTest::Initialize() {
  obbs_.resize(obb_num_);
  obbs_[0].SetLength(2, 1, 2);
  obbs_[1].SetLength(1, 2, 1);

  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  for (i32 i = 0; i < obb_num_; i++) {
    if (!obbs_[i].Initialize(device)) {
      return false;
    }
  }

  if (!plane_.Initialize(device)) {
    return false;
  }

  if (!world_constant_buffer_.Init(
          device, 1,
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

  //ルートシグネチャ作成
  root_signature_ = std::make_shared<directx::shader::RootSignature>();
  if (!root_signature_->Init(game::GameDevice::GetInstance()->GetDevice(),
                             L"Global Root Signature")) {
    return false;
  }

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

  //頂点シェーダー
  std::shared_ptr<directx::shader::VertexShader> vertex_shader =
      std::make_shared<directx::shader::VertexShader>();
  if (!vertex_shader->Init(game::GameDevice::GetInstance()->GetDevice(),
                           vertex_shader_path, elements)) {
    return false;
  }

  //ピクセルシェーダー
  std::shared_ptr<directx::shader::PixelShader> pixel_shader =
      std::make_shared<directx::shader::PixelShader>();
  if (!pixel_shader->Init(game::GameDevice::GetInstance()->GetDevice(),
                          pixel_shader_path)) {
    return false;
  }

  if (!pipeline_state_.Init(game::GameDevice::GetInstance()->GetDevice())) {
    return false;
  }

  //パイプライン作成開始
  pipeline_state_.SetRootSignature(root_signature_);
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

//更新
bool PhysicsTest::Update() {
  if (ImGui::Begin("Transform")) {
    //直方体1
    math::Vector3 obb1_position = obbs_[0].GetPosition();
    ImGui::SliderFloat3("OBB1_Position", &obb1_position.x, -180.0f, 180.0f);
    math::Vector3 obb1_rotation = obbs_[0].GetRotation();
    ImGui::SliderFloat3("OBB1_Rotation", &obb1_rotation.x, -180.0f, 180.0f);
    math::Vector3 obb1_scale = obbs_[0].GetScale();
    ImGui::SliderFloat3("OBB1_Scale", &obb1_scale.x, 0.1f, 2.0f);
    if (ImGui::Button("Plus_Y")) {
      obb1_position.y += 0.1f;
    }
    if (ImGui::Button("Minus_Y")) {
      obb1_position.y -= 0.1f;
    }
    obbs_[0].SetPosition(obb1_position);
    obbs_[0].SetRotation(obb1_rotation);
    obbs_[0].SetScale(obb1_scale);

    //直方体2
    math::Vector3 obb2_position = obbs_[1].GetPosition();
    ImGui::SliderFloat3("OBB2_Position", &obb2_position.x, -180.0f, 180.0f);
    math::Vector3 obb2_rotation = obbs_[1].GetRotation();
    ImGui::SliderFloat3("OBB2_Rotation", &obb2_rotation.x, -180.0f, 180.0f);
    math::Vector3 obb2_scale = obbs_[1].GetScale();
    ImGui::SliderFloat3("OBB2_Scale", &obb2_scale.x, 0.1f, 2.0f);
    obbs_[1].SetPosition(obb2_position);
    obbs_[1].SetRotation(obb2_rotation);
    obbs_[1].SetScale(obb2_scale);
  }
  ImGui::End();
  for (i32 i = 0; i < obb_num_; i++) {
    obbs_[i].Update();
  }

  if (physics::Collision::GetInstance()->Collision_OBB_OBB(obbs_[0],
                                                           obbs_[1])) {
    MY_LOG(L"直方体1と直方体2が衝突しました");
  }

  if (physics::Collision::GetInstance()->Collision_OBB_Plane(obbs_[0],
                                                             plane_)) {
    MY_LOG(L"直方体1と平面が衝突しました");
  }

  return true;
}

//描画
void PhysicsTest::Draw() {
  Scene::Draw();

  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  root_signature_->SetGraphicsCommandList(device);
  pipeline_state_.SetGraphicsCommandList(device);
  world_constant_buffer_.SetToHeap(device);

  for (i32 i = 0; i < obb_num_; i++) {
    obbs_[i].Draw(device);
  }

  plane_.Draw(device);
}

//終了
void PhysicsTest::Finalize() {
  game::GameDevice::GetInstance()->GetDevice().WaitForGPU();
}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend