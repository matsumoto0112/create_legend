#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/directx/shader/pixel_shader.h"
#include "src/directx/shader/vertex_shader.h"
#include "src/directx/vertex.h"
#include "src/game/application.h"
#include "src/window/window.h"

class MyApp final : public dx12_lib::device::Application {
 public:
  MyApp() : Application() {}
  ~MyApp() {}
  bool Init() override {
    Application::Init();
    dx12_lib::util::debug::Log(L"init_myapp");

    constexpr float D = 0.8f;
    const std::vector<dx12_lib::directx::Vertex> vertices = {
        dx12_lib::directx::Vertex(dx12_lib::math::Vector3(0, 0, 0)),
        dx12_lib::directx::Vertex(dx12_lib::math::Vector3(0, D, 0)),
        dx12_lib::directx::Vertex(dx12_lib::math::Vector3(D, 0, 0)),
        dx12_lib::directx::Vertex(dx12_lib::math::Vector3(0, -D, 0)),
        dx12_lib::directx::Vertex(dx12_lib::math::Vector3(-D, 0, 0)),
    };
    const dx12_lib::u32 vertex_size = sizeof(dx12_lib::directx::Vertex);
    const dx12_lib::u32 vertex_num =
        static_cast<dx12_lib::u32>(vertices.size());
    if (!vertex_buffer_.Init(GetDirectX12Device(), vertex_size, vertex_num)) {
      return false;
    }
    if (!vertex_buffer_.WriteBufferResource(vertices)) {
      return false;
    }

    const std::vector<dx12_lib::u16> indices = {0, 1, 2, 0, 3, 4};
    const dx12_lib::u32 index_num = static_cast<dx12_lib::u32>(indices.size());
    if (!index_buffer_.Init(GetDirectX12Device(), index_num)) {
      return false;
    }
    if (!index_buffer_.WriteBufferResource(indices)) {
      return false;
    }

    std::filesystem::path path = std::filesystem::current_path();
    path = path / L"assets" / L"shaders" / L"Shader.hlsl";
    std::shared_ptr<dx12_lib::directx::shader::VertexShader> vertex_shader =
        std::make_shared<dx12_lib::directx::shader::VertexShader>();
    if (!vertex_shader->Init(GetDirectX12Device(), path)) {
      return false;
    }
    std::shared_ptr<dx12_lib::directx::shader::PixelShader> pixel_shader =
        std::make_shared<dx12_lib::directx::shader::PixelShader>();
    if (!pixel_shader->Init(GetDirectX12Device(), path)) {
      return false;
    }

    pipeline_state_.Init(GetDirectX12Device());
    pipeline_state_.SetVertexShader(vertex_shader);
    pipeline_state_.SetPixelShader(pixel_shader);
    pipeline_state_.CreatePipelineState(GetDirectX12Device());

    return true;
  }
  void Update() override {
    Application::Update();
    dx12_lib::util::debug::Log(L"update_myapp");
  }
  void Draw() override {
    Application::Draw();
    pipeline_state_.SetGraphicsCommandList(GetDirectX12Device());
    vertex_buffer_.SetGraphicsCommandList(GetDirectX12Device());
    index_buffer_.SetGraphicsCommandList(GetDirectX12Device());
    index_buffer_.Draw(GetDirectX12Device());
    dx12_lib::util::debug::Log(L"draw_myapp");
  }
  void Finalize() override {
    Application::Finalize();
    dx12_lib::util::debug::Log(L"init_myfinalize");
  }

 private:
  dx12_lib::directx::buffer::VertexBuffer vertex_buffer_;
  dx12_lib::directx::buffer::IndexBuffer index_buffer_;
  dx12_lib::directx::shader::GraphicsPipelineState pipeline_state_;
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  std::shared_ptr<dx12_lib::window::Window> window =
      std::make_shared<dx12_lib::window::Window>();
  window->SetScreenSize(dx12_lib::math::IntVector2(1280, 720));
  window->SetWindowPosition(dx12_lib::math::IntVector2(0, 0));
  window->SetWindowTitle(L"Legend");

  MyApp app;
  app.RegisterWindow(window);

  app.Run();

  return 0;
}