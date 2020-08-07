#ifndef LEGEND_DIRECTX_DEVICE_DIRECTX_ACCESSOR_H_
#define LEGEND_DIRECTX_DEVICE_DIRECTX_ACCESSOR_H_

/**
 * @file directx_accessor.h
 * @brief DirectXアクセサ関数定義
 */

#include "src/directx/descriptor_heap/descriptor_handle.h"
#include "src/directx/descriptor_heap/heap_parameter.h"
#include "src/directx/shader/shader_resource_type.h"

namespace legend {
namespace directx {
namespace device {

/**
 * @brief DirectXアクセサ関数インターフェース
 */
class IDirectXAccessor {
 public:
  /**
   * @brief デストラクタ
   */
  virtual ~IDirectXAccessor() = default;
  /**
   * @brief デバイスの取得
   */
  virtual ID3D12Device* GetDevice() const = 0;
  /**
   * @brief レンダーターゲットのハンドルを取得する
   */
  virtual descriptor_heap::DescriptorHandle GetRTVHandle() = 0;
  /**
   * @brief デプス・ステンシルのハンドルを取得する
   */
  virtual descriptor_heap::DescriptorHandle GetDSVHandle() = 0;
  /**
   * @brief ローカルハンドルを取得する
   * @param heap_id 使用するローカルハンドルのID
   */
  virtual descriptor_heap::DescriptorHandle GetLocalHandle(
      descriptor_heap::heap_parameter::LocalHeapID heap_id) = 0;
  /**
   * @brief ハンドルをグローバルに登録する
   * @param register_num シェーダーのレジスター番号
   * @param type リソースの種類
   * @param handle 登録するハンドル
   */
  virtual void RegisterHandle(u32 register_num, shader::ResourceType type,
                              descriptor_heap::DescriptorHandle handle) = 0;
};

}  // namespace device
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DEVICE_DIRECTX_ACCESSOR_H_
