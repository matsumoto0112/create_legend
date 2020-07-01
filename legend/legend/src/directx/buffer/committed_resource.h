#ifndef LEGEND_DIRECTX_BUFFER_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_BUFFER_H_

/**
 * @file buffer.h
 * @brief �o�b�t�@�Ǘ��N���X��`
 */

#include "src/directx/directx_accessor.h"

namespace legend {
namespace directx {
namespace buffer {
/**
 * @brief CreateCommittedResource�ō쐬����郊�\�[�X�Ǘ��N���X
 */
class CommittedResource {
 public:
  struct TextureBufferDesc {
    std::wstring name;
    DXGI_FORMAT format;
    u32 width;
    u32 height;
    D3D12_RESOURCE_FLAGS flags;
    D3D12_CLEAR_VALUE clear_value;

    TextureBufferDesc(const std::wstring& name = L"",
                      DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN,
                      u32 width = 0, u32 height = 0,
                      D3D12_RESOURCE_FLAGS flags =
                          D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE,
                      D3D12_CLEAR_VALUE clear_value = CD3DX12_CLEAR_VALUE())
        : name(name),
          format(format),
          width(width),
          height(height),
          flags(flags),
          clear_value(clear_value){};
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
   * @brief �o�b�t�@�Ƃ��ď���������
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   * @param buffer_size �o�b�t�@�̃������T�C�Y
   * @param name ���\�[�X��
   * @return �������ɐ���������true��Ԃ�
   */
  bool InitAsBuffer(IDirectXAccessor& accessor, u64 buffer_size,
                    const std::wstring& name);
  bool InitAsTex2D(IDirectXAccessor& accessor, const TextureBufferDesc& desc);
  bool InitFromBuffer(IDirectXAccessor& accessor, ComPtr<ID3D12Resource> buffer,
                      D3D12_RESOURCE_STATES first_state);
  /**
   * @brief ��Ԃ�J�ڂ�����
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   * @param next_state ���̏��
   */
  void Transition(IDirectXAccessor& accessor, D3D12_RESOURCE_STATES next_state);

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
   * @brief �e�N�X�`���Ȃǂɗ��p����2��
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   * @param dest_resource�@�������ݐ�̃��\�[�X
   * @param immediate_resource �ꎞ�I�ȃ��\�[�X
   * @param data �������ރf�[�^
   * @param row 1�s�̃������T�C�Y (width * sizeof(Pixel))
   * @param slice �S�̂̃������T�C�Y (row * height)
   */
  static void UpdateSubresource(IDirectXAccessor& accessor,
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
};  // namespace buffer

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_BUFFER_H_
