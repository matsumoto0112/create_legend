#ifndef LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_PARAMETER_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_PARAMETER_H_

/**
 * @file heap_parameter.h
 * @brief ヒープのデフォルトパラメータ定義ファイル
 * @details コンパイル時に決定できる、基本的に変更しない情報を定義する
 */

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

namespace local_heap_id {
enum Enum : u32 {
  //! 常に存在するヒープ
  STATIC = 0,
  //! マルチレンダーターゲットテストシーン用
  MULTI_RENDER_TARGET_TEST_SCENE,
};
}  // namespace local_heap_id

namespace local {
/**
 * @brief ローカルヒープの登録ID
 * @details
 * できるだけ細かく分けたほうが各ヒープの必要量が抑えられるため、シーンごと、もしくはシチュエーションごとに分けるのを想定
 */
u32 GetDefinedLocalDescriptorNum(local_heap_id::Enum id);
}  // namespace local
}  // namespace heap_parameter
}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_PARAMETER_H_
