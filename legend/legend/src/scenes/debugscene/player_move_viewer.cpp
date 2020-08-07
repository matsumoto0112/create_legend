#include "src/scenes/debugscene/player_move_viewer.h"

#include "src/directx/shader/alpha_blend_desc.h"
//#include "src/physics/collision.h"

namespace {
namespace ResourceID = legend::util::resource::id;
constexpr ResourceID::VertexShader MODEL_VIEW_VS =
    ResourceID::VertexShader::MODEL_VIEW;
constexpr ResourceID::PixelShader MODEL_VIEW_PS =
    ResourceID::PixelShader::MODEL_VIEW;
constexpr ResourceID::Pipeline MODEL_VIEW_MAT =
    ResourceID::Pipeline::MODEL_VIEW;
constexpr ResourceID::Model PLAYER_MODEL = ResourceID::Model::ERASER;
constexpr ResourceID::Model DESK_MODEL = ResourceID::Model::DESK;
}  // namespace

namespace legend {
namespace scenes {
namespace debugscene {
//コンストラクタ
PlayerMoveViewer::PlayerMoveViewer(ISceneChange* scene_change)
    : Scene(scene_change) {}

//デストラクタ
PlayerMoveViewer::~PlayerMoveViewer() {}

//初期化
bool PlayerMoveViewer::Initialize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  directx::device::CommandList command_list;
  if (!command_list.Init(device, D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    return false;
  }

  device.GetHeapManager().AddLocalHeap(
      device,
      directx::descriptor_heap::heap_parameter::LocalHeapID::PHYSICS_TEST);
  device.GetHeapManager().AddLocalHeap(
      device, directx::descriptor_heap::heap_parameter::LocalHeapID::
                  PLAYER_MOVE_VIEWER);

  //このシーンで使用するリソースを事前に読み込む
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  if (!resource.GetVertexShader().Load(
          MODEL_VIEW_VS, util::Path::GetInstance()->shader() / "modelview" /
                             "model_view_vs.cso")) {
    return false;
  }
  if (!resource.GetPixelShader().Load(
          MODEL_VIEW_PS, util::Path::GetInstance()->shader() / "modelview" /
                             "model_view_ps.cso")) {
    return false;
  }

  //モデルデータを読み込む
  const std::filesystem::path player_model_path =
      util::Path::GetInstance()->model() / "eraser_01.glb";
  if (!resource.GetModel().Load(util::resource::id::Model::ERASER,
                                player_model_path, command_list)) {
    return false;
  }
  const std::filesystem::path desk_model_path =
      util::Path::GetInstance()->model() / "desk.glb";
  if (!resource.GetModel().Load(util::resource::id::Model::DESK,
                                desk_model_path, command_list)) {
    return false;
  }

  auto gps = std::make_shared<directx::shader::GraphicsPipelineState>();
  directx::shader::GraphicsPipelineState::PSODesc pso_desc = {};
  pso_desc.BlendState.RenderTarget[0] =
      directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT;
  pso_desc.BlendState.AlphaToCoverageEnable = true;
  pso_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
  pso_desc.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
  pso_desc.InputLayout =
      resource.GetVertexShader().Get(MODEL_VIEW_VS)->GetInputLayout();
  pso_desc.NumRenderTargets = 1;
  pso_desc.PrimitiveTopologyType =
      D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  pso_desc.pRootSignature =
      device.GetDefaultRootSignature()->GetRootSignature();
  pso_desc.PS =
      resource.GetPixelShader().Get(MODEL_VIEW_PS)->GetShaderBytecode();
  pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
  pso_desc.RTVFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
  pso_desc.SampleDesc.Count = 1;
  pso_desc.SampleMask = UINT_MAX;
  pso_desc.VS =
      resource.GetVertexShader().Get(MODEL_VIEW_VS)->GetShaderBytecode();
  if (!gps->Init(device, pso_desc)) {
    return false;
  }
  if (!resource.GetPipeline().Register(MODEL_VIEW_MAT, gps)) {
    return false;
  }

  auto pp = std::make_shared<directx::shader::GraphicsPipelineState>();
  pso_desc.RasterizerState.FillMode =
      D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME;
  if (!pp->Init(device, pso_desc)) {
    return false;
  }
  if (!resource.GetPipeline().Register(
          util::resource::id::Pipeline::OBJECT_WIREFRAME, pp)) {
    return false;
  }

  //プレイヤーの初期化
  {
    player::Player::InitializeParameter player_parameter;
    player_parameter.transform =
        util::Transform(math::Vector3::kZeroVector, math::Quaternion::kIdentity,
                        math::Vector3::kUnitVector);
    player_parameter.bouding_box_length = math::Vector3(1.0f, 0.5f, 2.0f);
    player_parameter.min_power = 0;
    player_parameter.max_power = 1;
    if (!player_.Init(player_parameter)) {
      return false;
    }
  }

  //机の初期化
  {
    //本来はステージデータから読み込む
    object::Desk::InitializeParameter desk_parameter;
    desk_parameter.transform =
        util::Transform(math::Vector3::kZeroVector, math::Quaternion::kIdentity,
                        math::Vector3::kUnitVector);
    desk_parameter.bounding_box_length = math::Vector3(3.0f, 0.5f, 2.0f);
    desk_parameter.normal = math::Vector3::kUpVector;
    if (!desk_.Init(desk_parameter)) {
      return false;
    }
  }

  //カメラの初期化
  {
    const math::Vector3 camera_position = math::Vector3(0, 0.5f, -0.5f);
    const math::Quaternion camera_rotation =
        math::Quaternion::FromEular(math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f);
    const math::IntVector2 screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
    const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;
    if (!camera_.Init(L"MainCamera", camera_position, camera_rotation,
                      math::util::DEG_2_RAD * 50.0f, aspect_ratio)) {
      return false;
    }
  }

  command_list.Close();
  device.ExecuteCommandList({command_list});
  device.WaitExecute();
  return true;
}

//更新
bool PlayerMoveViewer::Update() {
  if (!player_.Update()) {
    return false;
  }

  if (ImGui::Begin("Camera")) {
    //カメラ座標
    math::Vector3 camera_position = camera_.GetPosition();
    ImGui::SliderFloat3("Position", &camera_position.x, -100.0f, 100.0f);
    camera_.SetPosition(camera_position);
    //カメラ回転角
    math::Vector3 camera_rotation =
        math::Quaternion::ToEular(camera_.GetRotation()) *
        math::util::RAD_2_DEG;
    ImGui::SliderFloat3("Rotation", &camera_rotation.x, -180.0f, 180.0f);
    camera_.SetRotation(
        math::Quaternion::FromEular(camera_rotation * math::util::DEG_2_RAD));

    //カメラの上方向ベクトルを変更する
    if (ImGui::Button("X_UP")) {
      camera_.SetUpVector(math::Vector3::kRightVector);
    }
    if (ImGui::Button("Y_UP")) {
      camera_.SetUpVector(math::Vector3::kUpVector);
    }
    if (ImGui::Button("Z_UP")) {
      camera_.SetUpVector(math::Vector3::kForwardVector);
    }
    float fov = camera_.GetFov() * math::util::RAD_2_DEG;
    ImGui::SliderFloat("FOV", &fov, 0.01f, 90.0f);
    camera_.SetFov(fov * math::util::DEG_2_RAD);
  }
  ImGui::End();

  if (ImGui::Begin("Player")) {
    math::Vector3 position = player_.GetPosition();
    math::Vector3 velocity = player_.GetVelocity();
    float impulse = player_.GetImpulse();
    ImGui::SliderFloat3("Velocity", &velocity.x, -1.0f, 1.0f);
    ImGui::SliderFloat("Impulse", &impulse, 0, 1.0f);
    ImGui::SliderFloat3("Position", &position.x, -100.0f, 100.0f);

    math::Vector3 rotation = math::Quaternion::ToEular(player_.GetRotation()) *
                             math::util::RAD_2_DEG;
    ImGui::SliderFloat3("Rotation", &rotation.x, -180.0f, 180.0f);
    player_.SetRotation(
        math::Quaternion::FromEular(rotation * math::util::DEG_2_RAD));
  }
  ImGui::End();

  // if (physics::Collision::GetInstance()->Collision_OBB_DeskOBB(
  //        player_.GetCollisionRef(), desk_.GetCollisionRef())) {
  //  MY_LOG(L"消しゴムと机が衝突しました");
  //  player_.SetPosition(player_.GetCollisionRef().GetPosition());
  //}

  return true;
}

//描画
void PlayerMoveViewer::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  device.GetHeapManager().SetGraphicsCommandList(command_list);

  game::GameDevice::GetInstance()
      ->GetResource()
      .GetPipeline()
      .Get(MODEL_VIEW_MAT)
      ->SetGraphicsCommandList(command_list);
  camera_.RenderStart();

  // player_.Draw();
  desk_.Draw();
}

//終了
void PlayerMoveViewer::Finalize() {
  util::resource::Resource& resource =
      game::GameDevice::GetInstance()->GetResource();
  resource.GetVertexShader().Unload(MODEL_VIEW_VS);
  resource.GetPixelShader().Unload(MODEL_VIEW_PS);
  resource.GetPipeline().Unload(MODEL_VIEW_MAT);
  resource.GetModel().Unload(PLAYER_MODEL);
  resource.GetModel().Unload(DESK_MODEL);
}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend