#ifndef LEGEND_DRAW_PARTICLE_PARTICLE_COMMAND_LIST_H_
#define LEGEND_DRAW_PARTICLE_PARTICLE_COMMAND_LIST_H_

/**
 * @file particle_command_list.h
 * @brief �p�[�e�B�N���p�R�}���h���X�g��`
 */

#include "src/directx/device/directx_accessor.h"
#include "src/directx/device/directx_device.h"
#include "src/directx/frame_resource.h"
#include "src/draw/particle/particle_emitter.h"

namespace legend {
namespace draw {
namespace particle {

/**
 * @brief �p�[�e�B�N���p�R�}���h���X�g�N���X
 */
class ParticleCommandList {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  ParticleCommandList();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~ParticleCommandList();
  /**
   * @brief ������
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param frame_count �o�b�N�o�b�t�@�̃t���[����
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(directx::device::IDirectXAccessor& accessor, u32 frame_count);

  template <class T, class... Args>
  std::shared_ptr<T> CreateParticle(directx::device::CommandList& command_list,
                                    Args... args);

  /**
   * @brief �p�[�e�B�N���̍X�V
   */
  void UpdateParticles();
  /**
   * @brief �p�[�e�B�N���̕`��
   */
  void RenderParticle(directx::device::CommandList& render_command_list);
  /**
   * @brief �t���[���J�n������
   * @param device DirectX�f�o�C�X
   */
  void BeginFrame(directx::device::DirectXDevice& device);

 private:
  std::vector<std::shared_ptr<ParticleEmitter>> particle_emitters_;
  //! �t���[�����\�[�X
  std::vector<directx::FrameResource> frame_resources_;
  //! ���݃t���[���̃��\�[�X
  directx::FrameResource* current_frame_resource_;
  //! �R�}���h�L���[
  ComPtr<ID3D12CommandQueue> command_queue_;
  //! �R�}���h�t�F���X
  ComPtr<ID3D12Fence> fence_;
  //! �t�F���X�l
  u64 fence_value_;
};
template <class T, class... Args>
inline std::shared_ptr<T> ParticleCommandList::CreateParticle(
    directx::device::CommandList& command_list, Args... args) {
  auto res = std::make_shared<T>();
  res->Init(command_list, args...);
  particle_emitters_.emplace_back(res);
  return res;
}

}  // namespace particle
}  // namespace draw
}  // namespace legend
#endif  //! LEGEND_DRAW_PARTICLE_PARTICLE_COMMAND_LIST_H_
