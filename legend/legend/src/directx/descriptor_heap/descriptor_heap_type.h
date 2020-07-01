#ifndef LEGEND_DIRECTX_DESCRIPTOR_HEAP_TYPE_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HEAP_TYPE_H_

/**
 * @file descriptor_heap_type.h
 * @brief �f�B�X�N���v�^�q�[�v��ޒ�`
 */

namespace legend {
namespace directx {

/**
 * @brief �f�B�X�N���v�^�q�[�v�̎��
 */
enum class DescriptorHeapType {
  CBV_SRV_UAV,
  RTV,
  DSV,
  SAMPLER,
};

/**
 * @brief �f�B�X�N���v�^�q�[�v�̎g�p�t���O
 */
enum class DescriptorHeapFlag {
  NONE,
  SHADER_VISIBLE,
};
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_TYPE_H_
