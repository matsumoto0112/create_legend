#ifndef LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_

/**
 * @file constant_buffer.h
 * @brief �R���X�^���g�o�b�t�@�N���X��`
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/directx12_device.h"
#include "src/directx/heap_manager.h"
#include "src/math/math_util.h"

namespace legend {
namespace directx {
namespace buffer {

/**
 * @brief �R���X�^���g�o�b�t�@�N���X
 * @tparam T �R���X�^���g�o�b�t�@�Ƃ��Ďg�p����\����
 */
template <class T>
class ConstantBuffer {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  ConstantBuffer();
  /**
   * @brief �f�X�g���N�^
   */
  ~ConstantBuffer();
  /**
   * @brief ������
   * @param device DirectX12�f�o�C�X
   * @param register_num �V�F�[�_�[�̃��W�X�^�[�ԍ�
   * @param name ���\�[�X��
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(DirectX12Device& device, u32 register_num,
            const std::wstring& name);
  /**
   * @brief �������݊J�n
   */
  bool WriteStart();
  /**
   * @brief �������ݏI��
   */
  void WriteEnd();
  /**
   * @brief ���݂̃R���X�^���g�o�b�t�@�\���̂̎Q�Ƃ��擾����
   */
  T& GetStagingRef() { return staging_; }
  /**
   * @brief ���݂̏�Ԃ�GPU�ɍX�V����
   */
  void UpdateStaging() const;
  /**
   * @brief �q�[�v�Ɏ��g��ǉ�����
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   */
  void SetToHeap(DirectX12Device& device);

 public:
  //�R�s�[�E���[�u�֎~
  ConstantBuffer(const ConstantBuffer&) = delete;
  ConstantBuffer& operator=(const ConstantBuffer&) = delete;
  ConstantBuffer(ConstantBuffer&&) = delete;
  ConstantBuffer& operator=(ConstantBuffer&&) = delete;

 private:
  //! �R���X�^���g�o�b�t�@�\����
  T staging_;
  //! �A���C�����g���ꂽ�o�b�t�@�T�C�Y
  u32 buffer_aligned_size_;
  //! �V�F�[�_�[�̃��W�X�^�[�ԍ�
  u32 register_num_;
  //! ���\�[�X
  CommittedResource resource_;
  //! �R���X�^���g�o�b�t�@�r���[
  D3D12_CONSTANT_BUFFER_VIEW_DESC view_;
  //! CPU�n���h��
  D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle_;
  //! GPU�n���h��
  D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle_;
  //! CPU��̃��\�[�X�̏������ݗ̈�
  void* resource_begin_;
};

//�R���X�g���N�^
template <class T>
inline ConstantBuffer<T>::ConstantBuffer() {}

//�f�X�g���N�^
template <class T>
inline ConstantBuffer<T>::~ConstantBuffer() {
  WriteEnd();
}

//������
template <class T>
inline bool ConstantBuffer<T>::Init(DirectX12Device& device, u32 register_num,
                                    const std::wstring& name) {
  WriteEnd();

  buffer_aligned_size_ = math::util::AlignPow2(
      sizeof(T), D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
  if (!resource_.InitAsBuffer(device, buffer_aligned_size_, name)) {
    return false;
  }

  DescriptorHandle handle = device.GetHeapManager().GetLocalHandle();
  this->cpu_handle_ = handle.cpu_handle_;
  this->gpu_handle_ = handle.gpu_handle_;
  this->register_num_ = register_num;
  this->view_.BufferLocation = resource_.GetResource()->GetGPUVirtualAddress();
  this->view_.SizeInBytes = buffer_aligned_size_;
  device.GetDevice()->CreateConstantBufferView(&view_, this->cpu_handle_);

  return WriteStart();
}

//�������݊J�n
template <class T>
inline bool ConstantBuffer<T>::WriteStart() {
  if (resource_begin_) return true;

  if (FAILED(resource_.GetResource()->Map(0, nullptr, &resource_begin_))) {
    return false;
  }

  return true;
}

//�������ݏI��
template <class T>
inline void ConstantBuffer<T>::WriteEnd() {
  if (resource_begin_) {
    resource_.GetResource()->Unmap(0, nullptr);
    resource_begin_ = nullptr;
  }
}

//�\���̂̍X�V
template <class T>
inline void ConstantBuffer<T>::UpdateStaging() const {
  memcpy_s(resource_begin_, buffer_aligned_size_, &staging_, sizeof(T));
}

//�q�[�v�ɃZ�b�g����
template <class T>
inline void ConstantBuffer<T>::SetToHeap(DirectX12Device& device) {
  device.GetHeapManager().SetHandleToLocalHeap(this->register_num_,
                                          ResourceType::Cbv, this->cpu_handle_);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_
