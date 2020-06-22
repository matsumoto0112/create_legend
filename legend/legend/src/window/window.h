#ifndef LEGEND_WINDOW_WINDOW_H_
#define LEGEND_WINDOW_WINDOW_H_

/**
 * @file window.h
 * @brief �V���O���E�B���h�E�N���X��`
 */

#include "src/math/int_vector_2.h"
#include "src/util/enum_util.h"
#include "src/window/window_procedure_event_callback.h"

namespace legend {
namespace window {

/**
 * @brief �V���O���E�B���h�E�N���X
 */
class Window {
 public:
  /**
   * @brief �X�V����E�B���h�E�ݒ�t���O
   */
  enum class UpdateWindowSettingFlags {
    WindowTitle = 1 << 0,     //!< �E�B���h�E�^�C�g��
    ScreenSize = 1 << 1,      //!< �X�N���[���T�C�Y
    WindowPosition = 1 << 2,  //!< �E�B���h�E���W
  };

 public:
  /**
   * @brief �R���X�g���N�^
   */
  Window();
  /**
   * @brief ���ۂ̃E�B���h�E���쐬����
   */
  void Create();
  /**
   * @brief �E�B���h�E��\������
   */
  void Show(int show_command) const;
  /**
   * @brief �E�B���h�E�v���V�[�W����ݒ肷��
   */
  void SetWindowProc(WNDPROC win_proc);
  /**
   * @brief �v���V�[�W���C�x���g�R�[���o�b�N���ݒ肷��
   */
  void SetWindowProcCallBack(IWindowProcedureEventCallback* callback);
  /**
   * @brief �E�B���h�E�^�C�g�����擾����
   */
  inline std::wstring GetWindowTitle() const { return window_title_; }
  /**
   * @brief �E�B���h�E�^�C�g����ݒ肷��
   */
  void SetWindowTitle(const std::wstring& title);
  /**
   * @brief �X�N���[���T�C�Y���擾����
   */
  inline const math::IntVector2& GetScreenSize() const { return screen_size_; }
  /**
   * @brief �X�N���[���T�C�Y��ݒ肷��
   */
  void SetScreenSize(const math::IntVector2& screen_size);
  /**
   * @brief �E�B���h�E���W���擾����
   */
  inline const math::IntVector2& GetWindowPosition() const {
    return window_position_;
  }
  /**
   * @brief �E�B���h�E���W��ݒ肷��
   */
  void SetWindowPosition(const math::IntVector2& window_position);
  /**
   * @brief �E�B���h�E�n���h�����擾����
   */
  inline HWND__* GetHWND() const { return hWnd_; }
  /**
   * @brief �ݒ���X�V����
   */
  void UpdateSettings(UpdateWindowSettingFlags flags) const;

 private:
  /**
   * @brief �X�N���[���T�C�Y�ƍ��W���X�V����
   */
  void UpdateScreenSizeAndPosition() const;
  /**
   * @brief �E�B���h�E�^�C�g�����X�V����
   */
  void UpdateWindowTitle() const;

 private:
  //! �E�B���h�E�n���h��
  HWND__* hWnd_;
  //! �E�B���h�E�������̏��
  WNDCLASSEXW wndclassex_;
  //! �E�B���h�E�^�C�g��
  std::wstring window_title_;
  //! �X�N���[���T�C�Y
  math::IntVector2 screen_size_;
  //! �E�B���h�E���W
  math::IntVector2 window_position_;
  //! �v���V�[�W���C�x���g�R�[���o�b�N��
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
