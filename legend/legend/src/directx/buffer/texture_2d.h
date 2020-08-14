#ifndef LEGEND_DIRECTX_BUFFER_TEXTURE_2D_H_
#define LEGEND_DIRECTX_BUFFER_TEXTURE_2D_H_

/**
 * @file texture_2d.h
 * @brief 2D�e�N�X�`���N���X��`
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {
namespace buffer {

/**
 * @class Texture2D
 * @brief 2D�e�N�X�`���N���X
 */
class Texture2D {
 public:
  /**
   * @struct Desc
   * @brief �e�N�X�`���f�X�N
   */
  struct Desc {
    //! �t�H�[�}�b�g
    DXGI_FORMAT format;
    //! ��
    u32 width;
    //! ����
    u32 height;
    //! �n���h��
    descriptor_heap::DescriptorHandle handle;
    //! ���\�[�X��
    std::wstring name;
  };

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
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param desc �e�N�X�`���f�X�N
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(device::IDirectXAccessor& accessor, const Desc& desc);
  /**
   * @brief �e�N�X�`����ǂݍ��݂���������
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param command_list �R�}���h���X�g
   * @param filename �t�@�C����
   * @param handle �e�N�X�`���n���h��
   * @return �������ɐ���������true��Ԃ�
   * @details ���\�[�X�̃R�s�[�̂��߂ɃR�}���h���X�g���K�v
   */
  bool InitAndWrite(device::IDirectXAccessor& accessor,
                    device::CommandList& command_list,
                    const std::filesystem::path& filename,
                    const descriptor_heap::DescriptorHandle& handle);
  /**
   * @brief �e�N�X�`�����������݂���������
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param command_list �R�}���h���X�g
   * @param format �e�N�X�`���̃t�H�[�}�b�g
   * @param data �e�N�X�`���f�[�^
   * @param handle �e�N�X�`���n���h��
   * @param filename ���\�[�X��
   * @return �������ɐ���������true��Ԃ�
   * @details ���\�[�X�̃R�s�[�̂��߂ɃR�}���h���X�g���K�v
   */
  bool InitAndWrite(device::IDirectXAccessor& accessor,
                    device::CommandList& command_list, DXGI_FORMAT format,
                    const std::vector<u8>& data,
                    const descriptor_heap::DescriptorHandle& handle,
                    const std::wstring& name);
  /**
   * @brief �e�N�X�`���f�[�^����������
   * @param command_list �R�}���h���X�g
   * @param data �e�N�X�`���f�[�^
   */
  void WriteResource(device::CommandList& command_list, const void* data);

  /**
   * @brief �q�[�v�Ɏ��g��ǉ�����
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param register_num �V�F�[�_�[�̃��W�X�^�[�ԍ�
   */
  void RegisterHandle(device::IDirectXAccessor& accessor, u32 register_num);

 public:
  /**
   * @brief �e�N�X�`���̕����擾����
   */
  inline u32 GetWidth() const { return width_; }
  /**
   * @brief �e�N�X�`���̍������擾����
   */
  inline u32 GetHeight() const { return height_; }

 private:
  /**
   * @brief �e�N�X�`���o�b�t�@������������
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param desc �e�N�X�`���f�X�N
   * @return �������ɐ���������true��Ԃ�
   */
  bool InitTexBuffer(device::IDirectXAccessor& accessor, const Desc& desc);

 private:
  //! �e�N�X�`���f�[�^
  buffer::CommittedResource texture_;
  //! �R�s�[�p�̃e�N�X�`�����\�[�X
  buffer::CommittedResource texture_immediate_;
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
