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
  GLOBAL_DATA = 0,      //! グローバル情報
  TRANSFORM = 1,        //! トランスフォーム情報
  WORLD_CONTEXT = 2,    //! ビュー、プロジェクション情報
  UV_RECT = 3,          //! UV情報
  COLOR = 4,            //! 色情報
  GAUGE_PARAMETER = 5,  //! UIのゲージ情報
  PARTICLE_INFO = 5,
};
}  // namespace ConstantBufferRegisterID

/**
 * @brief テクスチャのレジスター番号
 */
namespace TextureRegisterID {
enum Enum : u32 {
  ALBEDO = 0,  //! アルベド情報
  NORMAL = 1,  //! ノーマルマップ

  G_BUFFER_WORLD_POSITION = 0,
  G_BUFFER_WORLD_NORMAL = 1,
  G_BUFFER_WORLD_DIFFUSE = 2,
};
}  // namespace TextureRegisterID

namespace UnorderedAccessID {
enum Enum : u32 {
  Particle = 0,  //! GPUパーティクルの構造体配列バッファ
};
}  // namespace UnorderedAccessID

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
