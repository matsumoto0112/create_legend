#ifndef LEGEND_UTIL_RESOURCE_TEXTURE_H_
#define LEGEND_UTIL_RESOURCE_TEXTURE_H_

/**
 * @file texture.h
 * @brief �e�N�X�`�����\�[�X�Ǘ��N���X��`
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
 * @brief �e�N�X�`�����\�[�X�Ǘ��N���X
 */
class Texture
    : public ResourceManager<std::shared_ptr<directx::buffer::Texture2D>> {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Texture();
  /**
   * @brief �f�X�g���N�^
   */
  ~Texture();
  /**
   * @brief ���\�[�X��ǂݍ���
   * @param name ���\�[�X��
   * @return �ǂݍ��݂ɐ���������true��Ԃ�
   */
  bool Load(const std::wstring& name) override;
};

}  // namespace resource
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_TEXTURE_H_
