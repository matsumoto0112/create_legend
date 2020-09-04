#include "src/skill/skill_paste.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace {
DXGI_FORMAT GetFormat(legend::u32 size) {
  switch (size) {
    case 4:
      return DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
    default:
      return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
  }
}
}  // namespace

namespace legend {
namespace skill {
namespace resource_name = util::resource::resource_names;

//コンストラクタ
SkillPaste::SkillPaste() : Parent(L"SkillPaste") {}

//デストラクタ
SkillPaste::~SkillPaste() {}

//初期化
void SkillPaste::Init(math::Vector3 position, actor::IActorMediator* mediator) {
  if (!Parent::Init(mediator)) {
    return;
  }

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();

  if (!transform_cb_.Init(
          device,
          device.GetHeapManager().GetLocalHeap(
              directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
          L"Paste_TransformConstantBuffer")) {
    return;
  }

  float y = game::GameDevice::GetInstance()->GetRandom().Range(-180.0f, 180.0f);
  math::Quaternion rotate =
      math::Quaternion::FromEular(0, y * math::util::DEG_2_RAD, 0);

  transform_.SetPosition(position);
  transform_.SetRotation(rotate);
  transform_.SetScale(math::Vector3::kUnitVector);

  bullet::BoundingBox::InitializeParameter parameter;
  parameter.position = transform_.GetPosition();
  parameter.rotation = transform_.GetRotation();
  parameter.scale = math::Vector3(3.25f, 0.25f, 3.25f);
  parameter.mass = 0.0f;
  box_ = std::make_shared<bullet::BoundingBox>(this, parameter);
  box_->SetFlags(box_->GetFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
  mediator->AddCollider(box_);

  auto& resource = game::GameDevice::GetInstance()->GetResource();

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  model_ = resource.GetModel().Get(
      util::resource::resource_names::model::GLUE_PLANE);
}

//更新
bool SkillPaste::Update() { return true; }

//描画
void SkillPaste::Draw() { Parent::Draw(); }

//削除
void SkillPaste::Destroy(actor::IActorMediator* mediator) {
  mediator->RemoveCollider(box_);
  mediator->RemoveActor(this);
}

}  // namespace skill
}  // namespace legend