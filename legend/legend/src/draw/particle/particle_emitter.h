#ifndef LEGEND_DRAW_PARTICLE_PARTICLE_EMITTER_H_
#define LEGEND_DRAW_PARTICLE_PARTICLE_EMITTER_H_

/**
 * @file particle_emitter.h
 * @brief �p�[�e�B�N���G�~�b�^�[���N���X��`
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/directx/device/command_list.h"
#include "src/directx/shader/pipeline_state.h"
#include "src/draw/particle/particle_manager.h"
#include "src/util/transform.h"

namespace legend {
namespace draw {
namespace particle {

/**
 * @class ParticleEmitter
 * @brief �p�[�e�B�N���G�~�b�^�[
 */
class ParticleEmitter {
 protected:
  using TransformStruct = directx::buffer::constant_buffer_structure::Transform;
  using TransformConstantBuffer =
      directx::buffer::ConstantBuffer<TransformStruct>;

 public:
  /**
   * @brief �R���X�g���N�^
   * @param particle_max_size �p�[�e�B�N���̍ő吔
   * @param particle_structure_size �p�[�e�B�N���̍\���̂̃T�C�Y
   * @param dispatch_x ��ƃX���b�h�O���[�v��(X)
   * @param dispatch_y ��ƃX���b�h�O���[�v��(Y)
   * @param name �p�[�e�B�N����
   */
  ParticleEmitter(u32 particle_max_size, u32 particle_structure_size,
                  u32 dispatch_x, u32 dispatch_y, const std::wstring& name);
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~ParticleEmitter();
  /**
   * @brief ������
   * @param command_list �o�b�t�@���R�s�[����̂Ɏg�p����R�}���h���X�g
   * @param data �R�s�[���̃f�[�^
   * @return �������ɐ���������true��Ԃ�
   */
  virtual bool Init(directx::device::CommandList& command_list,
                    const void* data,
                    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphics_desc,
                    D3D12_COMPUTE_PIPELINE_STATE_DESC compute_desc);
  /**
   * @brief �X�V����
   * @param compute_command_list �R���s���[�g�V�F�[�_�[�����s����R�}���h���X�g
   */
  virtual void Update(directx::device::CommandList& compute_command_list);
  /**
   * @brief �`�揈��
   * @param graphics_command_list �`�揈�������s����R�}���h���X�g
   */
  virtual void Render(directx::device::CommandList& graphics_command_list);
  /**
   * @brief �g�����X�t�H�[���̎Q�Ƃ�Ԃ�
   * @return
   */
  util::Transform& GetTransformRef() { return transform_; }

 protected:
  //! �p�[�e�B�N����
  const std::wstring name_;
  //! �p�[�e�B�N���̍ő吔
  const u32 particle_max_num_;
  //! �p�[�e�B�N���\���̂̃T�C�Y
  const u32 particle_structure_size_;
  //! ��ƃX���b�h�O���[�v��(X)
  const u32 dispatch_x_;
  //! ��ƃX���b�h�O���[�v��(Y)
  const u32 dispatch_y_;

 protected:
  directx::shader::PipelineState graphics_pipeline_state_;
  directx::shader::PipelineState compute_pipeline_state_;

  util::Transform transform_;
  TransformConstantBuffer transform_cb_;

  ComPtr<ID3D12Resource> particle_uav_;
  ComPtr<ID3D12Resource> particle_uav_upload_;
  directx::descriptor_heap::DescriptorHandle handle_;
  D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;
};

}  // namespace particle
}  // namespace draw
}  // namespace legend
#endif  //! LEGEND_DRAW_PARTICLE_PARTICLE_EMITTER_H_
