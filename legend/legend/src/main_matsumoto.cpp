//#include "src/directx/buffer/constant_buffer.h"
//#include "src/directx/buffer/index_buffer.h"
//#include "src/directx/buffer/texture_2d.h"
//#include "src/directx/buffer/vertex_buffer.h"
//#include "src/directx/descriptor_heap.h"
//#include "src/directx/shader/graphics_pipeline_state.h"
//#include "src/directx/shader/pixel_shader.h"
//#include "src/directx/shader/root_signature.h"
//#include "src/directx/shader/vertex_shader.h"
//#include "src/directx/vertex.h"
//#include "src/game/application.h"
//#include "src/libs/imgui/imgui.h"
//#include "src/math/matrix_4x4.h"
//#include "src/util/path.h"
//#include "src/util/texture_loader.h"
//#include "src/window/window.h"
//
//namespace legend {
//constexpr u32 CUBE_NUM = 100;
//
//struct Color {
//  std::array<float, 4> color;
//};
//struct Matrix {
//  math::Matrix4x4 world;
//  math::Matrix4x4 view;
//  math::Matrix4x4 proj;
//};
//
//class MyApp final : public device::Application {
// public:
//  MyApp() : Application() {}
//  ~MyApp() {}
//  bool Init() override {
//    if (!Application::Init()) {
//      return false;
//    }
//
//    constexpr u16 OBJ_NUM = 10;
//    objects.resize(OBJ_NUM);
//    for (u16 i = 0; i < OBJ_NUM; i++) {
//      if (!objects[i].Init(GetDirectX12Device(), math::Vector3(i * 3.0f, 0, 0)))
//        return false;
//    }
//    const math::IntVector2 screen_size = main_window_->GetScreenSize();
//    projection_ = math::Matrix4x4::CreateProjection(
//        45.0f, screen_size.x * 1.0f / screen_size.y, 0.1f, 100.0f);
//    view_ = math::Matrix4x4::CreateView(math::Vector3(0, 10, -10),
//                                        math::Vector3(0, 0, 0),
//                                        math::Vector3::kUpVector);
//
//    std::filesystem::path p = util::Path::getInstance()->texture() / L"tex.png";
//    if (!texture_.Init(GetDirectX12Device(), 0, p)) {
//      return false;
//    }
//
//    root_signature_ = std::make_shared<directx::shader::RootSignature>();
//    if (!root_signature_->Init(GetDirectX12Device(),
//                               L"Global Root Signature")) {
//      return false;
//    }
//    pipeline_state_.SetRootSignature(root_signature_);
//
//    //頂点シェーダー
//    std::filesystem::path path = util::Path::getInstance()->shader();
//    std::filesystem::path vertex_shader_path = path / L"VertexShader.cso";
//    std::filesystem::path pixel_shader_path = path / L"TextureTest.cso";
//    std::vector<D3D12_INPUT_ELEMENT_DESC> elements{
//        {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,
//         D3D12_APPEND_ALIGNED_ELEMENT,
//         D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
//         0},
//        {"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0,
//         D3D12_APPEND_ALIGNED_ELEMENT,
//         D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
//         0},
//    };
//    std::shared_ptr<directx::shader::VertexShader> vertex_shader =
//        std::make_shared<directx::shader::VertexShader>();
//    if (!vertex_shader->Init(GetDirectX12Device(), vertex_shader_path,
//                             elements)) {
//      return false;
//    }
//
//    //ピクセルシェーダー
//    std::shared_ptr<directx::shader::PixelShader> pixel_shader =
//        std::make_shared<directx::shader::PixelShader>();
//    if (!pixel_shader->Init(GetDirectX12Device(), pixel_shader_path)) {
//      return false;
//    }
//
//    if (!pipeline_state_.Init(GetDirectX12Device())) {
//      return false;
//    }
//    pipeline_state_.SetVertexShader(vertex_shader);
//    pipeline_state_.SetPixelShader(pixel_shader);
//
//    if (!pipeline_state_.CreatePipelineState(GetDirectX12Device())) {
//      return false;
//    }
//
//    return true;
//  }
//  bool Update() override {
//    if (!Application::Update()) {
//      return false;
//    }
//    return true;
//  }
//
//  bool Draw() override {
//    if (!Application::Draw()) {
//      return false;
//    }
//
//    root_signature_->SetGraphicsCommandList(GetDirectX12Device());
//    pipeline_state_.SetGraphicsCommandList(GetDirectX12Device());
//
//    GetDirectX12Device().GetHeapManager().SetGraphicsCommandList(
//        GetDirectX12Device());
//    texture_.SetToHeap(GetDirectX12Device());
//
//    for (auto&& o : objects) {
//      o.Draw(GetDirectX12Device(), view_, projection_, texture_);
//    }
//    return true;
//  }
//  void Finalize() override { Application::Finalize(); }
//
// private:
//  struct Object {
//    bool Init(directx::DirectX12Device& device, const math::Vector3& position) {
//      //頂点定義
//      constexpr float D = 0.8f;
//      const std::vector<directx::Vertex> vertices = {
//          directx::Vertex(math::Vector3(-D, D, -D), math::Vector2(0.0f, 0.0f)),
//          directx::Vertex(math::Vector3(D, D, -D), math::Vector2(1.0f, 0.0f)),
//          directx::Vertex(math::Vector3(D, -D, -D), math::Vector2(1.0f, 1.0f)),
//          directx::Vertex(math::Vector3(-D, -D, -D), math::Vector2(0.0f, 1.0f)),
//
//          directx::Vertex(math::Vector3(D, D, -D), math::Vector2(0.0f, 0.0f)),
//          directx::Vertex(math::Vector3(D, D, D), math::Vector2(1.0f, 0.0f)),
//          directx::Vertex(math::Vector3(D, -D, D), math::Vector2(1.0f, 1.0f)),
//          directx::Vertex(math::Vector3(D, -D, -D), math::Vector2(0.0f, 1.0f)),
//
//          directx::Vertex(math::Vector3(D, D, D), math::Vector2(0.0f, 0.0f)),
//          directx::Vertex(math::Vector3(-D, D, D), math::Vector2(1.0f, 0.0f)),
//          directx::Vertex(math::Vector3(-D, -D, D), math::Vector2(1.0f, 1.0f)),
//          directx::Vertex(math::Vector3(D, -D, D), math::Vector2(0.0f, 1.0f)),
//
//          directx::Vertex(math::Vector3(-D, D, D), math::Vector2(0.0f, 0.0f)),
//          directx::Vertex(math::Vector3(-D, D, -D), math::Vector2(1.0f, 0.0f)),
//          directx::Vertex(math::Vector3(-D, -D, -D), math::Vector2(1.0f, 1.0f)),
//          directx::Vertex(math::Vector3(-D, -D, D), math::Vector2(0.0f, 1.0f)),
//
//          directx::Vertex(math::Vector3(-D, D, D), math::Vector2(0.0f, 0.0f)),
//          directx::Vertex(math::Vector3(D, D, D), math::Vector2(0.0f, 1.0f)),
//          directx::Vertex(math::Vector3(D, D, -D), math::Vector2(1.0f, 1.0f)),
//          directx::Vertex(math::Vector3(-D, D, -D), math::Vector2(1.0f, 0.0f)),
//
//          directx::Vertex(math::Vector3(-D, -D, -D), math::Vector2(0.0f, 0.0f)),
//          directx::Vertex(math::Vector3(D, -D, -D), math::Vector2(0.0f, 1.0f)),
//          directx::Vertex(math::Vector3(D, -D, D), math::Vector2(1.0f, 1.0f)),
//          directx::Vertex(math::Vector3(-D, -D, D), math::Vector2(1.0f, 0.0f)),
//      };
//
//      const u32 vertex_size = sizeof(directx::Vertex);
//      const u32 vertex_num = static_cast<u32>(vertices.size());
//      if (!vertex_buffer_.Init(device, vertex_size, vertex_num,
//                               L"Object_VertexBuffer")) {
//        return false;
//      }
//      if (!vertex_buffer_.WriteBufferResource(vertices)) {
//        return false;
//      }
//
//      //インデックス定義
//      const std::vector<u16> indices = {0,  1,  2,  0,  2,  3,  4,  5,  6,
//                                        4,  6,  7,  8,  9,  10, 8,  10, 11,
//                                        12, 13, 14, 12, 14, 15, 16, 17, 18,
//                                        16, 18, 19, 20, 21, 22, 20, 22, 23};
//      const u32 index_num = static_cast<u32>(indices.size());
//      if (!index_buffer_.Init(device, index_num,
//                              directx::PrimitiveTopology::TriangleList,
//                              L"Object_IndexBuffer")) {
//        return false;
//      }
//      if (!index_buffer_.WriteBufferResource(indices)) {
//        return false;
//      }
//
//      if (!color_constant_buffer.Init(device, 0, L"Color ConstantBuffer")) {
//        return false;
//      }
//      Color& color = color_constant_buffer.GetStagingRef();
//      color.color = {1.0f, 1.0f, 1.0f, 1.0f};
//
//      if (!matrix_constant_buffer.Init(device, 1, L"Matrix ConstantBuffer")) {
//        return false;
//      }
//
//      position_ = position;
//      rotation_ = math::Vector3::kZeroVector;
//      scale_ = math::Vector3::kUnitVector;
//
//      matrix_constant_buffer.GetStagingRef().world =
//          math::Matrix4x4::CreateScale(scale_) *
//          math::Matrix4x4::CreateRotation(rotation_) *
//          math::Matrix4x4::CreateTranslate(position_);
//
//      return true;
//    }
//    void Draw(directx::DirectX12Device& device, const math::Matrix4x4& view,
//              const math::Matrix4x4& proj,
//              directx::buffer::Texture2D& texture) {
//      matrix_constant_buffer.GetStagingRef().view = view;
//      matrix_constant_buffer.GetStagingRef().proj = proj;
//      matrix_constant_buffer.UpdateStaging();
//      matrix_constant_buffer.SetToHeap(device);
//
//      color_constant_buffer.UpdateStaging();
//      color_constant_buffer.SetToHeap(device);
//
//      device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);
//
//      vertex_buffer_.SetGraphicsCommandList(device);
//      index_buffer_.SetGraphicsCommandList(device);
//      index_buffer_.Draw(device);
//    }
//
//   private:
//    directx::buffer::VertexBuffer vertex_buffer_;
//    directx::buffer::IndexBuffer index_buffer_;
//    directx::buffer::ConstantBuffer<Matrix> matrix_constant_buffer;
//    directx::buffer::ConstantBuffer<Color> color_constant_buffer;
//    math::Vector3 position_;
//    math::Vector3 rotation_;
//    math::Vector3 scale_;
//  };
//
//  std::vector<Object> objects;
//  math::Matrix4x4 view_;
//  math::Matrix4x4 projection_;
//  directx::buffer::Texture2D texture_;
//
//  std::shared_ptr<directx::shader::RootSignature> root_signature_;
//  directx::shader::GraphicsPipelineState pipeline_state_;
//};
//}  // namespace legend
//
//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
//  std::shared_ptr<legend::window::Window> window =
//      std::make_shared<legend::window::Window>();
//  window->SetScreenSize(legend::math::IntVector2(1280, 720));
//  window->SetWindowPosition(legend::math::IntVector2(0, 0));
//  window->SetWindowTitle(L"Legend");
//
//  legend::MyApp app;
//  app.RegisterWindow(window);
//
//  app.Run();
//
//  return 0;
//}