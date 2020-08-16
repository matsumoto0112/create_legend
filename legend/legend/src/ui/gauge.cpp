#include "src/ui/gauge.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace ui {

//コンストラクタ
Gauge::Gauge() {}

//デストラクタ
Gauge::~Gauge() {}

//初期化
bool Gauge::Init(
    const std::wstring& name,
    directx::descriptor_heap::heap_parameter::LocalHeapID heap_id) {
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  if (!sprite_.Init(resource.GetTexture().Get(name), heap_id)) {
    return false;
  }

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  return constant_buffer_.Init(device, device.GetLocalHandle(heap_id),
                               name + L"_GaugeConstantBuffer");
}

//座標を設定する
void Gauge::SetPosition(const math::Vector2& position) {
  sprite_.SetPosition(position);
}

//座標を取得する
math::Vector2 Gauge::GetPosition() const { return sprite_.GetPosition(); }

// Z-Orderを設定する
void Gauge::SetZOrder(float z) { sprite_.SetZOrder(z); }

// Z-Orderを取得する
float Gauge::GetZOrder() const { return sprite_.GetZOrder(); }

//描画リストに追加
void Gauge::StackOwnItems(draw::SpriteRenderer& renderer) {
  //描画前に呼ぶ関数定義
  //通常スプライトではなく、ゲージ用マテリアルでの描画にし、コンスタントバッファをセットする
  auto pre_call_func = [&]() {
    auto& device = game::GameDevice::GetInstance()->GetDevice();
    auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
    auto& resource = game::GameDevice::GetInstance()->GetResource();
    resource.GetPipeline()
        .Get(util::resource::resource_names::pipeline::GAUGE)
        ->SetCommandList(command_list);
    constant_buffer_.GetStagingRef() = param_;
    constant_buffer_.UpdateStaging();
    constant_buffer_.RegisterHandle(
        device, directx::shader::ConstantBufferRegisterID::GAUGE_PARAMETER);
  };

  renderer.AddDrawItems(&sprite_, pre_call_func);
}

//ゲージの値を設定する
void Gauge::SetValue(float value) { param_.value = value; }

//ゲージの値を取得する
float Gauge::GetValue() const { return param_.value; }

}  // namespace ui
}  // namespace legend