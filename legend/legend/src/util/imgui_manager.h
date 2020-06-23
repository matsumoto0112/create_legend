#ifndef LEGEND_UTIL_IMGUI_MANAGER_H_
#define LEGEND_UTIL_IMGUI_MANAGER_H_

/**
 * @file imgui_manager.h
 * @brief ImGUI管理クラス定義
 */

namespace legend {
namespace util {
/**
 * @brief Imgui管理
 */
class ImguiManager {
 public:
  /**
   * @brief コンストラクタ
   */
  ImguiManager();
  /**
   * @brief デストラクタ
   */
  ~ImguiManager();
  /**
   * @brief 初期化
   * @param window_handle ウィンドウハンドル
   * @param device DirectX12デバイス
   * @param back_buffer_format バックバッファのフォーマット
   * @param frame_count バックバッファの枚数
   * @param enable_gui GUIを有効にするかどうか
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(HWND__* window_handle, ID3D12Device* device,
            DXGI_FORMAT back_buffer_format, u32 frame_count,
            bool enable_gui = true);
  /**
   * @brief GUIを可能にするかどうかセットする
   * @param enable trueなら有効にする
   */
  void SetEnableGui(bool enable) { enable_gui_ = enable; }
  /**
   * @brief フレーム開始時
   */
  void BeginFrame();
  /**
   * @brief フレーム終了時
   * @param command_list コマンドリスト
   */
  void EndFrame(ID3D12GraphicsCommandList* command_list);

 private:
  //! ImGui用ヒープ
  ComPtr<ID3D12DescriptorHeap> heap_;
  //! guiが有効かどうか
  bool enable_gui_;
};

}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_IMGUI_MANAGER_H_
