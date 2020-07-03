//#include "src/directx/buffer/index_buffer.h"
//#include "src/directx/buffer/texture_2d.h"
//#include "src/directx/buffer/vertex_buffer.h"
//#include "src/directx/shader/graphics_pipeline_state.h"
//#include "src/directx/shader/pixel_shader.h"
//#include "src/directx/shader/root_signature.h"
//#include "src/directx/shader/vertex_shader.h"
//#include "src/directx/vertex.h"
//#include "src/game/application.h"
//#include "src/game/game_device.h"
//#include "src/math/matrix_4x4.h"
//#include "src/scenes/scene.h"
//#include "src/scenes/scene_manager.h"
//#include "src/util/loader/texture_loader.h"
//#include "src/util/path.h"
//#include "src/window/window.h"
//
//namespace legend {
//struct Transform {
//  math::Matrix4x4 world;
//};
//struct WorldContext {
//  math::Matrix4x4 view;
//  math::Matrix4x4 projection;
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
//    MY_LOG(L"init_myapp");
//    if (!sprite_.Init(game::GameDevice::GetInstance()->GetDevice(),
//                      math::Vector3(0, 0, 0)))
//      return false;
//
//    const math::IntVector2 screen_size = main_window_->GetScreenSize();
//    projection_ = math::Matrix4x4::CreateProjection(
//        45.0f, screen_size.x * 1.0f / screen_size.y, 0.1f, 100.0f);
//    camera_position_ = math::Vector3(0, 10, -10);
//    camera_at_ = math::Vector3(0, 0, 0);
//
//    if (!world_constant_buffer_.Init(
//            game::GameDevice::GetInstance()->GetDevice(), 1,
//            L"World Context")) {
//      return false;
//    }
//
//    std::filesystem::path p = util::Path::GetInstance()->texture() / L"tex.png";
//    if (!texture_.InitAndWrite(game::GameDevice::GetInstance()->GetDevice(), 0,
//                               p)) {
//      return false;
//    }
//
//    root_signature_ = std::make_shared<directx::shader::RootSignature>();
//    if (!root_signature_->Init(game::GameDevice::GetInstance()->GetDevice(),
//                               L"Global Root Signature")) {
//      return false;
//    }
//    pipeline_state_.SetRootSignature(root_signature_);
//
//    //頂点シェーダー
//    std::filesystem::path path = util::Path::GetInstance()->shader();
//    std::filesystem::path vertex_shader_path =
//        path / L"draw2d" / L"sprite_vs.cso";
//    std::filesystem::path pixel_shader_path =
//        path / L"draw2d" / L"sprite_ps.cso";
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
//    if (!vertex_shader->Init(game::GameDevice::GetInstance()->GetDevice(),
//                             vertex_shader_path, elements)) {
//      return false;
//    }
//
//    //ピクセルシェーダー
//    std::shared_ptr<directx::shader::PixelShader> pixel_shader =
//        std::make_shared<directx::shader::PixelShader>();
//    if (!pixel_shader->Init(game::GameDevice::GetInstance()->GetDevice(),
//                            pixel_shader_path)) {
//      return false;
//    }
//
//    if (!pipeline_state_.Init(game::GameDevice::GetInstance()->GetDevice())) {
//      return false;
//    }
//    pipeline_state_.SetVertexShader(vertex_shader);
//    pipeline_state_.SetPixelShader(pixel_shader);
//
//    if (!pipeline_state_.CreatePipelineState(
//            game::GameDevice::GetInstance()->GetDevice())) {
//      return false;
//    }
//
//    scene_manager_.Initialize();
//
//    return true;
//  }
//  bool Update() override {
//    if (!Application::Update()) {
//      return false;
//    }
//
//    scene_manager_.Update();
//
//    MY_LOG(L"update_myapp");
//    return true;
//  }
//
//  bool Draw() override {
//    if (!Application::Draw()) {
//      return false;
//    }
//
//    scene_manager_.Draw();
//    MY_LOG(L"draw_myapp");
//
//    root_signature_->SetGraphicsCommandList(
//        game::GameDevice::GetInstance()->GetDevice());
//    pipeline_state_.SetGraphicsCommandList(
//        game::GameDevice::GetInstance()->GetDevice());
//
//    game::GameDevice::GetInstance()
//        ->GetDevice()
//        .GetHeapManager()
//        .SetGraphicsCommandList(game::GameDevice::GetInstance()->GetDevice());
//    texture_.SetToHeap(game::GameDevice::GetInstance()->GetDevice());
//
//    math::Matrix4x4 view = math::Matrix4x4::CreateView(
//        camera_position_, camera_at_, math::Vector3::kUpVector);
//    world_constant_buffer_.GetStagingRef().view = view;
//    world_constant_buffer_.GetStagingRef().projection = projection_;
//    world_constant_buffer_.UpdateStaging();
//    world_constant_buffer_.SetToHeap(
//        game::GameDevice::GetInstance()->GetDevice());
//
//    sprite_.Draw(game::GameDevice::GetInstance()->GetDevice());
//
//    return true;
//  }
//  void Finalize() override {
//    Application::Finalize();
//    MY_LOG(L"finalize_myapp");
//  }
//
// private:
//  struct Sprite {
//    bool Init(directx::DirectX12Device& device, const math::Vector3& position) {
//      std::filesystem::path texture_path =
//          util::Path::GetInstance()->texture() / L"tex.png";
//      util::loader::texture_loader::LoadedTextureData data =
//          util::loader::texture_loader::Load(texture_path);
//
//      std::vector<directx::Sprite> sprites = {
//          {math::Vector3(-1, 1, 0), math::Vector2(0, 0)},
//          {math::Vector3(1, 1, 0), math::Vector2(1, 0)},
//          {math::Vector3(1, -1, 0), math::Vector2(1, 1)},
//          {math::Vector3(-1, -1, 0), math::Vector2(0, 1)}};
//
//      const u32 vertex_size = sizeof(directx::Sprite);
//      const u32 vertex_num = static_cast<u32>(sprites.size());
//      if (!vertex_buffer_.Init(device, vertex_size, vertex_num,
//                               L"Sprite_VertexBuffer")) {
//        return false;
//      }
//      if (!vertex_buffer_.WriteBufferResource(sprites)) {
//        return false;
//      }
//
//      const u32 index_num = sizeof(data.pixels.size());
//       if (!index_buffer_.Init(device, index_num,
//                              directx::PrimitiveTopology::TriangleList,
//                              L"Sprite_IndexBuffer")) {
//        return false;
//      }
//      // if (!index_buffer_.WriteBufferResource() {
//      //  return false;
//      //}
//
//      if (!transform_constant_buffer_.Init(device, 0,
//                                           L"Matrix ConstantBuffer")) {
//        return false;
//      }
//
//      position_ = position;
//      rotation_ = math::Vector3::kZeroVector;
//      scale_ = math::Vector3::kUnitVector * 10.0f;
//
//      transform_constant_buffer_.GetStagingRef().world =
//          math::Matrix4x4::CreateScale(scale_) *
//          math::Matrix4x4::CreateRotation(rotation_) *
//          math::Matrix4x4::CreateTranslate(position_);
//
//      return true;
//    }
//
//    void Draw(directx::DirectX12Device& device) {
//      transform_constant_buffer_.GetStagingRef().world =
//          math::Matrix4x4::CreateScale(scale_) *
//          math::Matrix4x4::CreateRotation(rotation_) *
//          math::Matrix4x4::CreateTranslate(position_);
//      transform_constant_buffer_.UpdateStaging();
//      transform_constant_buffer_.SetToHeap(device);
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
//    directx::buffer::ConstantBuffer<Transform> transform_constant_buffer_;
//    math::Vector3 position_;
//    math::Vector3 rotation_;
//    math::Vector3 scale_;
//  };
//
//  Sprite sprite_;
//  scenes::SceneManager scene_manager_;
//  math::Vector3 camera_position_;
//  math::Vector3 camera_at_;
//  math::Matrix4x4 projection_;
//  directx::buffer::Texture2D texture_;
//  directx::buffer::ConstantBuffer<WorldContext> world_constant_buffer_;
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