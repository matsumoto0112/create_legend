#include "src/window/window_procedure.h"

namespace legend {
namespace window {
namespace procedure {
/**
 * @brief 内部実装
 */
namespace details {

//生成時に呼ばれる
long long Create(HWND__* hWnd, long long lParam);

//破壊時に呼ばれる
long long Destroy(IWindowProcedureEventCallback* callback);

//描画
long long Paint(IWindowProcedureEventCallback* callback);
}  // namespace details

//ウィンドウプロシージャ
long long CALLBACK WindowProcdures(HWND__* hWnd, unsigned int msg,
                                   unsigned long long wParam,
                                   long long lParam) {
  //ウィンドウ生成時に登録したポインタからコールバックを取得する
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
//生成時
long long Create(HWND__* hWnd, long long lParam) {
  tagCREATESTRUCTW* create_struct = reinterpret_cast<tagCREATESTRUCTW*>(lParam);
  SetWindowLongPtrW(hWnd, GWLP_USERDATA,
                    reinterpret_cast<long long>(create_struct->lpCreateParams));

  return 0L;
}

//破壊時
long long Destroy(IWindowProcedureEventCallback* callback) {
  //破壊時の通知
  if (callback) callback->Destroy();
  PostQuitMessage(0);
  return 0L;
}

//描画
long long Paint(IWindowProcedureEventCallback* callback) {
  if (callback) callback->Paint();
  return 0L;
}

}  // namespace details
}  // namespace procedure
}  // namespace window
}  // namespace legend
