#include "src/game/game_device.h"

#include <btBulletDynamicsCommon.h>

#include "src/directx/bullet_debug_draw.h"
#include "src/directx/device/device_option.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/window/window_procedure.h"

namespace {
constexpr legend::u32 WINDOW_WIDTH = 1280;
constexpr legend::u32 WINDOW_HEIGHT = 720;

#pragma region bullet描画テスト用変数定義
//std::shared_ptr<btCollisionConfiguration> config_;
//std::shared_ptr<btCollisionDispatcher> dispatcher_;
//std::shared_ptr<btDbvtBroadphase> broadphase_;
//std::shared_ptr<btSequentialImpulseConstraintSolver> solver_;
//std::shared_ptr<btDynamicsWorld> world_;
//std::shared_ptr<legend::directx::BulletDebugDraw> debug_drawer_;
//std::shared_ptr<btCollisionShape> ground_shape_;
//std::shared_ptr<btDefaultMotionState> motion_;
//std::shared_ptr<btRigidBody> body_;
#pragma endregion
}  // namespace

namespace legend {
namespace game {
GameDevice::GameDevice() : fps_counter_{} {}

GameDevice::~GameDevice() {}

bool GameDevice::Init(window::IWindowProcedureEventCallback* callback) {
  //作るオブジェクトは
  window_ = std::make_unique<window::Window>();
  //ウィンドウ
  window_->SetScreenSize(math::IntVector2(WINDOW_WIDTH, WINDOW_HEIGHT));
  window_->SetWindowTitle(L"Game");
  window_->SetWindowProc(window::procedure::WindowProcdures);
  window_->SetWindowProcCallBack(callback);
  window_->Create();
  // DirectX12デバイス

  device_ = std::make_unique<directx::device::DirectXDevice>();
  if (!device_->Init(WINDOW_WIDTH, WINDOW_HEIGHT, window_->GetHWND())) {
    return false;
  }

  //入力
  input_manager_ = std::make_unique<input::InputManager>(window_->GetHWND());
  random_ = std::make_unique<util::Random>();

  //サウンド
  audio_manager = std::make_unique<audio::AudioManager>();
  if (!audio_manager->Init()) {
    return false;
  }

  //スプライト描画
  const math::Vector2 screen_size =
      math::Vector2(static_cast<float>(window_->GetScreenSize().x),
                    static_cast<float>(window_->GetScreenSize().y));
  if (!sprite_renderer_.Init(screen_size)) {
    return false;
  }

  //リソース管理
  if (!resource_.Init()) {
    return false;
  }

  if (!particle_command_list_.Init(GetDevice(),
                                   directx::device::defines::FRAME_COUNT)) {
    return false;
  }

  // ImGui
  if (!imgui_manager_.Init(window_->GetHWND(), device_->GetDevice(),
                           DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
                           directx::device::defines::FRAME_COUNT, true)) {
    return false;
  }

  if (!global_cb_.Init(
          GetDevice(),
          GetDevice().GetLocalHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
          L"Global_ConstantBuffer")) {
    return false;
  }
  global_cb_.GetStagingRef().time = 0.0f;
  global_cb_.GetStagingRef().delta_time = 0.0f;

#pragma region bullet描画テスト用データ作成
  // config_ = std::make_shared<btDefaultCollisionConfiguration>();
  // dispatcher_ = std::make_shared<btCollisionDispatcher>(config_.get());
  // broadphase_ = std::make_shared<btDbvtBroadphase>();
  // solver_ = std::make_shared<btSequentialImpulseConstraintSolver>();
  // world_ = std::make_shared<btDiscreteDynamicsWorld>(
  //    dispatcher_.get(), broadphase_.get(), solver_.get(), config_.get());
  // world_->setDebugDrawer(nullptr);
  // world_->setGravity(btVector3(0.0f, -10.0f, 0.0f));

  // debug_drawer_ = std::make_shared<directx::BulletDebugDraw>();
  // if (!debug_drawer_->Init(device_->GetDevice())) {
  //  return false;
  //}
  // debug_drawer_->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
  // world_->setDebugDrawer(debug_drawer_.get());

  // ground_shape_ =
  // std::make_shared<btBoxShape>(btVector3(50.0f, 1.0f, 50.0f)); btTransform
  // ground_pos; ground_pos.setIdentity(); ground_pos.setOrigin(btVector3(0.0f,
  // 0.0f, 0.0f)); btScalar mass(0.0f); btVector3 inertia(0.0f, 0.0f, 0.0f);
  // motion_ = std::make_shared<btDefaultMotionState>(ground_pos);
  // btRigidBody::btRigidBodyConstructionInfo info(mass, motion_.get(),
  //                                              ground_shape_.get(), inertia);
  // body_ = std::make_shared<btRigidBody>(info);
  // world_->addRigidBody(body_.get());
#pragma endregion

  return true;
}

bool GameDevice::BeginFrame() {
  fps_counter_.Update();

  imgui_manager_.BeginFrame();
  input_manager_->Update();
  audio_manager->Update();

  if (!device_->Prepare()) {
    return false;
  }
  particle_command_list_.BeginFrame(GetDevice());

  global_cb_.GetStagingRef().time = fps_counter_.GetTotalSeconds<float>();
  global_cb_.GetStagingRef().delta_time = fps_counter_.GetDeltaSeconds<float>();
  global_cb_.UpdateStaging();
  global_cb_.RegisterHandle(
      GetDevice(), directx::shader::ConstantBufferRegisterID::GLOBAL_DATA);

  return true;
}

bool GameDevice::MidFrame() {
  particle_command_list_.Execute();
  return true;
}

bool GameDevice::EndFrame() {
  auto& command_list = device_->GetCurrentFrameResource()->GetCommandList();

#pragma region bullet描画テスト
  // world_->debugDrawWorld();
  // math::Vector3 eye(0.0f, 100.0f, -100.0f);
  // math::Vector3 at(0.0f, 0.0f, 0.0f);
  // math::Vector3 up(0.0f, 1.0f, 0.0f);
  // float aspect =
  //    static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
  // float fov = 50.0f * math::util::DEG_2_RAD;
  // float near_z = 0.1f;
  // float far_z = 1000.0f;
  // debug_drawer_->Render(
  //    math::Matrix4x4::CreateView(eye, at, up),
  //    math::Matrix4x4::CreateProjection(fov, aspect, near_z, far_z),
  //    command_list);
#pragma endregion

  imgui_manager_.EndFrame(command_list.GetCommandList());
  if (!device_->Present()) {
    return false;
  }

  return true;
}

void GameDevice::Finalize() {
#pragma region bullet描画テスト用後処理
  //world_->removeCollisionObject(body_.get());
#pragma endregion
  
  device_->Destroy();
}

}  // namespace game
}  // namespace  legend