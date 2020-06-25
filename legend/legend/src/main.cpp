#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/descriptor_heap.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/directx/shader/pixel_shader.h"
#include "src/directx/shader/vertex_shader.h"
#include "src/directx/vertex.h"
#include "src/game/application.h"
#include "src/libs/imgui/imgui.h"
#include "src/util/path.h"
#include "src/window/window.h"

namespace legend {

class MyApp final : public device::Application {
 public:
  MyApp() : Application() {}
  ~MyApp() {}
  bool Init() override {
    if (!Application::Init()) {
      return false;
    }

    MY_LOG(L"init_myapp");

    directx::DescriptorHeap::Desc heap_desc = {};
    heap_desc.descriptor_num = 10;
    heap_desc.type = directx::HeapType::CBV_SRV_UAV;
    heap_desc.flag = directx::HeapFlag::ShaderVisible;
    if (!heap_.Init(GetDirectX12Device(), heap_desc, L"ShaderResourceHeap")) {
      return false;
    }

    //頂点定義
    constexpr float D = 0.8f;
    const std::vector<directx::Vertex> vertices = {
        directx::Vertex(math::Vector3(0, 0, 0), math::Vector2(0.5f, 0.5f)),
        directx::Vertex(math::Vector3(0, D, 0), math::Vector2(0.5f, 0.0f)),
        directx::Vertex(math::Vector3(D, 0, 0), math::Vector2(1.0f, 0.5f)),
        directx::Vertex(math::Vector3(0, -D, 0), math::Vector2(0.5f, 1.0f)),
        directx::Vertex(math::Vector3(-D, 0, 0), math::Vector2(0.0f, 0.5f)),
    };
    const u32 vertex_size = sizeof(directx::Vertex);
    const u32 vertex_num = static_cast<u32>(vertices.size());
    if (!vertex_buffer_.Init(GetDirectX12Device(), vertex_size, vertex_num,
                             L"TestTriangle_VertexBuffer")) {
      return false;
    }
    if (!vertex_buffer_.WriteBufferResource(vertices)) {
      return false;
    }

    //インデックス定義
    const std::vector<u16> indices = {0, 1, 2, 0, 3, 4};
    const u32 index_num = static_cast<u32>(indices.size());
    if (!index_buffer_.Init(GetDirectX12Device(), index_num,
                            directx::PrimitiveTopology::TriangleList,
                            L"TestTriangles_IndexBuffer")) {
      return false;
    }
    if (!index_buffer_.WriteBufferResource(indices)) {
      return false;
    }

    //頂点シェーダー
    std::filesystem::path path = util::Path::getInstance()->shader();
    std::filesystem::path vertex_shader_path = path / L"VertexShader.cso";
    std::filesystem::path pixel_shader_path = path / L"TextureTest.cso";
    std::vector<D3D12_INPUT_ELEMENT_DESC> elements{
        {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,
         D3D12_APPEND_ALIGNED_ELEMENT,
         D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
         0},
        {"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0,
         D3D12_APPEND_ALIGNED_ELEMENT,
         D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
         0},
    };
    std::shared_ptr<directx::shader::VertexShader> vertex_shader =
        std::make_shared<directx::shader::VertexShader>();
    if (!vertex_shader->Init(GetDirectX12Device(), vertex_shader_path,
                             elements)) {
      return false;
    }

    //ピクセルシェーダー
    std::shared_ptr<directx::shader::PixelShader> pixel_shader =
        std::make_shared<directx::shader::PixelShader>();
    if (!pixel_shader->Init(GetDirectX12Device(), pixel_shader_path)) {
      return false;
    }

    if (!pipeline_state_.Init(GetDirectX12Device())) {
      return false;
    }
    pipeline_state_.SetVertexShader(vertex_shader);
    pipeline_state_.SetPixelShader(pixel_shader);

    if (!pipeline_state_.CreatePipelineState(GetDirectX12Device())) {
      return false;
    }

    return true;
  }
  bool Update() override {
    if (!Application::Update()) {
      return false;
    }

    if (ImGui::Begin("Debug Window")) {
      ImGui::Text("Debug Message");
      static float value;
      ImGui::SliderFloat("Debug Value", &value, 0.0f, 100.0f);
      if (ImGui::Button("Debug Button")) {
        ImGui::Text("Debug Button Message");
        value = 50.0f;
      }
    }
    ImGui::End();

    MY_LOG(L"update_myapp");
    return true;
  }

  bool Draw() override {
    if (!Application::Draw()) {
      return false;
    }

    pipeline_state_.SetGraphicsCommandList(GetDirectX12Device());
    vertex_buffer_.SetGraphicsCommandList(GetDirectX12Device());
    index_buffer_.SetGraphicsCommandList(GetDirectX12Device());
    index_buffer_.Draw(GetDirectX12Device());
    MY_LOG(L"draw_myapp");

    return true;
  }
  void Finalize() override {
    Application::Finalize();
    MY_LOG(L"finalize_myapp");
  }

 private:
  directx::buffer::VertexBuffer vertex_buffer_;
  directx::buffer::IndexBuffer index_buffer_;
  directx::shader::GraphicsPipelineState pipeline_state_;
  directx::DescriptorHeap heap_;
};
}  // namespace legend

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  std::shared_ptr<legend::window::Window> window =
      std::make_shared<legend::window::Window>();
  window->SetScreenSize(legend::math::IntVector2(1280, 720));
  window->SetWindowPosition(legend::math::IntVector2(0, 0));
  window->SetWindowTitle(L"Legend");

  legend::MyApp app;
  app.RegisterWindow(window);

  app.Run();

  return 0;
}