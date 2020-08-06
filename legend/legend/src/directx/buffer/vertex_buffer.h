#ifndef LEGEND_DIRECTX_BUFFER_VERTEX_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_VERTEX_BUFFER_H_

/**
 * @file vertex_buffer_h
 * @brief ���_�o�b�t�@��`
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {
namespace buffer {

/**
 * @class VertexBuffer
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
   */
  bool Init(device::IDirectXAccessor& accessor, u32 vertex_size, u32 vertex_num,
            const std::wstring& name);
  /**
   * @brief ���\�[�X�Ƀf�[�^���������ޏ�������
   */
  bool WriteBufferResource(const void* data);
  /**
   * @brief �R�}���h���X�g�ɃZ�b�g����
   */
  void SetGraphicsCommandList(device::CommandList& command_list);

 protected:
  //! ���_���\�[�X
  CommittedResource resource_;
  //! ���_�o�b�t�@�r���[
  D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  // !LEGEND_DIRECTX_BUFFER_VERTEX_BUFFER_H_
