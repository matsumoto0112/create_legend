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
constexpr u32 GLOBAL_HEAP_DESCRIPTOR_NUM = 500000;
//! RTVで使用する割り当て可能数
constexpr u32 RTV_HEAP_DESCRIPTOR_NUM = 100;
//! DSVで使用する割り当て可能数
constexpr u32 DSV_HEAP_DESCRIPTOR_NUM = 6;

/**
 * @brief ローカルヒープの登録ID
 * @details
 * ヒープを確保するたびに領域が減っていくので1ゲームごとにリセットしないとそのうち足りなくなる。
 * そのため、常に存在するヒープ（モデルのテクスチャデータなど）と、1ゲームごとのヒープ（敵のトランスフォームコンスタントバッファなど）に分ける必要がある
 */
enum class LocalHeapID : u32 {
  GLOBAL_ID = 0,  //! 常に存在するヒープ
  ONE_PLAY = 1,   //! 1ゲームごとにリセットされる
};

namespace local {

//未定義の時のディスクリプタ割り当て数
constexpr u32 UNDEFINED_DESCRIPTOR_NUM = UINT_LEAST32_MAX;

/**
 * @brief 定義済みのローカルIDに対応したディスクリプタ数を取得する
 */
inline constexpr u32 GetDefinedLocalDescriptorNum(LocalHeapID id) {
  switch (id) {
    case LocalHeapID::GLOBAL_ID:
      return 20000;
    case LocalHeapID::ONE_PLAY:
      return 20000;
    default:
      MY_LOG(L"未定義のIDが選択されました。");
      break;
  }

  return UNDEFINED_DESCRIPTOR_NUM;
}
}  // namespace local
}  // namespace heap_parameter
}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_PARAMETER_H_
