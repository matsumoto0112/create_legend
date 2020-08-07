#ifndef LEGEND_DIRECTX_BUFFER_COMMITTED_RESOURCE_H_
#define LEGEND_DIRECTX_BUFFER_COMMITTED_RESOURCE_H_

/**
 * @file buffer.h
 * @brief �o�b�t�@�Ǘ��N���X��`
 */

#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {
namespace buffer {

/**
 * @class CommittedResource
 * @brief CreateCommittedResource�ō쐬����郊�\�[�X�Ǘ��N���X
 */
class CommittedResource {
 public:
  /**
   * @brief �o�b�t�@�f�X�N
   */
  struct BufferDesc {
    //! ���\�[�X��
    std::wstring name;
    //! �o�b�t�@�̑傫��
    u64 buffer_size;
    //! ���\�[�X�̏������
    D3D12_RESOURCE_STATES init_states;
  };

  /**
   * @brief �e�N�X�`���o�b�t�@�f�X�N
   */
  struct Tex2DDesc {
    //! ���\�[�X��
    std::wstring name;
    //! �e�N�X�`���t�H�[�}�b�g
    DXGI_FORMAT format;
    //! �e�N�X�`����
    u32 width;
    //! �e�N�X�`������
    u32 height;
    //! ���\�[�X�̎g�p�t���O
    D3D12_RESOURCE_FLAGS flags;
    //! ���������K�v�Ȏ��̏������l
    D3D12_CLEAR_VALUE clear_value;
    //! ���\�[�X�̏������
    D3D12_RESOURCE_STATES init_states;
  };

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
   * @brief ��Ԃ�S�ă��Z�b�g����
   */
  void Reset();
  /**
   * @brief �o�b�t�@�Ƃ��ď���������
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param desc �o�b�t�@�f�X�N
   */
  bool InitAsBuffer(device::IDirectXAccessor& accessor, const BufferDesc& desc);
  /**
   * @brief 2D�e�N�X�`���Ƃ��ď���������
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param desc �e�N�X�`���f�X�N
   */
  bool InitAsTex2D(device::IDirectXAccessor& accessor, const Tex2DDesc& desc);
  /**
   * @brief �o�b�t�@���R�s�[���ď���������
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param buffer �R�s�[���̃o�b�t�@
   * @param init_states ���\�[�X�̏������
   * @param name ���\�[�X��
   */
  bool InitFromBuffer(device::IDirectXAccessor& accessor,
                      ComPtr<ID3D12Resource> buffer,
                      D3D12_RESOURCE_STATES init_states,
                      const std::wstring& name);
  /**
   * @brief ��Ԃ�J�ڂ�����
   * @param command_list �R�}���h���X�g
   * @param next_state ���̏��
   */
  void Transition(device::CommandList& command_list,
                  D3D12_RESOURCE_STATES next_state);

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
  /**
   * @brief �o�b�t�@�T�C�Y���擾����
   */
  u64 GetBufferSize() const { return buffer_size_; }

 public:
  /**
   * @brief �e�N�X�`���Ȃǂɗ��p����2�̃��\�[�X�𗘗p�����R�s�[������
   * @param command_list �R�}���h���X�g
   * @param dest_resource�@�������ݐ�̃��\�[�X
   * @param immediate_resource �ꎞ�I�ȃ��\�[�X
   * @param data �������ރf�[�^
   * @param row 1�s�̃������T�C�Y (width * sizeof(Pixel))
   * @param slice �S�̂̃������T�C�Y (row * height)
   */
  static void UpdateSubresource(device::CommandList& command_list,
                                CommittedResource* dest_resource,
                                CommittedResource* immediate_resource,
                                const void* data, u64 row, u64 slice);

 private:
  //! �Ǘ����Ă��郊�\�[�X
  ComPtr<ID3D12Resource> resource_;
  //! �o�b�t�@�̃������T�C�Y
  u64 buffer_size_;
  //! ���\�[�X�̏��
  D3D12_RESOURCE_STATES current_state_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_COMMITTED_RESOURCE_H_
