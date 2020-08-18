#ifndef LEGEND_UTIL_LOADER_ARCHIVE_LOADER_H
#define LEGEND_UTIL_LOADER_ARCHIVE_LOADER_H

/**
 * @file archive_loader.h
 * @brief archive�t�@�C������f�[�^�𕜌�����
 */
namespace legend {
namespace util {
namespace loader {

/**
 * @brief �A�[�J�C�u�̊e���ڂ̃f�[�^
 */
struct Entity {
 public:
  //! �擪����̓ǂݎ��ʒu
  i32 position;
  //! 1���ڂ̃f�[�^�̑傫��
  i32 size;
  /**
   * @brief �R���X�g���N�^
   */
  Entity(i32 position, i32 size) : position(position), size(size) {}
  /**
   * @brief �f�X�g���N�^
   */
  ~Entity() {}
};

/**
 * @brief �A�[�J�C�u����f�[�^��ǂݎ��N���X
 */
class ArchiveLoader {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  ArchiveLoader();
  /**
   * @brief �f�X�g���N�^
   */
  ~ArchiveLoader();
  /**
   * @brief ������
   * @param filepath �A�[�J�C�u�t�@�C���ւ̃p�X
   */
  bool Init(const std::filesystem::path& filepath);
  /**
   * @brief �A�[�J�C�u�̊e���ڂ�ǂݎ��A�o�C�i���f�[�^�Ƃ��ĕԂ�
   * @param filename ���ږ��ƂȂ��Ă���t�@�C����(assets����̑��΃p�X)
   */
  std::vector<u8> Load(const std::filesystem::path& filename);
  /**
   * @brief �I������
   */
  void Finalize();

 private:
  //! �ǂݍ��񂾃t�@�C���}�b�v
  //! �ǂݍ��ݎ���path���󂯎�邪�Apath�͓����\����������̂ł�hash�l����v���Ȃ����߁Awstring�ɕϊ����ĕۑ�����
  std::unordered_map<std::wstring, Entity> load_files_;
  //! �A�[�J�C�u�̃t�@�C���X�g���[��
  std::ifstream ifs_;
};

}  // namespace loader
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_LOADER_ARCHIVE_LOADER_H
