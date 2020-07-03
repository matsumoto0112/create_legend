#ifndef LEGEND_DIRECTX_BUFFER_INDEX_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_INDEX_BUFFER_H_

/**
 * @file index_buffer.h
 * @brief �C���f�b�N�X�o�b�t�@��`
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/directx_accessor.h"
#include "src/directx/primitive_topology.h"

namespace legend {
namespace directx {
namespace buffer {

/**
 * @class IndexBuffer
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
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   * @param index_num �C���f�b�N�X��
   * @param topology �v���~�e�B�u�̌`��
   * @param name ���\�[�X��
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(IDirectXAccessor& accessor, u32 index_num,
            PrimitiveTopology topology, const std::wstring& name);
  /**
   * @brief �������Ə������݂𓯎��ɂ���
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   * @param indices �C���f�b�N�X�z��
   * @param topology �v���~�e�B�u�̌`��
   * @param name ���\�[�X��
   * @return �������A�������݂ɐ���������true��Ԃ�
   */
  bool InitAndWrite(IDirectXAccessor& accessor,
                    const std::vector<Index>& indices,
                    PrimitiveTopology topology, const std::wstring& name);
  /**
   * @brief �o�b�t�@�Ƀ��\�[�X����������
   * @param indices �C���f�b�N�X�z��
   * @return �������݂ɐ���������true��Ԃ�
   */
  bool WriteBufferResource(const std::vector<Index>& indices);
  /**
   * @brief �O���t�B�b�N�X�R�}���h���X�g�ɒǉ�����
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   */
  void SetGraphicsCommandList(IDirectXAccessor& accessor);
  /**
   * @brief �`��
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   */
  void Draw(IDirectXAccessor& accessor);

 private:
  //! �C���f�b�N�X�o�b�t�@�̃��\�[�X�f�[�^
  CommittedResource resource_;
  //! �o�b�t�@�r���[
  D3D12_INDEX_BUFFER_VIEW index_buffer_view_;
  //! �C���f�b�N�X��
  u32 index_num_;
  //! �v���~�e�B�u�`��
  D3D12_PRIMITIVE_TOPOLOGY primitive_toporogy_;
};
}  // namespace buffer
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_BUFFER_INDEX_BUFFER_H_
