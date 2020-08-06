#ifndef LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_

/**
 * @file constant_buffer.h
 * @brief �R���X�^���g�o�b�t�@�N���X��`
 */

#include "src/directx/buffer/committed_resource.h"
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
   * @brief �R�s�[�R���X�g���N�^
   */
  ConstantBuffer(const ConstantBuffer& other);
  /**
   * @brief �R�s�[���Z�q
   */
  ConstantBuffer& operator=(const ConstantBuffer& other);
  /**
   * @brief ���[�u�R���X�g���N�^
   */
  ConstantBuffer(ConstantBuffer&& other) noexcept;
  /**
   * @brief ���[�u������Z�q
   */
  ConstantBuffer& operator=(ConstantBuffer&& other) noexcept;
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
  bool Init(device::IDirectXAccessor& accessor, u32 register_num,
            descriptor_heap::DescriptorHandle handle, const std::wstring& name);

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
  void SetToHeap(device::IDirectXAccessor& accessor) const;

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
  descriptor_heap::DescriptorHandle resource_handle_;
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
inline ConstantBuffer<T>::ConstantBuffer(const ConstantBuffer& other) {
  *this = other;
}

template <class T>
inline ConstantBuffer<T>& ConstantBuffer<T>::operator=(
    const ConstantBuffer& other) {
  if (this != &other) {
    this->resource_ = other.resource_;
    this->resource_handle_ = other.resource_handle_;
    this->staging_ = other.staging_;
    this->resource_begin_ = nullptr;
    this->register_num_ = other.register_num_;
    this->buffer_aligned_size_ = other.buffer_aligned_size_;

    if (this->resource_.GetResource()) {
      this->WriteStart();
    }
  }
  return *this;
}

template <class T>
inline ConstantBuffer<T>::ConstantBuffer(ConstantBuffer&& other) noexcept {
  *this = std::move(other);
}

template <class T>
inline ConstantBuffer<T>& ConstantBuffer<T>::operator=(
    ConstantBuffer&& other) noexcept {
  if (this != &other) {
    this->resource_ = other.resource_;
    this->resource_handle_ = other.resource_handle_;
    this->staging_ = other.staging_;
    this->resource_begin_ = nullptr;
    this->register_num_ = other.register_num_;
    this->buffer_aligned_size_ = other.buffer_aligned_size_;

    if (this->resource_.GetResource()) {
      this->WriteStart();
    }
  }
  return *this;
}

//���Z�b�g
template <class T>
inline void ConstantBuffer<T>::Reset() {
  WriteEnd();

  resource_.Reset();
  resource_handle_ = descriptor_heap::DescriptorHandle{};
  staging_ = T{};
  u32 buffer_aligned_size_ = 0;
  register_num_ = 0;
}

//������
template <class T>
inline bool ConstantBuffer<T>::Init(device::IDirectXAccessor& accessor,
                                    u32 register_num,
                                    descriptor_heap::DescriptorHandle handle,
                                    const std::wstring& name) {
  Reset();

  //�o�b�t�@�͈��̑傫���ɃA���C�������g����Ă���K�v������̂ŃA���C�������g�̌v�Z������
  buffer_aligned_size_ = math::util::AlignPow2(
      sizeof(T), D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);

  const CommittedResource::BufferDesc desc{
      name, buffer_aligned_size_,
      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ};
  if (!resource_.InitAsBuffer(accessor, desc)) {
    return false;
  }

  this->register_num_ = register_num;
  this->resource_handle_ = handle;

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
inline void ConstantBuffer<T>::SetToHeap(
    device::IDirectXAccessor& accessor) const {
  accessor.RegisterHandle(this->register_num_, shader::ResourceType::CBV,
                          this->resource_handle_);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_
