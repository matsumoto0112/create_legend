#ifndef LEGEND_DIRECTX_BUFFER_INDEX_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_INDEX_BUFFER_H_

/**
 * @file index_buffer.h
 * @brief �C���f�b�N�X�o�b�t�@��`
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"
#include "src/directx/primitive_topology.h"

namespace legend {
namespace directx {
namespace buffer {

/**
 * @class IndexBuffer
 * @brief �C���f�b�N�X�o�b�t�@�N���X
 */
class IndexBuffer {
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
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param index_size �C���f�b�N�X�z��̑傫��
   * @param index_num �C���f�b�N�X�̃o�C�g�T�C�Y
   * @param topology �v���~�e�B�u�̌`��
   * @param name ���\�[�X��
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(device::IDirectXAccessor& accessor, u32 index_size, u32 index_num,
            PrimitiveTopology topology, const std::wstring& name);
  /**
   * @brief �o�b�t�@�Ƀ��\�[�X����������
   * @param data �������ރf�[�^
   * @return �������݂ɐ���������true��Ԃ�
   */
  bool WriteBufferResource(const void* data);
  /**
   * @brief �R�}���h���X�g�ɃZ�b�g����
   * @param command_list �R�}���h���X�g
   */
  void SetGraphicsCommandList(device::CommandList& command_list);
  /**
   * @brief �`��
   * @param command_list �R�}���h���X�g
   */
  void Draw(device::CommandList& command_list);

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
