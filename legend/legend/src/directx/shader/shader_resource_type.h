#ifndef LEGEND_DIRECTX_SHADER_SHADER_RESOURCE_TYPE_H_
#define LEGEND_DIRECTX_SHADER_SHADER_RESOURCE_TYPE_H_

/**
 * @file shader_resource_type.h
 * @brief �V�F�[�_�[���\�[�X�̎�ޒ�`
 */

namespace legend {
namespace directx {
namespace shader {

/**
 * @enum ResourceType
 * @brief ���\�[�X�̎��
 */
enum class ResourceType : u32 {
  CBV,
  SRV,
  UAV,
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_SHADER_RESOURCE_TYPE_H_
