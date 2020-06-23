#ifndef LEGEND_UTIL_PATH_H_
#define LEGEND_UTIL_PATH_H_

/**
 * @file path.h
 * @brief �p�X�Ǘ��N���X��`
 */

#include "src/util/singleton.h"

namespace legend {
namespace util {
/**
 * @brief �p�X�Ǘ�
 */
class Path : public Singleton<Path> {
 public:
  /**
   * @brief ���s�t�@�C���̂���p�X���擾����
   */
  std::filesystem::path exe() const { return exe_filepath_; }
  /**
   * @brief �V�F�[�_�[�t�@�C���̂���t�H���_�̃p�X���擾����
   * @return
   */
  std::filesystem::path shader() const { return shader_filepath_; }

 protected:
  Path();
  ~Path();

 private:
  std::filesystem::path exe_filepath_;
  std::filesystem::path shader_filepath_;
};

}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_PATH_H_
