#ifndef LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_

/**
 * @file constant_buffer.h
 * @brief �R���X�^���g�o�b�t�@�N���X��`
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/descriptor_heap/descriptor_handle.h"
#include "src/directx/directx_accessor.h"
#include "src/math/math_util.h"

namespace legend {
namespace directx {
namespace buffer {

/**
 * @class ConstantBuffer
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
   * @brief ��Ԃ�S�ă��Z�b�g����
   */
  void Reset();
  /**
   * @brief ������
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   * @param register_num �V�F�[�_�[�̃��W�X�^�[�ԍ�
   * @param name ���\�[�X��
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(IDirectXAccessor& accessor, u32 register_num,
            const std::wstring& name);

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
  void SetToHeap(IDirectXAccessor& accessor) const;

 private:
  /**
   * @brief �������݊J�n
   */
  bool WriteStart();
  /**
   * @brief �������ݏI��
   */
  void WriteEnd();

 private:
  //! ���\�[�X
  CommittedResource resource_;
  //! �n���h��
  DescriptorHandle resource_handle_;
  //! �R���X�^���g�o�b�t�@�\����
  T staging_;
  //! CPU��̃��\�[�X�̏������ݗ̈�
  void* resource_begin_;
  //! �V�F�[�_�[�̃��W�X�^�[�ԍ�
  u32 register_num_;
  //! �A���C�����g���ꂽ�o�b�t�@�T�C�Y
  u32 buffer_aligned_size_;
};

//�R���X�g���N�^
template <class T>
inline ConstantBuffer<T>::ConstantBuffer()
    : resource_{},
      resource_handle_{},
      staging_{},
      resource_begin_{nullptr},
      register_num_{0},
      buffer_aligned_size_{0} {}

//�f�X�g���N�^
template <class T>
inline ConstantBuffer<T>::~ConstantBuffer() {
  Reset();
}

template <class T>
inline void ConstantBuffer<T>::Reset() {
  WriteEnd();

  resource_.Reset();
  resource_handle_ = DescriptorHandle{};
  staging_ = T{};
  u32 buffer_aligned_size_ = 0;
  register_num_ = 0;
}

//������
template <class T>
inline bool ConstantBuffer<T>::Init(IDirectXAccessor& accessor,
                                    u32 register_num,
                                    const std::wstring& name) {
  Reset();

  buffer_aligned_size_ = math::util::AlignPow2(
      sizeof(T), D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
  if (!resource_.InitAsBuffer(accessor, buffer_aligned_size_, name)) {
    return false;
  }

  this->register_num_ = register_num;
  this->resource_handle_ = accessor.GetHandle(DescriptorHeapType::CBV_SRV_UAV);

  D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc = {};
  cbv_desc.BufferLocation = resource_.GetResource()->GetGPUVirtualAddress();
  cbv_desc.SizeInBytes = buffer_aligned_size_;
  accessor.GetDevice()->CreateConstantBufferView(
      &cbv_desc, this->resource_handle_.cpu_handle_);

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
inline void ConstantBuffer<T>::SetToHeap(IDirectXAccessor& accessor) const {
  accessor.SetToGlobalHeap(this->register_num_, ResourceType::Cbv,
                           this->resource_handle_);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_
