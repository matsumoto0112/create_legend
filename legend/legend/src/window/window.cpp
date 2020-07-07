#include "src/window/window.h"

#include <Windows.h>

#include <iostream>

#include "src/util/debug.h"

namespace {
//�E�B���h�E�N���X��
static const wchar_t* CLASS_NAME = L"CLASS_NAME";
}  // namespace

namespace legend {
namespace window {

//�R���X�g���N�^
Window::Window()
    : hWnd_(nullptr),
      wndclassex_{},
      window_title_(L"Title"),
      screen_size_(0, 0),
      window_position_(0, 0) {
  wndclassex_.cbSize = sizeof(tagWNDCLASSEXW);
  wndclassex_.style = CS_HREDRAW | CS_VREDRAW;
  wndclassex_.lpfnWndProc = DefWindowProc;
  wndclassex_.cbClsExtra = 0;
  wndclassex_.cbWndExtra = 0;
  wndclassex_.hInstance = nullptr;
  wndclassex_.hIcon = LoadIconW(NULL, IDI_APPLICATION);
  wndclassex_.hCursor = LoadCursorW(NULL, IDC_ARROW);
  wndclassex_.hbrBackground = (HBRUSH__*)(COLOR_WINDOW + 1);
  wndclassex_.lpszMenuName = NULL;
  wndclassex_.lpszClassName = CLASS_NAME;
}

//�E�B���h�E�𐶐�����
void Window::Create() {
  RegisterClassExW(&wndclassex_);

  constexpr u32 WINDOW_FLAGS =
      (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);

  RECT window_rect = {0, 0, static_cast<i32>(screen_size_.x),
                      static_cast<i32>(screen_size_.y)};
  AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, false);

  hWnd_ = CreateWindowExW(0L, CLASS_NAME, window_title_.c_str(), WINDOW_FLAGS,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          window_rect.right - window_rect.left,
                          window_rect.bottom - window_rect.top, nullptr,
                          nullptr, nullptr, callback_object_);

  MY_ASSERTION(hWnd_, L"�E�B���h�E�̐����Ɏ��s���܂���");
}

//�E�B���h�E��\������
void Window::Show(i32 show_command) const { ShowWindow(hWnd_, show_command); }

//�E�B���h�E�v���V�[�W����ݒ肷��
void Window::SetWindowProc(WNDPROC win_proc) {
  wndclassex_.lpfnWndProc = win_proc;
}

//�v���V�[�W���C�x���g�R�[���o�b�N���ݒ肷��
void Window::SetWindowProcCallBack(IWindowProcedureEventCallback* callback) {
  this->callback_object_ = callback;
}

//�X�N���[���T�C�Y���Z�b�g����
void Window::SetScreenSize(const math::IntVector2& screen_size) {
  this->screen_size_ = screen_size;
}

//�E�B���h�E�^�C�g����ݒ肷��
void Window::SetWindowTitle(const std::wstring& title) {
  this->window_title_ = title;
}

//�E�B���h�E���W��ݒ肷��
void Window::SetWindowPosition(const math::IntVector2& window_position) {
  this->window_position_ = window_position;
}

//�E�B���h�E�̐ݒ���X�V����
void Window::UpdateSettings(UpdateWindowSettingFlags flags) const {
  if (util::enum_util::IsBitpop(flags &
                                 UpdateWindowSettingFlags::WindowTitle)) {
    UpdateWindowTitle();
  }
  if (util::enum_util::IsBitpop(flags &
                                 UpdateWindowSettingFlags::ScreenSize) ||
      util::enum_util::IsBitpop(flags &
                                 UpdateWindowSettingFlags::WindowPosition)) {
    UpdateScreenSizeAndPosition();
  }
}

//�E�B���h�E�^�C�g�����X�V����
void Window::UpdateWindowTitle() const {
  SetWindowTextW(hWnd_, window_title_.c_str());
}

//�X�N���[���T�C�Y�ƃE�B���h�E���W���X�V����
void Window::UpdateScreenSizeAndPosition() const {
  //�E�B���h�E�̕��⍂���ɂ̓^�C�g���o�[��ӂ����܂܂�Ă��邽�߁A���̍������擾����
  tagRECT rx;
  tagRECT cx;
  GetWindowRect(hWnd_, &rx);
  GetClientRect(hWnd_, &cx);

  //�擾�����������g���Ē�������
  const i32 new_width =
      screen_size_.x + (rx.right - rx.left) - (cx.right - cx.left);
  const i32 new_height =
      screen_size_.y + (rx.bottom - rx.top) - (cx.bottom - cx.top);
  SetWindowPos(hWnd_, nullptr, window_position_.x, window_position_.y,
               new_width, new_height, SWP_SHOWWINDOW);
}

}  // namespace window
}  // namespace legend
