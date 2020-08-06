#ifndef LEGEND_UTIL_RESOURCE_MODEL_H_
#define LEGEND_UTIL_RESOURCE_MODEL_H_

/**
 * @file model.h
 * @brief �ǂݍ��񂾃��f�����Ǘ�����N���X��`
 */

#include "src/draw/model.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @enum ModelID
 * @brief ���f������ӂɓ��肷��ID
 */
enum class ModelID {
  CHECK_XYZ,
  KARI,
  OBJECT_1000CM,
  ERASER,
  DESK,
};

/**
 * @class Model
 * @brief ���f�����\�[�X�Ǘ��N���X
 */
class Model : public ResourceManager<ModelID, std::shared_ptr<draw::Model>> {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Model();
  /**
   * @brief �f�X�g���N�^
   */
  ~Model();
  /**
   * @brief ���\�[�X��ǂݍ���
   * @param key ���f������ӂɓ��肷��ID
   * @param filepath �t�@�C���ւ̃p�X
   * @return �ǂݍ��݂ɐ���������true��Ԃ�
   */
  bool Load(ModelID key, const std::filesystem::path& filepath,
            directx::device::CommandList& command_list);
};

}  // namespace resource
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_MODEL_H_
