#ifndef LEGEND_UTIL_RESOURCE_RESOURCE_ID_H_
#define LEGEND_UTIL_RESOURCE_RESOURCE_ID_H_

/**
 * @file resource_id.h
 */
namespace legend {
namespace util {
namespace resource {
namespace id {

/**
 * @brief テクスチャID
 */
enum class Texture {
  TEX,
};

/**
 * @enum Pipeline
 * @brief パイプラインを一意に特定するID
 */
enum class Pipeline {
  MODEL_VIEW,
  MULTI_RENDER_TARGET_TEST,
  MULTI_RENDER_TARGET_TEST_PP,
  SIMPLE_PARTICLE,
};

/**
 * @brief 頂点シェーダーID
 */
enum class VertexShader {
  SPRITE,
  MODEL_VIEW,
  MULTI_RENDER_TARGET_TEST_PP,
  MULTI_RENDER_TARGET_TEST,
  OBB,
  SIMPLE_PARTICLE,
};

/**
 * @brief ピクセルシェーダーID
 */
enum class PixelShader {
  SPRITE,
  MODEL_VIEW,
  MULTI_RENDER_TARGET_TEST_PP,
  MULTI_RENDER_TARGET_TEST,
  OBB,
  SIMPLE_PARTICLE,
};

}  // namespace id
}  // namespace resource
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_RESOURCE_ID_H_
