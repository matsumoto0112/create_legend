#include "src/util/imgui_manager.h"

#include "src/libs/imgui/imgui.h"
#include "src/libs/imgui/imgui_impl_dx12.h"
#include "src/libs/imgui/imgui_impl_win32.h"
#include "src/util/path.h"

namespace legend {
namespace util {

//コンストラクタ
ImguiManager::ImguiManager() : heap_(nullptr), enable_gui_(false) {}

//デストラクタ
ImguiManager::~ImguiManager() {
  ImGui_ImplDX12_Shutdown();
  ImGui_ImplWin32_Shutdown();
  if (ImGui::GetCurrentContext()) {
    ImGui::DestroyContext();
  }
}

//初期化
bool ImguiManager::Init(HWND__* window_handle, ID3D12Device* device,
                        DXGI_FORMAT back_buffer_format, u32 frame_count,
                        bool enable_gui) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();

  //const std::filesystem::path font_path =
  //    Path::GetInstance()->exe() / "assets" / "fonts" / "APJapanesefontF.ttf";
  //io.Fonts->AddFontFromFileTTF(font_path.generic_string().c_str(), 14.0f,
  //                             nullptr, io.Fonts->GetGlyphRangesJapanese());
  ImGui::StyleColorsDark();
  ImGui_ImplWin32_Init(window_handle);

  // ImGui用のヒープ確保
  D3D12_DESCRIPTOR_HEAP_DESC desc = {};
  desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
  desc.NumDescriptors = 1;
  desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
  device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap_));

  if (!ImGui_ImplDX12_Init(device, frame_count, back_buffer_format,
                           heap_->GetCPUDescriptorHandleForHeapStart(),
                           heap_->GetGPUDescriptorHandleForHeapStart())) {
    return false;
  }

  this->enable_gui_ = enable_gui;
  return true;
}

//フレーム開始時
void ImguiManager::BeginFrame() {
  ImGui_ImplDX12_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
}

void ImguiManager::EndFrame(ID3D12GraphicsCommandList* command_list) {
  if (enable_gui_) {
    command_list->SetDescriptorHeaps(1, heap_.GetAddressOf());
    ImGui::Render();
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), command_list);
  }
  ImGui::EndFrame();
}

}  // namespace util
}  // namespace legend