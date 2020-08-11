#ifndef LEGEND_DIRECTX_SHADER_SHADER_REGISTER_ID_H_
#define LEGEND_DIRECTX_SHADER_SHADER_REGISTER_ID_H_

/**
 * @file shader_register_id.h
 * @brief シェーダーのレジスター番号定義
 */

namespace legend {
namespace directx {
namespace shader {
/**
 * @brief コンスタントバッファのレジスター番号
 */
namespace ConstantBufferRegisterID {
enum Enum : u32 {
  GLOBAL_DATA = 0,    //! グローバル情報
  TRANSFORM = 1,      //! トランスフォーム情報
  WORLD_CONTEXT = 2,  //! ビュー、プロジェクション情報
  UV_RECT = 3,
};

}  // namespace ConstantBufferRegisterID

/**
 * @brief テクスチャのレジスター番号
 */
namespace TextureRegisterID {
enum Enum : u32 {
  ALBEDO = 0,  //! アルベド情報
};
}  // namespace TextureRegisterID

/**
 * @brief サンプラーのレジスター番号
 */
namespace SamplerRegisterID {
enum Enum : u32 {
  WARP = 0,
  CLAMP = 1,
};
}  // namespace SamplerRegisterID

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_SHADER_REGISTER_ID_H_
