#ifndef LEGEND_DIRECTX_BUFFER_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_BUFFER_H_

/**
 * @file buffer.h
 * @brief �o�b�t�@�Ǘ��N���X��`
 */

#include "src/directx/directx12_device.h"

namespace legend {
namespace directx {
namespace buffer {
/**
 * @brief CreateCommittedResource�ō쐬����郊�\�[�X�Ǘ��N���X
 */
class CommittedResource {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  CommittedResource();
  /**
   * @brief �f�X�g���N�^
   */
  ~CommittedResource();
  /**
   * @brief �o�b�t�@�Ƃ��ď���������
   * @param device DirectX12�f�o�C�X
   * @param buffer_size �o�b�t�@�̃������T�C�Y
   * @param name ���\�[�X��
   * @return �������ɐ���������true��Ԃ�
   */
  bool InitAsBuffer(DirectX12Device& device, u32 buffer_size,
                    const std::wstring& name);
  /**
   * @brief ���\�[�X���擾����
   */
  ID3D12Resource* GetResource() const { return resource_.Get(); }
  /**
   * @brief ���\�[�X�Ƀf�[�^����������
   * @param data �������ރf�[�^
   * @return �������݂ɐ���������true��Ԃ�
   */
  bool WriteResource(const void* data);

 private:
  //! �Ǘ����Ă��郊�\�[�X
  ComPtr<ID3D12Resource> resource_;
  //! �o�b�t�@�̃������T�C�Y
  u32 buffer_size_;
  //! ���\�[�X�̏��
  D3D12_RESOURCE_STATES current_state_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_BUFFER_H_
