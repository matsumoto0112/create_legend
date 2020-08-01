#ifndef LEGEND_DIRECTX_DIRECTX_HELPER_H_
#define LEGEND_DIRECTX_DIRECTX_HELPER_H_

/**
 * @file directx_helper.h
 * @brief DirectX�n�w���p�[�֐��Q
 */

namespace legend {
namespace directx {
namespace directx_helper {
/**
 * @brief �e�N�X�`���̃t�H�[�}�b�g����s�N�Z���̃������T�C�Y���擾����
 * @param format ���ׂ�t�H�[�}�b�g
 * @return �t�H�[�}�b�g�ɑΉ������������T�C�Y
 */
constexpr u32 CalcPixelSizeFromFormat(DXGI_FORMAT format) {
  switch (format) {
    case DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM:
      // RGBA���ꂼ��1Byte�ō��v4Byte
      return 4;
    case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:
      return 4;
    case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT:
      return sizeof(float);
    default:
      MY_LOG(L"����`�̃t�H�[�}�b�g���I������܂����B");
      return UINT_MAX;
  }
}

/**
 * @brief HRESULT�𕶎���ɕϊ�����
 */
inline std::wstring HrToWString(HRESULT hr) {
  wchar_t buf[64] = {};
  swprintf_s(buf, 64, L"HRESULT of 0x%08X", static_cast<u32>(hr));
  return std::wstring(buf);
}

/**
 * @brief DXR���T�|�[�g���Ă��邩
 * @param adapter ���ݗL���ȃA�_�v�^�[
 * @return �T�|�[�g���Ă����true��Ԃ�
 */
inline bool IsDirectXRaytracingSupported(IDXGIAdapter1* adapter) {
  ComPtr<ID3D12Device> test_device;
  D3D12_FEATURE_DATA_D3D12_OPTIONS5 feature_support_data = {};

  //�f�o�C�X������Ȃ���
  //�����ō��Ȃ���΂������������ɗ���O�ɏI�����Ă��邾�낤���O�̂��ߔ��肷��
  const bool succeeded_create_device = SUCCEEDED(
      D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0,
                        IID_PPV_ARGS(&test_device)));
  if (!succeeded_create_device) return false;

  // GPU�̃T�|�[�g��Ԃ��擾����
  const bool succeed_check_feature_support =
      SUCCEEDED(test_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5,
                                                 &feature_support_data,
                                                 sizeof(feature_support_data)));
  if (!succeed_check_feature_support) return false;

  //���C�g���[�V���O���T�|�[�g���Ă��邩�ǂ�������
  return feature_support_data.RaytracingTier !=
         D3D12_RAYTRACING_TIER::D3D12_RAYTRACING_TIER_NOT_SUPPORTED;
}

}  // namespace directx_helper
}  // namespace directx
}  // namespace legend

#endif  // !LEGEND_DIRECTX_DIRECTX_HELPER_H_
