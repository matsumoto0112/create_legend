#ifndef LEGEND_DIRECTX_RESOURCE_TYPE_H_
#define LEGEND_DIRECTX_RESOURCE_TYPE_H_

/**
 * @file resource_type.h
 * @brief ���\�[�X�̎�ޒ�`
 */

namespace legend {
namespace directx {
/**
 * @brief ���\�[�X�̎��
 */
enum class ResourceType : u32 {
  Cbv,
  Srv,
  Uav,
};

}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RESOURCE_TYPE_H_
