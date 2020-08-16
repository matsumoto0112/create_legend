#include "src/ui/quarter_gauge.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace ui {

//コンストラクタ
QuarterGauge::QuarterGauge() : Gauge() {}

//デストラクタ
QuarterGauge::~QuarterGauge() {}

//初期化
bool QuarterGauge::Init(
    const std::wstring& name,
    directx::descriptor_heap::heap_parameter::LocalHeapID heap_id) {
  return Gauge::Init(name, heap_id);
}

//描画リストに追加
void QuarterGauge::StackOwnItems(draw::SpriteRenderer& renderer) {
  //ほとんどGaugeと同じだが、使用するシェーダーが違う

  //描画前に呼ぶ関数定義
  //通常スプライトではなく、ゲージ用マテリアルでの描画にし、コンスタントバッファをセットする
  auto pre_call_func = [&]() {
    auto& device = game::GameDevice::GetInstance()->GetDevice();
    auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
    auto& resource = game::GameDevice::GetInstance()->GetResource();
    resource.GetPipeline()
        .Get(util::resource::resource_names::pipeline::QUARTER_GAUGE)
        ->SetCommandList(command_list);
    constant_buffer_.GetStagingRef() = param_;
    constant_buffer_.UpdateStaging();
    constant_buffer_.RegisterHandle(
        device, directx::shader::ConstantBufferRegisterID::GAUGE_PARAMETER);
  };

  renderer.AddDrawItems(&sprite_, pre_call_func);
}

}  // namespace ui
}  // namespace legend