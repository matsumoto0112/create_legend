#ifndef LEGEND_DIRECTX_BUFFER_TEXTURE_2D_H_
#define LEGEND_DIRECTX_BUFFER_TEXTURE_2D_H_

/**
 * @file texture_2d.h
 * @brief 2D�e�N�X�`���N���X��`
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/directx12_device.h"

namespace legend {
namespace directx {
namespace buffer {
/**
 * @brief 2D�e�N�X�`���N���X
 */
class Texture2D {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Texture2D();
  /**
   * @brief �f�X�g���N�^
   */
  ~Texture2D();
  /**
   * @brief ������
   * @param device DirectX12�f�o�C�X
   * @param register_num �V�F�[�_�[�̃��W�X�^�[�ԍ�
   * @param format �e�N�X�`���̃t�H�[�}�b�g
   * @param width �e�N�X�`���̕�
   * @param height �e�N�X�`���̍���
   * @param name ���\�[�X��
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(DirectX12Device& device, u32 register_num, DXGI_FORMAT format,
            u32 width, u32 height, const std::wstring& name);
  /**
   * @brief �e�N�X�`���f�[�^����������
   * @param device DirectX12�f�o�C�X
   * @param data �e�N�X�`���f�[�^
   */
  void WriteResource(DirectX12Device& device, const void* data);

  /**
   * @brief �q�[�v�Ɏ��g��ǉ�����
   * @param device DirectX12�f�o�C�X
   */
  void SetToHeap(DirectX12Device& device);

 private:
  //! �e�N�X�`���f�[�^
  buffer::CommittedResource texture_;
  //! �R�s�[�p�̃e�N�X�`�����\�[�X
  buffer::CommittedResource texture_immediate_;
  //! �V�F�[�_�[�̃��W�X�^�[�ԍ�
  u32 register_num_;
  //! �e�N�X�`���̃t�H�[�}�b�g
  DXGI_FORMAT format_;
  //! �e�N�X�`���̕�
  u32 width_;
  //! �e�N�X�`���̍���
  u32 height_;
  //! �V�F�[�_�[���\�[�X�r���[�f�X�N
  D3D12_SHADER_RESOURCE_VIEW_DESC srv_view_;
  //! CPU�n���h��
  D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle_;
  //! GPU�n���h��
  D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_TEXTURE_2D_H_