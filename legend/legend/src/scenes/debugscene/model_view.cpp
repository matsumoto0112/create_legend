#include "src/scenes/debugscene/model_view.h"

#include "src/directx/render_target/render_target_id.h"
#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"
#include "src/libs/stb_image.h"
#include "src/util/loader/glb_loader.h"
#include "src/util/path.h"
#include "src/util/resource_manager/vertex_shader_resource_manager.h"

namespace legend {
namespace scenes {
namespace debugscene {

//���f����
const std::wstring ModelView::MODEL_NAME = L"kari";

//�R���X�g���N�^
ModelView::ModelView(ISceneChange* scene_change)
    : Scene(scene_change), transform_() {}

ModelView::~ModelView() {
  game::GameDevice::GetInstance()->GetDevice().WaitForGPU();
}

//������
bool ModelView::Initialize() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  util::Resource& resource = game::GameDevice::GetInstance()->GetResource();
  if (!resource.GetVertexShader().Load(
          util::resource_manager::VertexShaderID::MODEL_VIEW,
          util::Path::GetInstance()->shader() / "modelview" /
              "model_view_vs.cso")) {
    return false;
  }

  //���f���f�[�^��ǂݍ���
  const std::filesystem::path model_path =
      util::Path::GetInstance()->model() / (MODEL_NAME + L".glb");
  if (!model_.Init(model_path)) {
    MY_LOG(L"���f���̓ǂݍ��݂Ɏ��s���܂����B");
    return false;
  }

  //�g�����X�t�H�[���o�b�t�@���쐬����
  if (!transform_cb_.Init(
          device, directx::shader::ConstantBufferRegisterID::Transform,
          device.GetLocalHeapHandle(directx::descriptor_heap::heap_parameter::
                                        LocalHeapID::MODEL_VIEW_SCENE),
          L"Transform ConstantBuffer")) {
    return false;
  }

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  //�J�����̏�����
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

  //�p�C�v���C���쐬
  {
    if (!pipeline_state_.Init(game::GameDevice::GetInstance()->GetDevice())) {
      return false;
    }

    const std::filesystem::path pixel_shader_path =
        util::Path::GetInstance()->shader() / L"modelview" /
        L"model_view_ps.cso";
    auto pixel_shader = std::make_shared<directx::shader::PixelShader>();
    if (!pixel_shader->Init(game::GameDevice::GetInstance()->GetDevice(),
                            pixel_shader_path)) {
      return false;
    }

    pipeline_state_.SetRootSignature(device.GetDefaultRootSignature());
    pipeline_state_.SetVertexShader(
        game::GameDevice::GetInstance()->GetResource().GetVertexShader().Get(
            util::resource_manager::VertexShaderID::MODEL_VIEW));
    pipeline_state_.SetPixelShader(pixel_shader);
    device.GetRenderResourceManager().WriteRenderTargetInfoToPipeline(
        device, directx::render_target::RenderTargetID::BACK_BUFFER,
        &pipeline_state_);
    pipeline_state_.SetBlendDesc(
        directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT, 0);

    if (!pipeline_state_.CreatePipelineState(
            game::GameDevice::GetInstance()->GetDevice())) {
      return false;
    }
  }

  return true;
}

//�X�V
bool ModelView::Update() {
  if (ImGui::Begin("Transform")) {
    //�J�������W
    math::Vector3 camera_position = camera_.GetPosition();
    ImGui::SliderFloat3("Position", &camera_position.x, -100.0f, 100.0f);
    camera_.SetPosition(camera_position);
    //�J������]�p
    math::Vector3 camera_rotation =
        math::Quaternion::ToEular(camera_.GetRotation()) *
        math::util::RAD_2_DEG;
    ImGui::SliderFloat3("Rotation", &camera_rotation.x, -180.0f, 180.0f);
    camera_.SetRotation(
        math::Quaternion::FromEular(camera_rotation * math::util::DEG_2_RAD));

    //�J�����̏�����x�N�g����ύX����
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
  return true;
}

//�`��
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
  util::Resource& resource = game::GameDevice::GetInstance()->GetResource();
  resource.GetVertexShader().Unload(
      util::resource_manager::VertexShaderID::MODEL_VIEW);
  game::GameDevice::GetInstance()->GetDevice().WaitForGPU();
}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
