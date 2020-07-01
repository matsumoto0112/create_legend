#ifndef LEGEND_DIRECTX_DESCRIPTOR_HEAP_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HEAP_H_

/**
 * @file descriptor_heap.h
 * @brief �f�B�X�N���v�^�q�[�v��`
 */

#include "src/directx/descriptor_handle.h"
#include "src/directx/directx_accessor.h"

namespace legend {
namespace directx {
/**
 * @brief �q�[�v�̎��
 */
enum class HeapType {
  CBV_SRV_UAV,
  Sampler,
  RTV,
  DSV,
};

/**
 * @brief �q�[�v�̃t���O
 */
enum class HeapFlag {
  None,
  ShaderVisible,
};

/**
 * @brief �f�B�X�N���v�^�q�[�v�̃��b�v�N���X
 */
class DescriptorHeap {
 public:
  /**
   * @brief �f�B�X�N���v�^�q�[�v�f�B�X�N
   */
  struct Desc {
    //! �q�[�v��
    std::wstring name;
    //! �g�p����f�B�X�N���v�^�q�[�v�̐�
    u32 descriptor_num;
    //! �q�[�v�̎��
    HeapType type;
    //! �q�[�v�t���O
    HeapFlag flag;

    Desc(const std::wstring name = std::wstring(), u32 descriptor_num = 0,
         HeapType type = HeapType::CBV_SRV_UAV, HeapFlag flag = HeapFlag::None)
        : name(name), descriptor_num(descriptor_num), type(type), flag(flag) {}
  };

 public:
  /**
   * @brief �R���X�g���N�^
   */
  DescriptorHeap();
  /**
   * @brief �f�X�g���N�^
   */
  ~DescriptorHeap();
  /**
   * @brief ������
   * @param device DirectX12�f�o�C�X
   * @param desc �쐬����q�[�v�̐ݒ�
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(IDirectXAccessor& device, const Desc& desc);
  /**
   * @brief CPU,GPU�n���h�����擾����
   * @param index �n���h���C���f�b�N�X
   * @return �L���ȃn���h����Ԃ�
   */
  DescriptorHandle GetHandle(u32 index) const;
  /**
   * @brief �q�[�v���擾����
   * @details
   * �����̃q�[�v�𓯎��Ɏg�p����Ƃ��A�܂Ƃ߂ēn���K�v������̂ł��̃N���X�ŃZ�b�g�͂��Ȃ����߃q�[�v���擾����֐������J����
   */
  ID3D12DescriptorHeap* GetHeap() const { return heap_.Get(); }

 private:
  //! �q�[�v
  ComPtr<ID3D12DescriptorHeap> heap_;
  //! �q�[�v�̒P�ʃT�C�Y
  u32 heap_size_;
};

}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_H_
