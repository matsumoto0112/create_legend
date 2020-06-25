#ifndef LEGEND_DIRECTX_BUFFER_VERTEX_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_VERTEX_BUFFER_H_

/**
 * @file vertex_buffer_h
 * @brief ���_�o�b�t�@��`
 */

#include "src/directx/buffer/committed_resource.h"
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
   * @param name ���\�[�X��
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(DirectX12Device& device, u32 vertex_size, u32 vertex_num,
            const std::wstring& name);
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
  CommittedResource resource_;
  //! ���_�o�b�t�@�r���[
  D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;
};

//���\�[�X�ɏ�������
template <class T>
inline bool VertexBuffer::WriteBufferResource(const std::vector<T>& vertices) {
  MY_ASSERTION(vertices.size() * sizeof(T) == vertex_buffer_view_.SizeInBytes,
               L"vertices size is incorrect.");

  return resource_.WriteResource(vertices.data());
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  // !LEGEND_DIRECTX_BUFFER_VERTEX_BUFFER_H_
