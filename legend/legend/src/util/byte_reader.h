#ifndef LEGEND_UTIL_BYTE_READER_H_
#define LEGEND_UTIL_BYTE_READER_H_

/**
 * @file byte_reader.h
 * @brief �o�C�g�f�[�^�ǂݍ��݊֐���`
 */

namespace legend {
namespace util {
namespace byte_reader {
/**
 * @brief �o�C�g�f�[�^�̓ǂݍ���
 * @param filepath �t�@�C���ւ̃p�X
 * @return �ǂݍ��񂾃f�[�^
 */
std::vector<u8> Read(const std::filesystem::path& filepath);
}  // namespace byte_reader
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_BYTE_READER_H_
