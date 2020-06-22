#include "src/window/window_procedure.h"

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

  return DefWindowProc(hWnd, msg, wParam, lParam);
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
