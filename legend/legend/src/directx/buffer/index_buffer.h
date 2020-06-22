#ifndef LEGEND_DIRECTX_BUFFER_INDEX_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_INDEX_BUFFER_H_

/**
 * @file index_buffer.h
 * @brief �C���f�b�N�X�o�b�t�@��`
 */

#include "src/directx/directx12_device.h"

namespace legend {
namespace directx {
namespace buffer {

/**
 * @brief �C���f�b�N�X�o�b�t�@�N���X
 */
class IndexBuffer {
  using Index = u16;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  IndexBuffer();
  /**
   * @brief �f�X�g���N�^
   */
  ~IndexBuffer();
  /**
   * @brief ������
   * @param device DirectX12�f�o�C�X
   * @param index_num �C���f�b�N�X��
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(DirectX12Device& device, u32 index_num);
  /**
   * @brief �o�b�t�@�Ƀ��\�[�X����������
   * @param indices �C���f�b�N�X�z��
   * @return �������݂ɐ���������true��Ԃ�
   */
  bool WriteBufferResource(const std::vector<Index>& indices);
  /**
   * @brief �O���t�B�b�N�X�R�}���h���X�g�ɒǉ�����
   * @param device DirectX12�f�o�C�X
   */
  void SetGraphicsCommandList(DirectX12Device& device);
  /**
   * @brief �`��
   * @param device DirectX12�f�o�C�X
   */
  void Draw(DirectX12Device& device);

 private:
  //! �C���f�b�N�X�o�b�t�@���\�[�X
  ComPtr<ID3D12Resource> index_buffer_resource_;
  //! �o�b�t�@�r���[
  D3D12_INDEX_BUFFER_VIEW index_buffer_view_;
  //! �C���f�b�N�X��
  u32 index_num_;
};
}  // namespace buffer
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_BUFFER_INDEX_BUFFER_H_
