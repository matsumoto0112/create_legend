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
  GLBLoader();
  /**
   * @brief �f�X�g���N�^
   */
  ~GLBLoader();
  /**
   * @brief ���f����ǂݍ���
   * @param filename �t�@�C����
   * @return ����������true��Ԃ�
   */
  bool Load(const std::filesystem::path& filename);
  /**
   * @brief ���_�����擾����
   * @param mesh_index ���b�V���ԍ�
   * @param primitive_index �v���~�e�B�u�ԍ�
   */
  u32 GetVertexNum(u32 mesh_index = 0, u32 primitive_index = 0) const;
  /**
   * @brief ���_���W�z����擾����
   * @param mesh_index ���b�V���ԍ�
   * @param primitive_index �v���~�e�B�u�ԍ�
   */
  std::vector<float> GetPosition(u32 mesh_index = 0,
                                 u32 primitive_index = 0) const;
  /**
   * @brief �꒸�_���W�̗v�f�����擾����
   * @param mesh_index ���b�V���ԍ�
   * @param primitive_index �v���~�e�B�u�ԍ�
   */
  u32 GetPositionComponentSize(u32 mesh_index = 0,
                               u32 primitive_index = 0) const;
  /**
   * @brief �@���z����擾����
   * @param mesh_index ���b�V���ԍ�
   * @param primitive_index �v���~�e�B�u�ԍ�
   */
  std::vector<float> GetNormal(u32 mesh_index = 0,
                               u32 primitive_index = 0) const;
  /**
   * @brief ��@���̗v�f�����擾����
   * @param mesh_index ���b�V���ԍ�
   * @param primitive_index �v���~�e�B�u�ԍ�
   */
  u32 GetNormalComponentSize(u32 mesh_index = 0, u32 primitive_index = 0) const;
  /**
   * @brief UV���W�z����擾����
   * @param mesh_index ���b�V���ԍ�
   * @param primitive_index �v���~�e�B�u�ԍ�
   */
  std::vector<float> GetUV(u32 mesh_index = 0, u32 primitive_index = 0) const;
  /**
   * @brief ����W�̗v�f�����擾����
   * @param mesh_index ���b�V���ԍ�
   * @param primitive_index �v���~�e�B�u�ԍ�
   */
  u32 GetUVComponentSize(u32 mesh_index = 0, u32 primitive_index = 0) const;
  /**
   * @brief �ڐ��z����擾����
   * @param mesh_index ���b�V���ԍ�
   * @param primitive_index �v���~�e�B�u�ԍ�
   */
  std::vector<float> GetTangent(u32 mesh_index = 0,
                                u32 primitive_index = 0) const;
  /**
   * @brief ��ڐ��̗v�f�����擾����
   * @param mesh_index ���b�V���ԍ�
   * @param primitive_index �v���~�e�B�u�ԍ�
   */
  u32 GetTangentComponentSize(u32 mesh_index = 0,
                              u32 primitive_index = 0) const;
  /**
   * @brief �C���f�b�N�X�z����擾����
   * @param mesh_index ���b�V���ԍ�
   * @param primitive_index �v���~�e�B�u�ԍ�
   */
  std::vector<u16> GetIndex(u32 mesh_index = 0, u32 primitive_index = 0) const;
  /**
   * @brief �A���x�h�e�N�X�`�����擾����
   */
  std::vector<u8> GetAlbedo() const;
  std::vector<u8> GetNormalMap() const;

 private:
  /**
   * @brief ���b�V�����擾����
   * @param mesh_index ���b�V���ԍ�
   * @param primitive_index �v���~�e�B�u�ԍ�
   */
  const Microsoft::glTF::MeshPrimitive& GetMesh(u32 mesh_index,
                                                u32 primitive_index) const;
  /**
   * @brief �A�N�Z�T�����݂���΂�����擾����
   * @param mesh �Ώۃ��b�V��
   * @param accessor_name �A�N�Z�T��
   * @param accessor �擾�����A�N�Z�T�i�߂�l�j
   * @return �A�N�Z�T�����݂�����true��Ԃ�
   */
  bool GetAccessor(const Microsoft::glTF::MeshPrimitive& mesh,
                   const char* accessor_name,
                   Microsoft::glTF::Accessor* accessor) const;
  /**
   * @brief �A�N�Z�T���ɑΉ������A�g���r���[�g���擾����
   * @param mesh �Ώۃ��b�V��
   * @param accessor_name �A�N�Z�T��
   * @return �A�N�Z�T���ɑΉ������v�f���A���݂��Ȃ���΋�̔z���Ԃ�
   */
  std::vector<float> GetAttribute(const Microsoft::glTF::MeshPrimitive& mesh,
                                  const char* accessor_name) const;
  /**
   * @brief �A�N�Z�T���ɑΉ������A�g���r���[�g�̗v�f�����擾����
   * @param mesh �Ώۃ��b�V��
   * @param accessor_name �A�N�Z�T��
   * @return �A�N�Z�T���ɑΉ������v�f���A���݂��Ȃ����0��Ԃ�
   */
  u32 GetAttributeComponentSize(const Microsoft::glTF::MeshPrimitive& mesh,
                                const char* accessor_name) const;

 private:
  //! �X�g���[���ǂݍ��݊�
  std::unique_ptr<StreamReader> stream_reader_;
  //! �o�C�i���ǂݍ���
  std::unique_ptr<Microsoft::glTF::GLBResourceReader> glb_resource_reader_;
  //! �f�V���A���C�Y���ꂽ�f�[�^
  Microsoft::glTF::Document document_;
};

}  // namespace loader
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_LOADER_GLB_LOADER_H_
