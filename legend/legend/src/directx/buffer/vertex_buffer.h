#ifndef LEGEND_DIRECTX_BUFFER_VERTEX_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_VERTEX_BUFFER_H_

/**
 * @file vertex_buffer_h
 * @brief ���_�o�b�t�@��`
 */

#include "src/directx/directx12_device.h"

namespace legend {
namespace directx {
namespace buffer {
/**
 * @brief ���_�o�b�t�@
 */
class VertexBuffer {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  VertexBuffer();
  /**
   * @brief �f�X�g���N�^
   */
  ~VertexBuffer();
  /**
   * @brief ������
   * @param device �f�o�C�X
   * @param vertex_size ���_�\���̂̃������T�C�Y
   * @param vertex_num ���_��
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(DirectX12Device& device, u32 vertex_size, u32 vertex_num);
  /**
   * @brief ���\�[�X�Ƀf�[�^���������ޏ�������
   * @param vertices �������ޒ��_�f�[�^
   * @return �������݂ɐ���������true��Ԃ�
   */
  template <class T>
  bool WriteBufferResource(const std::vector<T>& vertices);
  /**
   * @brief �R�}���h���X�g�ɃZ�b�g����
   */
  void SetGraphicsCommandList(DirectX12Device& device);

 protected:
  //! ���_���\�[�X
  ComPtr<ID3D12Resource> vertex_buffer_resource_;
  //! ���_�o�b�t�@�r���[
  D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;
};

//���\�[�X�ɏ�������
template <class T>
inline bool VertexBuffer::WriteBufferResource(const std::vector<T>& vertices) {
  util::debug::Assertion(
      vertices.size() * sizeof(T) == vertex_buffer_view_.SizeInBytes,
      L"vertices size is incorrect.");

  void* data_begin;
  if (FAILED(vertex_buffer_resource_->Map(0, nullptr, &data_begin))) {
    return false;
  }

  memcpy_s(data_begin, vertex_buffer_view_.SizeInBytes, vertices.data(),
           vertex_buffer_view_.SizeInBytes);
  vertex_buffer_resource_->Unmap(0, nullptr);
  return true;
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  // !LEGEND_DIRECTX_BUFFER_VERTEX_BUFFER_H_
