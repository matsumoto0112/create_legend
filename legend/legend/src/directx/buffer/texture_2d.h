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
            u32 width, u32 height, descriptor_heap::DescriptorHandle handle,
            const std::wstring& name);
  /**
   * @brief �e�N�X�`����ǂݍ��݂���������
   * @param device DirectX12�f�o�C�X
   * @param register_num �V�F�[�_�[�̃��W�X�^�[�ԍ�
   * @param filename �t�@�C����
   * @return �������ɐ���������true��Ԃ�
   */
  bool InitAndWrite(DirectX12Device& device, u32 register_num,
                    const std::filesystem::path& filename,
                    descriptor_heap::DescriptorHandle handle);
  /**
   * @brief �e�N�X�`�����������݂���������
   * @param device DirectX12�f�o�C�X
   * @param register_num �V�F�[�_�[�̃��W�X�^�[�ԍ�
   * @param data �e�N�X�`���f�[�^
   * @param filename �t�@�C����
   * @return �������ɐ���������true��Ԃ�
   */
  bool InitAndWrite(DirectX12Device& device, u32 register_num,
                    const std::vector<u8>& data,
                    descriptor_heap::DescriptorHandle handle,
                    const std::wstring& name);
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
  /**
   * @brief ���W�X�^�[�ԍ����w�肵�ăq�[�v�Ɏ��g��ǉ�����
   * @param device DirectX12�f�o�C�X
   * @param overwrite_register_num �㏑�����郌�W�X�^�[�ԍ�
   */
  void SetToHeap(DirectX12Device& device, u32 overwrite_register_num);

 public:
  inline void SetWidth(u32 width) { this->width_ = width; }
  inline u32 GetWidth() const { return width_; }
  inline void SetHeight(u32 height) { this->height_ = height; }
  inline u32 GetHeight() const { return height_; }

 private:
  /**
   * @brief �e�N�X�`���o�b�t�@������������
   * @param device DirectX12�f�o�C�X
   * @param register_num �V�F�[�_�[�̃��W�X�^�[�ԍ�
   * @param format �e�N�X�`���̃t�H�[�}�b�g
   * @param width �e�N�X�`���̕�
   * @param height �e�N�X�`���̍���
   * @param name ���\�[�X��
   * @return �������ɐ���������true��Ԃ�
   */
  bool InitTexBuffer(DirectX12Device& device, u32 register_num,
                     DXGI_FORMAT format, u32 width, u32 height,
                     descriptor_heap::DescriptorHandle handle,
                     const std::wstring& name);

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
  //! �n���h��
  descriptor_heap::DescriptorHandle handle_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_TEXTURE_2D_H_
