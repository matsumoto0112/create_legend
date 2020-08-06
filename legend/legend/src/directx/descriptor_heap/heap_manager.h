#ifndef LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_MANAGER_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_MANAGER_H_

/**
 * @file heap_manager.h
 * @brief �q�[�v�Ǘ��N���X��`
 * @details �f�B�X�N���v�^�q�[�v�Ǘ��N���X�̎����ɂ���
 �O���[�o���q�[�v�ƃ��[�J���q�[�v�̓��ނ̃q�[�v���g�������A��������Ă���B
 �O���[�o���q�[�v�͎��ۂ�GPU�ɓ]�������q�[�v�A���[�J���q�[�v��CPU���݂̂Ŏg�p�����B
 �ʏ�ASRV,CBV�Ȃǂ��쐬����Ƃ��ɂ�CPU�AGPU�n���h���Ƃ��ɕK�v�ŁA���̒i�K����n���h����ύX�ł��Ȃ����߁A�C�ӂ̏��A�C�ӂ̐��]������̂�����ɂȂ��Ă���
 ���̂��߁ASRV�����Ƃ��ɂ̓��[�J���q�[�v�̃n���h�����g�p���A�����K�v�ɂȂ莟��O���[�o���ɃR�s�[���Ďg���헪������͑I�������B
 */

#include "src/directx/descriptor_heap/counting_descriptor_heap.h"
#include "src/directx/descriptor_heap/descriptor_heap.h"
#include "src/directx/descriptor_heap/heap_parameter.h"
#include "src/directx/device/command_list.h"
#include "src/directx/shader/shader_resource_type.h"

namespace legend {
namespace directx {
namespace descriptor_heap {

/**
 * @brief �q�[�v�Ǘ��N���X
 */
class HeapManager {
 private:
  using LocalHeapMap =
      std::unordered_map<heap_parameter::LocalHeapID, CountingDescriptorHeap>;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  HeapManager();
  /**
   * @brief �f�X�g���N�^
   */
  ~HeapManager();
  /**
   * @brief ������
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(device::IDirectXAccessor& accessor);
  /**
   * @brief �t���[���J�n���ɌĂ�
   */
  void BeginFrame();

  void SetGraphicsCommandList(device::CommandList& command_list) const;

  void RegisterHandle(u32 register_num, shader::ResourceType type,
                      DescriptorHandle handle);

  void UpdateGlobalHeap(ID3D12Device* device,
                        device::CommandList& command_list);
  bool AddLocalHeap(device::IDirectXAccessor& accessor,
                    heap_parameter::LocalHeapID heap_id);

  void RemoveLocalHeap(heap_parameter::LocalHeapID heap_id);

  DescriptorHandle GetLocalHeap(heap_parameter::LocalHeapID heap_id);

 public:
  /**
   * @brief �����_�[�^�[�Q�b�g�q�[�v���擾����
   */
  CountingDescriptorHeap* GetRtvHeap() { return &rtv_heap_; }
  /**
   * @brief �f�v�X�E�X�e���V���q�[�v���擾����
   */
  CountingDescriptorHeap* GetDsvHeap() { return &dsv_heap_; }
  ///**
  // * @brief ID���烍�[�J���q�[�v���擾����
  // * @param id ���[�J���q�[�v��ID
  // * @return ID���L���Ȃ�Ή��������[�J���q�[�v��Ԃ�
  // */
  // CountingDescriptorHeap* GetLocalHeap(heap_parameter::LocalHeapID id);

 private:
  //! �����_�[�^�[�Q�b�g�q�[�v
  CountingDescriptorHeap rtv_heap_;
  //! �f�v�X�E�X�e���V���q�[�v
  CountingDescriptorHeap dsv_heap_;
  //! �O���[�o���q�[�v
  DescriptorHeap global_heap_;
  //! �O���[�o���q�[�v�̌��݃t���[���ɂ����銄�蓖�Đ�
  u32 global_heap_allocated_count_;
  //! ���[�J���̃V�F�[�_�[���\�[�X�q�[�v
  LocalHeapMap local_heaps_;

  struct LocalHandles final {
    //! �R���X�^���g�o�b�t�@�̃n���h��
    std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> cbv_handles_;
    //! �V�F�[�_�[���\�[�X�̃n���h��
    std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> srv_handles_;
  } current_local_handles_;

  /**
   * @brief �󔒂̊��蓖�Ă�u��������f�t�H���g�n���h��
   */
  struct DefaultHandle final {
    DescriptorHandle default_srv_handle_;
    DescriptorHandle default_cbv_handle_;
  } default_handle_;
};

}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_MANAGER_H_
