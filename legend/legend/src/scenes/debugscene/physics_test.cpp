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
PhysicsTest::~PhysicsTest() {}

//初期化
bool PhysicsTest::Initialize() {
  obbs.resize(obb_num);
  obbs[0].SetPosition(math::Vector3(0, 0, 0));
  obbs[1].SetPosition(math::Vector3(0, 20, 0));

  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  const std::wstring name = L"1000cmObject";
  const std::filesystem::path model_path =
      util::Path::GetInstance()->model() / (name + L".glb");
  util::loader::GLBLoader loader;
  if (!loader.Load(model_path)) {
    MY_LOG(L"モデルの読み込みに失敗しました。対象のファイルは%sです。",
           model_path.generic_wstring().c_str());
    return false;
  }

  for (i32 i = 0; i < obb_num; i++) {
    if (!obbs[i].Initialize(device, name, loader)) {
      continue;
    }
  }

  if (!world_constant_buffer_.Init(device, 1, L"WorldContext ConstantBuffer")) {
    return false;
  }

  world_constant_buffer_.GetStagingRef().view = math::Matrix4x4::CreateView(
      math::Vector3(0, 1, -1), math::Vector3(0, 0, 0),
      math::Vector3::kUpVector);
  const float aspect = 1280.0f / 720.0f;
  world_constant_buffer_.GetStagingRef().projection =
      math::Matrix4x4::CreateProjection(45.0f, aspect, 0.1f, 100.0);
  world_constant_buffer_.UpdateStaging();

  //メインテクスチャの書き込み
  const std::vector<u8> albedo = loader.GetAlbedo();
  if (!texture_.InitAndWrite(device, 0, albedo, name + L"_Albedo")) {
    return false;
  }

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
  pipeline_state_.SetRenderTargetInfo(device.GetRenderTarget(), true);
  pipeline_state_.SetBlendDesc(
      directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT, 0);

  if (!pipeline_state_.CreatePipelineState(
          game::GameDevice::GetInstance()->GetDevice())) {
    return false;
  }

  return true;
}

//更新
bool PhysicsTest::Update() {
  if (ImGui::Begin("Position")) {
    //直方体1
    math::Vector3 obb1_position = obbs[0].GetPosition();
    ImGui::SliderFloat3("OBB1_Position", &obb1_position.x, -100.0f, 100.0f);
    obbs[0].SetPosition(obb1_position);

    //直方体2
    math::Vector3 obb2_position = obbs[1].GetPosition();
    ImGui::SliderFloat3("OBB2_Position", &obb2_position.x, -100.0f, 100.0f);
    obbs[1].SetPosition(obb2_position);

    for (i32 i = 0; i < obb_num; i++) {
      obbs[i].Update();
    }
  }
  ImGui::End();

  if (!physics::Collision::GetInstance()->Collision_OBB_OBB(obbs[0], obbs[1])) {
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
  device.GetHeapManager().SetGraphicsCommandList(device);
  texture_.SetToHeap(device);
  world_constant_buffer_.SetToHeap(device);

  for (i32 i = 0; i < obb_num; i++) {
    obbs[i].Draw(device);
  }
}

//終了
void PhysicsTest::Finalize() {
  game::GameDevice::GetInstance()->GetDevice().WaitForGPU();
}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend