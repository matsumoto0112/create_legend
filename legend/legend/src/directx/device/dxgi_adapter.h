#ifndef LEGEND_DIRECTX_DEVICE_DXGI_ADAPTER_H_
#define LEGEND_DIRECTX_DEVICE_DXGI_ADAPTER_H_

/**
 * @file dxgi_adapter.h
 * @brief DXGI�A�_�v�^�[�Ǘ��N���X��`
 */

#include "src/directx/device/device_option.h"

namespace legend {
namespace directx {
namespace device {
/**
 * @brief DXGI�A�_�v�^�[�N���X
 */
class DXGIAdapter {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  DXGIAdapter();
  /**
   * @brief �f�X�g���N�^
   */
  ~DXGIAdapter();
  /**
   * @brief ������
   * @param required_option �v������I�v�V����
   * @param adapter_id_override �㏑������A�_�v�^�[ID
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(DeviceOptionFlags required_option = DeviceOptionFlags::NONE,
            u32 adapter_id_override = UINT_MAX);

 public:
  /**
   * @brief �t�@�N�g���̎擾
   */
  IDXGIFactory4* GetFactory() const { return factory_.Get(); }
  /**
   * @brief �A�_�v�^�[�̎擾k
   */
  IDXGIAdapter1* GetAdapter() const { return adapter_.Get(); }
  /**
   * @brief ���ݗL���ȃI�v�V�����̎擾
   */
  DeviceOptionFlags GetOptions() const { return options_; }

 private:
  /**
   * @brief �A�_�v�^�[�̏�����
   * @param adapter_id_override �㏑������A�_�v�^�[ID
   * @param adapter �A�_�v�^�[�i�߂�l�j
   * @return �擾�ɐ���������true��Ԃ�
   */
  bool InitializeAdapter(u32 adapter_id_override, IDXGIAdapter1** adapter);

 private:
  //! �t�@�N�g��
  ComPtr<IDXGIFactory6> factory_;
  //! �A�_�v�^�[
  ComPtr<IDXGIAdapter1> adapter_;
  //! �f�o�C�X�I�v�V����
  DeviceOptionFlags options_;
  //! �A�_�v�^�[ID
  u32 adapter_id_;
};

}  // namespace device
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DEVICE_DXGI_ADAPTER_H_
