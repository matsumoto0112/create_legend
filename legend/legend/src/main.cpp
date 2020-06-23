#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/directx/shader/pixel_shader.h"
#include "src/directx/shader/vertex_shader.h"
#include "src/directx/vertex.h"
#include "src/game/application.h"
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

    constexpr float D = 0.8f;
    const std::vector<directx::Vertex> vertices = {
        directx::Vertex(math::Vector3(0, 0, 0)),
        directx::Vertex(math::Vector3(0, D, 0)),
        directx::Vertex(math::Vector3(D, 0, 0)),
        directx::Vertex(math::Vector3(0, -D, 0)),
        directx::Vertex(math::Vector3(-D, 0, 0)),
    };
    const u32 vertex_size = sizeof(directx::Vertex);
    const u32 vertex_num = static_cast<u32>(vertices.size());
    if (!vertex_buffer_.Init(GetDirectX12Device(), vertex_size, vertex_num)) {
      return false;
    }
    if (!vertex_buffer_.WriteBufferResource(vertices)) {
      return false;
    }

    const std::vector<u16> indices = {0, 1, 2, 0, 3, 4};
    const u32 index_num = static_cast<u32>(indices.size());
    if (!index_buffer_.Init(GetDirectX12Device(), index_num)) {
      return false;
    }
    if (!index_buffer_.WriteBufferResource(indices)) {
      return false;
    }

    std::filesystem::path path = std::filesystem::current_path();
    path = path / L"assets" / L"shaders" / L"Shader.hlsl";
    std::shared_ptr<directx::shader::VertexShader> vertex_shader =
        std::make_shared<directx::shader::VertexShader>();
    if (!vertex_shader->Init(GetDirectX12Device(), path)) {
      return false;
    }
    std::shared_ptr<directx::shader::PixelShader> pixel_shader =
        std::make_shared<directx::shader::PixelShader>();
    if (!pixel_shader->Init(GetDirectX12Device(), path)) {
      return false;
    }

    pipeline_state_.Init(GetDirectX12Device());
    pipeline_state_.SetVertexShader(vertex_shader);
    pipeline_state_.SetPixelShader(pixel_shader);
    pipeline_state_.CreatePipelineState(GetDirectX12Device());

    return true;
  }
  bool Update() override {
    if (!Application::Update()) {
      return false;
    }

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