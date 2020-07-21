#ifndef LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_PARAMETER_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_PARAMETER_H_

namespace legend {
namespace directx {
namespace descriptor_heap {

//ヒープのデフォルトパラメータ
namespace parameter {

//! アプリケーション全体でのディスクリプタヒープの最大数
//! CBV,SRV,UAVで共通する割り当て可能数の上限値
constexpr u32 GLOBAL_HEAP_DESCRIPTOR_NUM = 100000;
}  // namespace parameter
}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_PARAMETER_H_
