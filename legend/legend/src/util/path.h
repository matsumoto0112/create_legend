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
   */
  std::filesystem::path shader() const { return shader_filepath_; }
  /**
   * @brief �e�N�X�`���̂���t�H���_�̃p�X���擾����
   */
  std::filesystem::path texture() const { return texture_filepath_; }
  /**
   * @brief ���f���̂���t�H���_�̃p�X���擾����
   */
  std::filesystem::path model() const { return model_filepath_; }
  /**
   * @brief �R���t�B�O���̂���t�H���_�̃p�X���擾����
   */
  std::filesystem::path config() const { return config_filepath_; }

 protected:
  Path();
  virtual ~Path();

 private:
  //! ���s�t�@�C���̃p�X
  std::filesystem::path exe_filepath_;
  //! �V�F�[�_�[�t�@�C���̃p�X
  std::filesystem::path shader_filepath_;
  //! �e�N�X�`���̃t�@�C���p�X
  std::filesystem::path texture_filepath_;
  //! ���f���̃t�@�C���p�X
  std::filesystem::path model_filepath_;
  //! �R���t�B�O�ւ̃t�@�C���p�X
  std::filesystem::path config_filepath_;
};

}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_PATH_H_
