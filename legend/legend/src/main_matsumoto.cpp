#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/texture_2d.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/descriptor_heap.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/directx/shader/pixel_shader.h"
#include "src/directx/shader/root_signature.h"
#include "src/directx/shader/vertex_shader.h"
#include "src/directx/vertex.h"
#include "src/game/application.h"
#include "src/libs/imgui/imgui.h"
#include "src/math/matrix_4x4.h"
#include "src/util/path.h"
#include "src/window/window.h"

namespace legend {
constexpr u32 CUBE_NUM = 100;

struct Color {
  std::array<float, 4> color;
};
struct Matrix {
  math::Matrix4x4 world;
  math::Matrix4x4 view;
  math::Matrix4x4 proj;
};

class MyApp final : public device::Application {
 public:
  MyApp() : Application() {}
  ~MyApp() {}
  bool Init() override {
    if (!Application::Init()) {
      return false;
    }

    if (!color_constant_buffer.Init(GetDirectX12Device(), 0,
                                    L"Color ConstantBuffer")) {
      return false;
    }
    Color& color = color_constant_buffer.GetStagingRef();
    color.color = {1.0f, 1.0f, 1.0f, 1.0f};

    if (!matrix_constant_buffer.Init(GetDirectX12Device(), 1,
                                     L"Matrix ConstantBuffer")) {
      return false;
    }

    position_ = math::Vector3(0.0f, 0.0f, 0.0f);
    rotation_ = math::Vector3(0, 0, 0);
    Matrix& mat = matrix_constant_buffer.GetStagingRef();
    mat.world = math::Matrix4x4::CreateRotation(rotation_) *
                math::Matrix4x4::CreateTranslate(position_);
    mat.view = math::Matrix4x4::CreateView(math::Vector3(0, 10, -10),
                                           math::Vector3(0, 0, 0),
                                           math::Vector3::kUpVector);
    const math::IntVector2 screen_size = main_window_->GetScreenSize();
    mat.proj = math::Matrix4x4::CreateProjection(
        45.0f, screen_size.x * 1.0f / screen_size.y, 0.1f, 100.0f);

    constexpr u32 WIDTH = 32;
    constexpr u32 HEIGHT = 32;
    struct Pixel {
      u8 color[4];
    };

    std::vector<Pixel> datas(WIDTH * HEIGHT);
    for (u32 w = 0; w < WIDTH; w++) {
      for (u32 h = 0; h < HEIGHT; h++) {
        datas[h * WIDTH + w].color[0] = (w + h) * 4;
        datas[h * WIDTH + w].color[1] = 0x00;
        datas[h * WIDTH + w].color[2] = 0;
        datas[h * WIDTH + w].color[3] = 0xff;
      }
    }

    texture_.Init(GetDirectX12Device(), 0,
                  DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, WIDTH, HEIGHT,
                  L"Yellow Texture");
    texture_.WriteResource(GetDirectX12Device(), datas.data());

    //頂点定義
    constexpr float D = 0.8f;
    const std::vector<directx::Vertex> vertices = {
        directx::Vertex(math::Vector3(-D, D, -D), math::Vector2(0.0f, 0.0f)),
        directx::Vertex(math::Vector3(D, D, -D), math::Vector2(1.0f, 0.0f)),
        directx::Vertex(math::Vector3(D, -D, -D), math::Vector2(1.0f, 1.0f)),
        directx::Vertex(math::Vector3(-D, -D, -D), math::Vector2(0.0f, 1.0f)),

        directx::Vertex(math::Vector3(D, D, -D), math::Vector2(0.0f, 0.0f)),
        directx::Vertex(math::Vector3(D, D, D), math::Vector2(1.0f, 0.0f)),
        directx::Vertex(math::Vector3(D, -D, D), math::Vector2(1.0f, 1.0f)),
        directx::Vertex(math::Vector3(D, -D, -D), math::Vector2(0.0f, 1.0f)),

        directx::Vertex(math::Vector3(D, D, D), math::Vector2(0.0f, 0.0f)),
        directx::Vertex(math::Vector3(-D, D, D), math::Vector2(1.0f, 0.0f)),
        directx::Vertex(math::Vector3(-D, -D, D), math::Vector2(1.0f, 1.0f)),
        directx::Vertex(math::Vector3(D, -D, D), math::Vector2(0.0f, 1.0f)),

        directx::Vertex(math::Vector3(-D, D, D), math::Vector2(0.0f, 0.0f)),
        directx::Vertex(math::Vector3(-D, D, -D), math::Vector2(1.0f, 0.0f)),
        directx::Vertex(math::Vector3(-D, -D, -D), math::Vector2(1.0f, 1.0f)),
        directx::Vertex(math::Vector3(-D, -D, D), math::Vector2(0.0f, 1.0f)),

        directx::Vertex(math::Vector3(-D, D, D), math::Vector2(0.0f, 0.0f)),
        directx::Vertex(math::Vector3(D, D, D), math::Vector2(0.0f, 1.0f)),
        directx::Vertex(math::Vector3(D, D, -D), math::Vector2(1.0f, 1.0f)),
        directx::Vertex(math::Vector3(-D, D, -D), math::Vector2(1.0f, 0.0f)),

        directx::Vertex(math::Vector3(-D, -D, -D), math::Vector2(0.0f, 0.0f)),
        directx::Vertex(math::Vector3(D, -D, -D), math::Vector2(0.0f, 1.0f)),
        directx::Vertex(math::Vector3(D, -D, D), math::Vector2(1.0f, 1.0f)),
        directx::Vertex(math::Vector3(-D, -D, D), math::Vector2(1.0f, 0.0f)),
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
    const std::vector<u16> indices = {
        0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,  8,  9,  10, 8,  10, 11,
        12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23};
    const u32 index_num = static_cast<u32>(indices.size());
    if (!index_buffer_.Init(GetDirectX12Device(), index_num,
                            directx::PrimitiveTopology::TriangleList,
                            L"TestTriangles_IndexBuffer")) {
      return false;
    }
    if (!index_buffer_.WriteBufferResource(indices)) {
      return false;
    }

    root_signature_ = std::make_shared<directx::shader::RootSignature>();
    if (!root_signature_->Init(GetDirectX12Device(),
                               L"Global Root Signature")) {
      return false;
    }
    pipeline_state_.SetRootSignature(root_signature_);

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

    if (ImGui::Begin("Cube Transform")) {
      static std::array<float, 3> position;
      ImGui::SliderFloat3("position", position.data(), -100.0f, 100.0f);
      static std::array<float, 3> rotation;
      ImGui::SliderFloat3("rotation", rotation.data(), 0.0f, 360.0f);
      position_ = math::Vector3(position[0], position[1], position[2]);
      rotation_ = math::Vector3(rotation[0], rotation[1], rotation[2]) *
                  math::util::DEG_2_RAD;
    }
    ImGui::End();

    return true;
  }

  bool Draw() override {
    if (!Application::Draw()) {
      return false;
    }

    root_signature_->SetGraphicsCommandList(GetDirectX12Device());
    pipeline_state_.SetGraphicsCommandList(GetDirectX12Device());

    float value = color_constant_buffer.GetStagingRef().color[1];
    value += 0.01f;
    if (value > 1.0f) value -= 1.0f;
    color_constant_buffer.GetStagingRef().color[1] = value;
    color_constant_buffer.UpdateStaging();

    GetDirectX12Device().GetHeapManager()->SetGraphicsCommandList(
        GetDirectX12Device());
    color_constant_buffer.SetToHeap(GetDirectX12Device());
    matrix_constant_buffer.SetToHeap(GetDirectX12Device());
    matrix_constant_buffer.UpdateStaging();

    texture_.SetToHeap(GetDirectX12Device());
    GetDirectX12Device().GetHeapManager()->PushToGlobalHeapAndSetCommandList(
        GetDirectX12Device());
    vertex_buffer_.SetGraphicsCommandList(GetDirectX12Device());
    index_buffer_.SetGraphicsCommandList(GetDirectX12Device());
    index_buffer_.Draw(GetDirectX12Device());

    return true;
  }
  void Finalize() override { Application::Finalize(); }

 private:
  directx::buffer::VertexBuffer vertex_buffer_;
  directx::buffer::IndexBuffer index_buffer_;
  std::shared_ptr<directx::shader::RootSignature> root_signature_;
  directx::shader::GraphicsPipelineState pipeline_state_;
  directx::buffer::ConstantBuffer<Color> color_constant_buffer;
  directx::buffer::ConstantBuffer<Matrix> matrix_constant_buffer;
  directx::buffer::Texture2D texture_;
  math::Vector3 position_;
  math::Vector3 rotation_;
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