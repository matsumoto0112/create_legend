#ifndef LEGEND_DIRECTX_DIRECTX_ACCESSOR_H_
#define LEGEND_DIRECTX_DIRECTX_ACCESSOR_H_

/**
 * @file directx_accessor.h
 * @brief DirectX�n�̃A�N�Z�T�֐��C���^�[�t�F�[�X��`
 */

namespace legend {
namespace directx {

/**
 * @brief DirectX�A�N�Z�T�֐��C���^�[�t�F�[�X
 */
class IDirectXAccessor {
 public:
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~IDirectXAccessor() = default;
  /**
   * @brief �f�o�C�X�̎擾
   */
  virtual ID3D12Device* GetDevice() const = 0;
  /**
   * @brief �R�}���h���X�g�̎擾
   */
  virtual ID3D12GraphicsCommandList4* GetCommandList() const = 0;
};
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DIRECTX_ACCESSOR_H_