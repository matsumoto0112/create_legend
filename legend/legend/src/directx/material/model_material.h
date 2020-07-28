#ifndef LEGEND_DIRECTX_MATERIAL_MODEL_MATERIAL_H_
#define LEGEND_DIRECTX_MATERIAL_MODEL_MATERIAL_H_

/**
 * @file model_material.h
 * @brief model_view.hlsl�g�p�̒ʏ탂�f���`��p�}�e���A����`
 */

#include "src/directx/material/material.h"

namespace legend {
namespace directx {
namespace material {

/**
 * @brief model_view.hlsl�g�p�̒ʏ탂�f���`��p�}�e���A��
 */
class ModelMaterial : public IMaterial {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  ModelMaterial();
  /**
   * @brief �f�X�g���N�^
   */
  ~ModelMaterial();
  /**
   * @brief ������
   * @return �������ɐ���������true��Ԃ�
   */
  virtual bool Init() override;
  /**
   * @brief �O���t�B�b�N�R�}���h���X�g�ɃZ�b�g����
   */
  virtual void SetToGraphicsCommandList() override;
};

}  // namespace material
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_MATERIAL_MODEL_MATERIAL_H_
