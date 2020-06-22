#ifndef LEGEND_WINDOW_WINDOW_H_
#define LEGEND_WINDOW_WINDOW_H_

/**
 * @file window.h
 * @brief シングルウィンドウクラス定義
 */

#include "src/math/int_vector_2.h"
#include "src/util/enum_util.h"
#include "src/window/window_procedure_event_callback.h"

namespace legend {
namespace window {

/**
 * @brief シングルウィンドウクラス
 */
class Window {
 public:
  /**
   * @brief 更新するウィンドウ設定フラグ
   */
  enum class UpdateWindowSettingFlags {
    WindowTitle = 1 << 0,     //!< ウィンドウタイトル
    ScreenSize = 1 << 1,      //!< スクリーンサイズ
    WindowPosition = 1 << 2,  //!< ウィンドウ座標
  };

 public:
  /**
   * @brief コンストラクタ
   */
  Window();
  /**
   * @brief 実際のウィンドウを作成する
   */
  void Create();
  /**
   * @brief ウィンドウを表示する
   */
  void Show(int show_command) const;
  /**
   * @brief ウィンドウプロシージャを設定する
   */
  void SetWindowProc(WNDPROC win_proc);
  /**
   * @brief プロシージャイベントコールバック先を設定する
   */
  void SetWindowProcCallBack(IWindowProcedureEventCallback* callback);
  /**
   * @brief ウィンドウタイトルを取得する
   */
  inline std::wstring GetWindowTitle() const { return window_title_; }
  /**
   * @brief ウィンドウタイトルを設定する
   */
  void SetWindowTitle(const std::wstring& title);
  /**
   * @brief スクリーンサイズを取得する
   */
  inline const math::IntVector2& GetScreenSize() const { return screen_size_; }
  /**
   * @brief スクリーンサイズを設定する
   */
  void SetScreenSize(const math::IntVector2& screen_size);
  /**
   * @brief ウィンドウ座標を取得する
   */
  inline const math::IntVector2& GetWindowPosition() const {
    return window_position_;
  }
  /**
   * @brief ウィンドウ座標を設定する
   */
  void SetWindowPosition(const math::IntVector2& window_position);
  /**
   * @brief ウィンドウハンドルを取得する
   */
  inline HWND__* GetHWND() const { return hWnd_; }
  /**
   * @brief 設定を更新する
   */
  void UpdateSettings(UpdateWindowSettingFlags flags) const;

 private:
  /**
   * @brief スクリーンサイズと座標を更新する
   */
  void UpdateScreenSizeAndPosition() const;
  /**
   * @brief ウィンドウタイトルを更新する
   */
  void UpdateWindowTitle() const;

 private:
  //! ウィンドウハンドル
  HWND__* hWnd_;
  //! ウィンドウ生成時の情報
  WNDCLASSEXW wndclassex_;
  //! ウィンドウタイトル
  std::wstring window_title_;
  //! スクリーンサイズ
  math::IntVector2 screen_size_;
  //! ウィンドウ座標
  math::IntVector2 window_position_;
  //! プロシージャイベントコールバック先
  IWindowProcedureEventCallback* callback_object_;
};
}  // namespace window
}  // namespace legend

namespace legend::util::enum_util {
template <>
struct provides_bitwise_operators<window::Window::UpdateWindowSettingFlags>
    : std::true_type {};
}  // namespace legend::util::enum_util

#endif  // !LEGEND_WINDOW_WINDOW_H_
