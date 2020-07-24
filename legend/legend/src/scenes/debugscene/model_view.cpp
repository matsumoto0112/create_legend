#include "src/scenes/debugscene/model_view.h"

#include "src/directx/render_target/render_target_id.h"
#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"
#include "src/libs/stb_image.h"
#include "src/util/loader/glb_loader.h"
#include "src/util/path.h"

namespace legend {
namespace scenes {
namespace debugscene {

//モデル名
const std::wstring ModelView::MODEL_NAME = L"1000cmObject";

//コンストラクタ
ModelView::ModelView(ISceneChange* scene_change)
    : Scene(scene_change), transform_() {}

ModelView::~ModelView() {
  game::GameDevice::GetInstance()->GetDevice().WaitForGPU();
}

//初期化
bool ModelView::Initialize() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  //モデルデータを読み込む
  const std::filesystem::path model_path =
      util::Path::GetInstance()->model() / (MODEL_NAME + L".glb");
  if (!model_.Init(model_path)) {
    MY_LOG(L"モデルの読み込みに失敗しました。");
    return false;
  }

  //トランスフォームバッファを作成する
  if (!transform_cb_.Init(
          device, directx::shader::ConstantBufferRegisterID::Transform,
          device.GetLocalHeapHandle(directx::descriptor_heap::heap_parameter::
                                        LocalHeapID::MODEL_VIEW_SCENE),
          L"Transform ConstantBuffer")) {
    return false;
  }

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  //カメラの初期化
  {
    const math::Vector3 camera_position = math::Vector3(0, 10, -10);
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

  //パイプライン作成
  {
    if (!pipeline_state_.Init(game::GameDevice::GetInstance()->GetDevice())) {
      return false;
    }

    const std::filesystem::path path = util::Path::GetInstance()->shader();
    const std::filesystem::path vertex_shader_path =
        path / L"modelview" / L"model_view_vs.cso";
    auto vertex_shader = std::make_shared<directx::shader::VertexShader>();
    if (!vertex_shader->Init(
            game::GameDevice::GetInstance()->GetDevice(), vertex_shader_path,
            directx::input_element::GetElementDescs<directx::Vertex>())) {
      return false;
    }

    const std::filesystem::path pixel_shader_path =
        path / L"modelview" / L"model_view_ps.cso";
    auto pixel_shader = std::make_shared<directx::shader::PixelShader>();
    if (!pixel_shader->Init(game::GameDevice::GetInstance()->GetDevice(),
                            pixel_shader_path)) {
      return false;
    }

    pipeline_state_.SetRootSignature(device.GetDefaultRootSignature());
    pipeline_state_.SetVertexShader(vertex_shader);
    pipeline_state_.SetPixelShader(pixel_shader);
    device.GetRenderResourceManager().WriteRenderTargetInfoToPipelineDesc(
        device, directx::render_target::RenderTargetID::BACK_BUFFER,
        pipeline_state_);
    pipeline_state_.SetBlendDesc(
        directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT, 0);

    if (!pipeline_state_.CreatePipelineState(
            game::GameDevice::GetInstance()->GetDevice())) {
      return false;
    }
  }

  return true;
}  // namespace scenes

//更新
bool ModelView::Update() {
  if (ImGui::Begin("Transform")) {
  }
  ImGui::End();
  return true;
}

//描画
void ModelView::Draw() {
  Scene::Draw();

  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  pipeline_state_.SetGraphicsCommandList(device);
  camera_.RenderStart();
  transform_cb_.SetToHeap(device);
  model_.Draw();
}

void ModelView::Finalize() {
  game::GameDevice::GetInstance()->GetDevice().WaitForGPU();
}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
