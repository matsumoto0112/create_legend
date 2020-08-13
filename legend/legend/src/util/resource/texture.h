#ifndef LEGEND_UTIL_RESOURCE_TEXTURE_H_
#define LEGEND_UTIL_RESOURCE_TEXTURE_H_

/**
 * @file texture.h
 * @brief テクスチャリソース管理クラス定義
 */

#include "src/directx/buffer/texture_2d.h"
#include "src/directx/descriptor_heap/heap_parameter.h"
#include "src/directx/device/command_list.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @class Texture
 * @brief テクスチャリソース管理クラス
 */
class Texture
    : public ResourceManager<std::shared_ptr<directx::buffer::Texture2D>> {
 public:
  /**
   * @brief コンストラクタ
   */
  Texture();
  /**
   * @brief デストラクタ
   */
  ~Texture();
  /**
   * @brief リソースを読み込む
   * @param name リソース名
   * @return 読み込みに成功したらtrueを返す
   */
  bool Load(const std::wstring& name) override;
};

}  // namespace resource
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_TEXTURE_H_
