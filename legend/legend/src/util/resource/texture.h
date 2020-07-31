#ifndef LEGEND_UTIL_RESOURCE_TEXTURE_H_
#define LEGEND_UTIL_RESOURCE_TEXTURE_H_

/**
 * @file texture.h
 * @brief テクスチャリソース管理クラス定義
 */

#include "src/directx/buffer/texture_2d.h"
#include "src/util/resource/resource_id.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @class Texture
 * @brief テクスチャリソース管理クラス
 */
class Texture
    : public ResourceManager<id::Texture,
                             std::shared_ptr<directx::buffer::Texture2D>> {
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
   * @param key リソースを一意に特定するID
   * @param filepath ファイルへのパス
   * @return 読み込みに成功したらtrueを返す
   */
  bool Load(id::Texture key, const std::filesystem::path& filepath) override {
    MY_ASSERTION(
        false,
        L"この関数を使用する代わりにLoad(id::Texture, const filesystem::path&,"
        L"u32,directx::descriptor_heap::heap_parameter::LocalHeapID)"
        L"のほうを使用して下さい。");
    return false;
  }
  /**
   * @brief リソースを読み込む
   * @param key リソースを一意に特定するID
   * @param filepath ファイルへのパス
   * @param register_num レジスターの登録番号
   * @param heap_id ローカルヒープのID
   * @return 読み込みに成功したらtrueを返す
   */
  bool Load(id::Texture key, const std::filesystem::path& filepath,
            u32 register_num,
            directx::descriptor_heap::heap_parameter::LocalHeapID heap_id);
};

}  // namespace resource
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_TEXTURE_H_
