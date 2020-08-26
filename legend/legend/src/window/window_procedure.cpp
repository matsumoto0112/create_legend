#include "src/window/window_procedure.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg,
                                              WPARAM wParam, LPARAM lParam);

namespace legend {
namespace window {
namespace procedure {
/**
 * @brief ��������
 */
namespace details {

//�������ɌĂ΂��
long long Create(HWND__* hWnd, long long lParam);

//�j�󎞂ɌĂ΂��
long long Destroy(IWindowProcedureEventCallback* callback);

//�`��
long long Paint(IWindowProcedureEventCallback* callback);

}  // namespace details

//�E�B���h�E�v���V�[�W��
long long CALLBACK WindowProcdures(HWND__* hWnd, unsigned int msg,
                                   unsigned long long wParam,
                                   long long lParam) {
  if (util::debug::ENABLE_DEBUG) {
    // Imgui�C�x���g���������Ă�����ȍ~�̏����̕K�v�Ȃ�
    {
      i64 res = ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
      if (res) return res;
    }
  }

  //�E�B���h�E�������ɓo�^�����|�C���^����R�[���o�b�N���擾����
  IWindowProcedureEventCallback* callback =
      reinterpret_cast<IWindowProcedureEventCallback*>(
          GetWindowLongPtrW(hWnd, GWLP_USERDATA));

  switch (msg) {
    case WM_CREATE:
      return details::Create(hWnd, lParam);
    case WM_PAINT:
      return details::Paint(callback);
    case WM_DESTROY:
      return details::Destroy(callback);
  }

  return DefWindowProcW(hWnd, msg, wParam, lParam);
}

namespace details {
//������
long long Create(HWND__* hWnd, long long lParam) {
  tagCREATESTRUCTW* create_struct = reinterpret_cast<tagCREATESTRUCTW*>(lParam);
  SetWindowLongPtrW(hWnd, GWLP_USERDATA,
                    reinterpret_cast<long long>(create_struct->lpCreateParams));

  return 0L;
}

//�j��
long long Destroy(IWindowProcedureEventCallback* callback) {
  //�j�󎞂̒ʒm
  if (callback) callback->Destroy();
  PostQuitMessage(0);
  return 0L;
}

//�`��
long long Paint(IWindowProcedureEventCallback* callback) {
  if (callback) callback->Paint();
  return 0L;
}

}  // namespace details
}  // namespace procedure
}  // namespace window
}  // namespace legend
