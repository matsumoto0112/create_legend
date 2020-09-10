#include "src/actor/differed_rendering_render_command_list.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"

namespace legend {
namespace actor {

//コンストラクタ
DifferedRenderingRenderCommandList::DifferedRenderingRenderCommandList() {
  back_ground_.Init();
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  {
    const std::vector<directx::Sprite> vertices = {
        {{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
    };
    const u32 size = static_cast<u32>(vertices.size());
    vertex_buffer_.Init(device, sizeof(directx::Sprite), size, L"VB");
    vertex_buffer_.WriteBufferResource(vertices.data());
  }
  {
    const std::vector<u16> indices = {0, 1, 2, 0, 2, 3};
    const u32 size = static_cast<u32>(indices.size());
    index_buffer_.Init(device, sizeof(u16), size,
                       directx::PrimitiveTopology::TRIANGLE_LIST, L"IB");
    index_buffer_.WriteBufferResource(indices.data());
  }
}

//デストラクタ
DifferedRenderingRenderCommandList::~DifferedRenderingRenderCommandList() {}

//追加
void DifferedRenderingRenderCommandList::Push(Actor* actor) {
  actor_list_.emplace_back(actor);
}

//クリア
void DifferedRenderingRenderCommandList::Clear() { actor_list_.clear(); }

//描画
void DifferedRenderingRenderCommandList::RenderPass() {
  using directx::render_target::DepthStencilTargetID;
  using directx::render_target::RenderTargetID;
  namespace TextureRegisterID = directx::shader::TextureRegisterID;
  namespace ConstantBufferRegisterID =
      directx::shader::ConstantBufferRegisterID;

  back_ground_.Draw();
  for (auto&& a : actor_list_) {
    a->Draw();
  }

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  auto& render_resource_manager = device.GetRenderResourceManager();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  render_resource_manager.SetRenderTargets(command_list,
                                           RenderTargetID::BACK_BUFFER, true,
                                           DepthStencilTargetID::NONE, true);

  // Differed-Rendering描画
  resource.GetPipeline()
      .Get(util::resource::resource_names::pipeline::DIFFERED_RENDERING)
      ->SetCommandList(command_list);
  render_resource_manager.UseAsSRV(device, command_list,
                                   RenderTargetID::DIFFERED_RENDERING_PRE,
                                   TextureRegisterID::G_BUFFER_WORLD_POSITION);
  render_resource_manager.UseAsSRV(device, command_list,
                                   RenderTargetID::DIFFERED_RENDERING_PRE,
                                   TextureRegisterID::G_BUFFER_WORLD_NORMAL);
  render_resource_manager.UseAsSRV(device, command_list,
                                   RenderTargetID::DIFFERED_RENDERING_PRE,
                                   TextureRegisterID::G_BUFFER_DIFFUSE);

  device.GetHeapManager().SetHeapTableToGraphicsCommandList(device,
                                                            command_list);

  vertex_buffer_.SetGraphicsCommandList(command_list);
  index_buffer_.SetGraphicsCommandList(command_list);
  index_buffer_.Draw(command_list);
}

}  // namespace actor
}  // namespace legend