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
 * @brief ���f��ID
 */
enum class Model {
  CHECK_XYZ,
  KARI,
  OBJECT_1000CM,
  ERASER,
  DESK,
  PLANE,
};

/**
 * @brief �e�N�X�`��ID
 */
enum class Texture {
  TEX,
};

/**
 * @enum Pipeline
 * @brief �p�C�v���C������ӂɓ��肷��ID
 */
enum class Pipeline {
  MODEL_VIEW,
  MULTI_RENDER_TARGET_TEST,
  MULTI_RENDER_TARGET_TEST_PP,
  SIMPLE_PARTICLE,
  OBJECT_WIREFRAME,
  GRAFFITI,
};

/**
 * @brief ���_�V�F�[�_�[ID
 */
enum class VertexShader {
  SPRITE,
  MODEL_VIEW,
  MULTI_RENDER_TARGET_TEST_PP,
  MULTI_RENDER_TARGET_TEST,
  OBB,
  SIMPLE_PARTICLE,
  GRAFFITI,
};

/**
 * @brief �s�N�Z���V�F�[�_�[ID
 */
enum class PixelShader {
  SPRITE,
  MODEL_VIEW,
  MULTI_RENDER_TARGET_TEST_PP,
  MULTI_RENDER_TARGET_TEST,
  OBB,
  SIMPLE_PARTICLE,
  GRAFFITI,
};

}  // namespace id
}  // namespace resource
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_RESOURCE_ID_H_
