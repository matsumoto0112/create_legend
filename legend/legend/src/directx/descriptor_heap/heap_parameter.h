#ifndef LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_PARAMETER_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_PARAMETER_H_

/**
 * @file heap_parameter.h
 * @brief ヒープのデフォルトパラメータ定義ファイル
 * @details コンパイル時に決定できる、基本的に変更しない情報を定義する
 */

#include "src/util/stl_extend.h"

namespace legend {
namespace directx {
namespace descriptor_heap {

/**
 * @brief /ヒープのデフォルトパラメータ
 */
namespace heap_parameter {

//! アプリケーション全体でのディスクリプタヒープの最大数
//! CBV,SRV,UAVで共通する割り当て可能数の上限値
constexpr u32 GLOBAL_HEAP_DESCRIPTOR_NUM = 100000;

/**
 * @brief ローカルヒープの登録ID
 * @details
 * できるだけ細かく分けたほうが各ヒープの必要量が抑えられるため、シーンごと、もしくはシチュエーションごとに分けるのを想定
 */
enum class LocalHeapID : u32 {
  //! 常に存在するヒープ
  GLOBAL_ID = 0,
  GAME_OVER,
  TITLE,

  MODEL_VIEW_SCENE = 100,
  //! マルチレンダーターゲットテストシーン用
  MULTI_RENDER_TARGET_TEST_SCENE,
  PERSPECTIVE_CAMERA_TEST_SCENE,
  PHYSICS_TEST,
  POST_PROCESS_VIEWER,
  SOUND_TEST,
  SPRITE_RENDER_TEST,
};

namespace local {
/**
 * @brief 定義済みのローカルIDに対応したディスクリプタ数を取得する
 */
inline constexpr u32 GetDefinedLocalDescriptorNum(LocalHeapID id) {
  switch (id) {
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::
        GLOBAL_ID:
      return 20000;
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::
        GAME_OVER:
      return 10000;
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::TITLE:
      return 10000;
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::
        MODEL_VIEW_SCENE:
      return 100;
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::
        MULTI_RENDER_TARGET_TEST_SCENE:
      return 100;
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::
        PERSPECTIVE_CAMERA_TEST_SCENE:
      return 100;
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::
        PHYSICS_TEST:
      return 100;
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::
        POST_PROCESS_VIEWER:
      return 100;
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::
        SOUND_TEST:
      return 100;
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::
        SPRITE_RENDER_TEST:
      return 100;
    default:
      MY_LOG(L"未定義のIDが選択されました。");
      break;
  }

  return 0;
}
}  // namespace local
}  // namespace heap_parameter
}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_PARAMETER_H_
