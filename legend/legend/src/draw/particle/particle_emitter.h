#ifndef LEGEND_DRAW_PARTICLE_PARTICLE_EMITTER_H_
#define LEGEND_DRAW_PARTICLE_PARTICLE_EMITTER_H_

/**
 * @file particle_emitter.h
 * @brief �p�[�e�B�N���G�~�b�^�[���N���X��`
 */

#include "assets/shaders/gpu_particle/gpu_particle.h"
#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/directx/device/command_list.h"
#include "src/directx/shader/pipeline_state.h"
#include "src/util/timer.h"
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
  using ParticleInfoConstantBuffer =
      directx::buffer::ConstantBuffer<shader::gpu_particle::ParticleInfo>;

 public:
  struct ParticleConstData {
    u32 particle_max_size;
    u32 particle_structure_size;
    u32 dispatch_x;
    u32 dispatch_y;
    std::wstring name;
  };

 public:
  /**
   * @brief �R���X�g���N�^
   * @param particle_max_size �p�[�e�B�N���̍ő吔
   * @param particle_structure_size �p�[�e�B�N���̍\���̂̃T�C�Y
   * @param dispatch_x ��ƃX���b�h�O���[�v��(X)
   * @param dispatch_y ��ƃX���b�h�O���[�v��(Y)
   * @param name �p�[�e�B�N����
   */
  ParticleEmitter(const ParticleConstData& const_data);
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~ParticleEmitter();
  /**
   * @brief ������
   * @param command_list �o�b�t�@���R�s�[����̂Ɏg�p����R�}���h���X�g
   * @param data �R�s�[���̃f�[�^
   * @param graphics_desc �O���t�B�b�N�X�p�C�v���C���X�e�[�g�f�B�X�N
   * @param compute_desc �R���s���[�g�p�C�v���C���X�e�[�g�f�B�X�N
   * @return �������ɐ���������true��Ԃ�
   */
  virtual bool Init(directx::device::CommandList& command_list,
                    const void* data, const std::wstring& texture_name,
                    const D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphics_desc,
                    const D3D12_COMPUTE_PIPELINE_STATE_DESC& compute_desc);
  /**
   * @brief �X�V����
   * @param command_list �p�[�e�B�N���̍X�V�����p�̃R�}���h���X�g
   */
  virtual void Update(directx::device::CommandList& command_list);
  /**
   * @brief �`�揈��
   * @param graphics_command_list �`�揈�������s����R�}���h���X�g
   */
  virtual void Render(directx::device::CommandList& graphics_command_list);

  void SetTransform(const util::Transform& transform) {
    this->transform_ = transform;
  }
  util::Transform GetTransform() const { return transform_; }
  /**
   * @brief �g�����X�t�H�[���̎Q�Ƃ�Ԃ�
   */
  util::Transform& GetTransformRef() { return transform_; }

  void SetTextureName(std::wstring_view view);

  void SetEnableUpdate(bool enable_update) {
    this->enable_update_ = enable_update;
  }
  void SetEnableRender(bool enable_render) {
    this->enable_render_ = enable_render;
  }
  void ResetParticle() { this->reset_particle_ = true; }
  void SetEmitEnable(bool emit_enable) { this->emit_enable_ = emit_enable; }
  void Delete();
  void Delete(float second);
  bool StandDeleteFlag() const { return delete_; }

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
  //! �O���t�B�b�N�X�p�C�v���C��
  directx::shader::PipelineState graphics_pipeline_state_;
  //! �R���s���[�g�p�C�v���C��
  directx::shader::PipelineState compute_pipeline_state_;

  //! �g�����X�t�H�[��
  util::Transform transform_;
  //! �g�����X�t�H�[���R���X�^���g�o�b�t�@
  TransformConstantBuffer transform_cb_;
  shader::gpu_particle::ParticleInfo info_;
  ParticleInfoConstantBuffer info_cb_;
  std::wstring texture_name_;

  //! �p�[�e�B�N���f�[�^��UAV�����_�o�b�t�@
  ComPtr<ID3D12Resource> particle_uav_;
  //! �p�[�e�B�N���f�[�^�̃A�b�v���[�h�p�o�b�t�@
  ComPtr<ID3D12Resource> particle_uav_upload_;
  //! UAV�n���h��
  directx::descriptor_heap::DescriptorHandle handle_;
  //! ���_�o�b�t�@�r���[
  D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;

  bool enable_update_;
  bool enable_render_;
  bool reset_particle_;
  bool emit_enable_;
  bool delete_;
  //! �폜�O�ɑҋ@������p�̃^�C�}�[
  //! �����폜�����computeshader�N�����ɍ폜����鋰�ꂪ���邽�߁A�����x�点��p�̃^�C�}�[
  util::CountDownTimer delete_wait_timer_;
  util::CountDownTimer delete_timer_;
};

}  // namespace particle
}  // namespace draw
}  // namespace legend
#endif  //! LEGEND_DRAW_PARTICLE_PARTICLE_EMITTER_H_
