#include "src/draw/particle_system.h"

#include <deque>

#include "src/directx/descriptor_heap/heap_parameter.h"
#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"

namespace {
std::shared_ptr<legend::directx::buffer::Texture2D> texture_;
std::vector<legend::directx::buffer::ConstantBuffer<
    legend::directx::constant_buffer_structure::Transform>>
    constant_buffers_;
std::deque<legend::util::Transform> transforms_;
legend::directx::buffer::VertexBuffer vertex_buffer_;
legend::directx::buffer::IndexBuffer index_buffer_;
legend::directx::shader::GraphicsPipelineState pipeline_state_;

}  // namespace

namespace legend {
namespace draw {

ParticleSystem::ParticleSystem() {}
ParticleSystem::~ParticleSystem() {}
bool ParticleSystem::Init(util::resource::id::Texture texture) {
  texture_ =
      game::GameDevice::GetInstance()->GetResource().GetTexture().Get(texture);
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  util::Random& random = game::GameDevice::GetInstance()->GetRandom();
  for (u32 i = 0; i < 100; i++) {
    auto& transform = transforms_.emplace_back();
    transform.SetRotation(
        math::Quaternion::FromEular(0, math::util::DEG_2_RAD * 180.0f, 0));
    transform.SetPosition(math::Vector3(random.Range(-5.0f, 5.0f), 0.0f, 0.0f));
    auto& cb = constant_buffers_.emplace_back();
    if (!cb.Init(
            device, directx::shader::ConstantBufferRegisterID::Transform,
            device.GetLocalHeapHandle(directx::descriptor_heap::heap_parameter::
                                          LocalHeapID::GLOBAL_ID),
            L"CB")) {
      return false;
    }
  }

  const std::vector<directx::Sprite> vertices{
      {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
      {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
      {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
      {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}};
  if (!vertex_buffer_.Init(device, sizeof(directx::Sprite),
                           static_cast<u32>(vertices.size()),
                           L"Particle_VertexBuffer")) {
    return false;
  }
  if (!vertex_buffer_.WriteBufferResource(vertices)) {
    return false;
  }

  const std::vector<u16> indices{0, 1, 2, 0, 2, 3};
  if (!index_buffer_.InitAndWrite(device, indices,
                                  directx::PrimitiveTopology::TriangleList,
                                  L"Particle_IndexBuffer")) {
    return false;
  }

  auto vertex_shader = std::make_shared<directx::shader::VertexShader>();
  const std::filesystem::path vertex_shader_path =
      util::Path::GetInstance()->shader() / "particle" /
      "simple_particle_vs.cso";
  if (!vertex_shader->Init(
          device, vertex_shader_path,
          directx::input_element::GetElementDescs<directx::Sprite>())) {
    return false;
  }

  const std::filesystem::path pixel_shader_path =
      util::Path::GetInstance()->shader() / "particle" /
      "simple_particle_ps.cso";
  auto pixel_shader = std::make_shared<directx::shader::PixelShader>();
  if (!pixel_shader->Init(device, pixel_shader_path)) {
    return false;
  }

  if (!pipeline_state_.Init(device)) {
    return false;
  }
  pipeline_state_.SetRootSignature(device.GetDefaultRootSignature());
  pipeline_state_.SetVertexShader(vertex_shader);
  pipeline_state_.SetPixelShader(pixel_shader);
  pipeline_state_.SetBlendDesc(
      directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT, 0);
  device.GetRenderResourceManager().WriteRenderTargetInfoToPipeline(
      device, directx::render_target::RenderTargetID::BACK_BUFFER,
      &pipeline_state_);
  device.GetRenderResourceManager().WriteDepthStencilTargetInfoToPipeline(
      device, directx::render_target::DepthStencilTargetID::Depth,
      &pipeline_state_);

  if (!pipeline_state_.CreatePipelineState(device)) {
    return false;
  }

  return true;
}
bool ParticleSystem::Update(math::Vector3 v) {
  const u32 num = static_cast<u32>(transforms_.size());
  math::Matrix4x4 bill = math::Matrix4x4::CreateView(math::Vector3(0, 0, 0), v,
                                                     math::Vector3::kUpVector);
  bill = bill.Inverse();
  for (u32 i = 0; i < num; i++) {
    //transforms_[i].SetPosition(transforms_[i].GetPosition() +
    //                           math::Vector3(0, 0.1f, 0));
    //constant_buffers_[i].GetStagingRef().world =
    //    transforms_[i].CreateWorldMatrix() * bill;
  }

  //util::Random& random = game::GameDevice::GetInstance()->GetRandom();
  //auto& transform = transforms_.emplace_back();
  //transform.SetRotation(
  //    math::Quaternion::FromEular(0, math::util::DEG_2_RAD * 180.0f, 0));
  //transform.SetPosition(math::Vector3(random.Range(-5.0f, 5.0f), 0.0f, 0.0f));
  //transforms_.pop_front();
  return true;
}
void ParticleSystem::Render() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  pipeline_state_.SetGraphicsCommandList(device);
  vertex_buffer_.SetGraphicsCommandList(device);
  index_buffer_.SetGraphicsCommandList(device);
  texture_->SetToHeap(device);

  for (auto&& cb : constant_buffers_) {
    cb.UpdateStaging();
    cb.SetToHeap(device);
    device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);
    index_buffer_.Draw(device);
  }
}

}  // namespace draw
}  // namespace legend