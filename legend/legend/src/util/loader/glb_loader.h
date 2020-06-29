#ifndef LEGEND_UTIL_LOADER_GLB_LOADER_H_
#define LEGEND_UTIL_LOADER_GLB_LOADER_H_

#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/GLTF.h>

/**
 * @file glb_loader.h
 * @brief .glb�t�@�C����ǂݍ���
 */

namespace legend {
namespace util {
namespace loader {

/**
 * @brief �ǂݍ��񂾃��f���f�[�^
 */
struct LoadedMeshData {
  //! ���f����
  std::wstring name;
  std::vector<float> positions;
  std::vector<float> normals;
  std::vector<float> uvs;
  std::vector<u16> indices;
};

/**
 * @brief �ǂݍ���
 */
class StreamReader final : public Microsoft::glTF::IStreamReader {
 public:
  StreamReader();
  /**
   * @brief filename���o�C�i���`���œǂݍ���
   * @param filename �ǂݍ��ރt�@�C����
   * @return �X�g���[�����\�z���ĕԂ�
   */
  std::shared_ptr<std::istream> GetInputStream(
      const std::string& filename) const override;
};

/**
 * @brief GLB���f����ǂݍ���
 */
class GLBLoader {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  GLBLoader();
  /**
   * @brief �f�X�g���N�^
   */
  ~GLBLoader();
  /**
   * @brief �ǂݍ���
   * @param filename �ǂݍ��ރt�@�C����
   * @return �ǂݍ��񂾃f�[�^���烂�f���̏���Ԃ�
   * @details ����V���O�����b�V���������܂��擾�ł��܂���
   */
  LoadedMeshData Load(const std::filesystem::path& filename);

 private:
  //! �X�g���[���ǂݍ��݊�
  std::unique_ptr<StreamReader> stream_reader_;
};

}  // namespace loader
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_LOADER_GLB_LOADER_H_
