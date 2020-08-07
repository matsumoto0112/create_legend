#include "src/scenes/debugscene/model_view.h"

#include "src/directx/directx_helper.h"
#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/pixel_shader.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/shader/vertex_shader.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"
#include "src/util/path.h"

namespace {
constexpr legend::u32 OBJ_NUM = 1;
}  // namespace

namespace legend {
namespace scenes {
namespace debugscene {

//コンストラクタ
ModelView::ModelView(ISceneChange* scene_change) : Scene(scene_change) {}

ModelView::~ModelView() {}

//初期化
bool ModelView::Initialize() {
  if (!Scene::Initialize()) {
    return false;
  }

  auto& device = game::GameDevice::GetInstance()->GetDevice();

  if (!device.GetHeapManager().AddLocalHeap(
          device, directx::descriptor_heap::heap_parameter::LocalHeapID::
                      MODEL_VIEW_SCENE)) {
    return false;
  }

  transform_cb_.resize(OBJ_NUM);
  transforms_.resize(OBJ_NUM);
  for (u32 i = 0; i < OBJ_NUM; i++) {
    if (!transform_cb_[i].Init(
            device, directx::shader::ConstantBufferRegisterID::TRANSFORM,
            device.GetLocalHandle(directx::descriptor_heap::heap_parameter::
                                      LocalHeapID::GLOBAL_ID),
            L"Transform")) {
      return false;
    }
    transforms_[i].SetPosition(math::Vector3(i * 0.6f, 0.0f, i * 0.6f));
    transform_cb_[i].GetStagingRef().world = transforms_[i].CreateWorldMatrix();
    transform_cb_[i].UpdateStaging();
  }
  {
    const math::Vector3 pos = math::Vector3(0.0f, 0.3f, -0.5f);
    const math::Quaternion rot =
        math::Quaternion::FromEular(20.0f * math::util::DEG_2_RAD, 0.0f, 0.0f);
    const float fov = 50.0f * math::util::DEG_2_RAD;
    const float aspect = 1280.0f / 720.0f;
    if (!camera_.Init(L"MainCamera", pos, rot, fov, aspect)) {
      return false;
    }
  }

  return true;
}

//更新
bool ModelView::Update() {
  if (!Scene::Update()) {
    return false;
  }

  return true;
}

//描画
void ModelView::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto& render_resource_manager = device.GetRenderResourceManager();
  directx::device::CommandList& command_list =
      device.GetCurrentFrameResource()->GetCommandList();
  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, true,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

  resource.GetPipeline()
      .Get(util::resource::id::Pipeline::MODEL_VIEW)
      ->SetGraphicsCommandList(command_list);
  camera_.RenderStart();

  for (i32 i = OBJ_NUM - 1; i >= 0; i--) {
    transform_cb_[i].GetStagingRef().world = transforms_[i].CreateWorldMatrix();
    transform_cb_[i].UpdateStaging();
    transform_cb_[i].SetToHeap(device);
    resource.GetModel()
        .Get(util::resource::id::Model::DESK)
        ->Draw(command_list);
  }
}

void ModelView::Finalize() {
  game::GameDevice::GetInstance()->GetDevice().GetHeapManager().RemoveLocalHeap(
      directx::descriptor_heap::heap_parameter::LocalHeapID::MODEL_VIEW_SCENE);
}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
