#ifndef LEGEND_UTIL_RESOURCE_TEXTURE_H_
#define LEGEND_UTIL_RESOURCE_TEXTURE_H_

/**
 * @file texture.h
 * @brief �e�N�X�`�����\�[�X�Ǘ��N���X��`
 */

#include "src/directx/buffer/texture_2d.h"
#include "src/util/resource/resource_id.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @class Texture
 * @brief �e�N�X�`�����\�[�X�Ǘ��N���X
 */
class Texture
    : public ResourceManager<id::Texture,
                             std::shared_ptr<directx::buffer::Texture2D>> {
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
   * @param key ���\�[�X����ӂɓ��肷��ID
   * @param filepath �t�@�C���ւ̃p�X
   * @return �ǂݍ��݂ɐ���������true��Ԃ�
   */
  bool Load(id::Texture key, const std::filesystem::path& filepath) override {
    MY_ASSERTION(
        false,
        L"���̊֐����g�p��������Load(id::Texture, const filesystem::path&,"
        L"u32,directx::descriptor_heap::heap_parameter::LocalHeapID)"
        L"�̂ق����g�p���ĉ������B");
    return false;
  }
  /**
   * @brief ���\�[�X��ǂݍ���
   * @param key ���\�[�X����ӂɓ��肷��ID
   * @param filepath �t�@�C���ւ̃p�X
   * @param register_num ���W�X�^�[�̓o�^�ԍ�
   * @param heap_id ���[�J���q�[�v��ID
   * @return �ǂݍ��݂ɐ���������true��Ԃ�
   */
  bool Load(id::Texture key, const std::filesystem::path& filepath,
            u32 register_num,
            directx::descriptor_heap::heap_parameter::LocalHeapID heap_id);
};

}  // namespace resource
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_TEXTURE_H_
