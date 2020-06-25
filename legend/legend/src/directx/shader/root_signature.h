#ifndef LEGEND_DIRECTX_SHADER_ROOT_SIGNATURE_H_
#define LEGEND_DIRECTX_SHADER_ROOT_SIGNATURE_H_

/**
 * @file root_signature.h
 * @brief ���[�g�V�O�l�`���N���X��`
 * @details
 ���̃N���X�Œ�`���郋�[�g�V�O�l�`���͂��ׂẴV�F�[�_�[�Ŏg���܂킹��悤�Ƀf�B�X�N���v�^���ő���Ɋm�ۂ���B
 �����đ��x�I�ȈӖ��Ō����I�ȍ\���Ƃ͌����Ȃ����A�V�F�[�_�[���Ƃɍ쐬�����胁�����������ǂ��B
 �ő���̑��x���o�����߂Ȃ�΁A�V�F�[�_�[���ƂɃ`���[�j���O�������[�g�V�O�l�`�������ق����ǂ����A���̕��쐬���ԁE�������������B
 */

#include "src/directx/directx12_device.h"

namespace legend {
namespace directx {
namespace shader {
/**
 * @brief ���[�g�V�O�l�`���N���X
 */
class RootSignature {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  RootSignature();
  /**
   * @brief �f�X�g���N�^
   */
  ~RootSignature();
  /**
   * @brief ������
   * @param device DirectX12�f�o�C�X
   * @param name ���\�[�X��
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(DirectX12Device& device, const std::wstring& name);
  /**
   * @brief ���[�g�V�O�l�`�����擾����
   */
  inline ID3D12RootSignature* GetRootSignature() const {
    return root_signature_.Get();
  }

  /**
   * @brief �R�}���h���X�g�ɃZ�b�g����
   * @param device DirectX12�f�o�C�X
   */
  void SetGraphicsCommandList(DirectX12Device& device);

 private:
  //! ���[�g�V�O�l�`��
  ComPtr<ID3D12RootSignature> root_signature_;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_SHADER_ROOT_SIGNATURE_H_
