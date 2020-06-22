#ifndef LEGEND_WINDOW_WINDOW_PROCEDURE_H_
#define LEGEND_WINDOW_WINDOW_PROCEDURE_H_

/**
 * @file window_procedure.h
 * @brief ウィンドウプロシージャ定義
 */


#include "src/window/window_procedure_event_callback.h"

namespace legend {
namespace window {
namespace procedure {

/**
 * @brief ウィンドウプロシージャ関数定義
 */
long long CALLBACK WindowProcdures(HWND__* hWnd, unsigned int msg,
                                   unsigned long long wParam, long long lParam);


}  // namespace procedure
}  // namespace window
}  // namespace legend
#endif  // !LEGEND_WINDOW_WINDOW_PROCEDURE_H_
