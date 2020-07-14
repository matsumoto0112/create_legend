#include "src/directx/shader/root_signature.h"

#include "src/directx/shader/defined_static_sampler.h"

namespace {
constexpr legend::u32 CBV_DESCRIPTOR_NUM = 16;
constexpr legend::u32 SRV_DESCRIPTOR_NUM = 48;
constexpr legend::u32 UAV_DESCRIPTOR_NUM = 16;
}  // namespace

namespace legend {
namespace directx {
namespace shader {

//コンストラクタ
RootSignature::RootSignature() {}

//デストラクタ
RootSignature::~RootSignature() {}

//初期化
bool RootSignature::Init(IDirectXAccessor& accessor, const std::wstring& name) {
  //すべてのシェーダーで使いまわせるように最大限の領域を確保する
  std::array<CD3DX12_DESCRIPTOR_RANGE, 3> ranges;
  ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
                 CBV_DESCRIPTOR_NUM, 0);
  ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
                 SRV_DESCRIPTOR_NUM, 0);
  ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
                 UAV_DESCRIPTOR_NUM, 0);

  std::array<CD3DX12_ROOT_PARAMETER, 3> params;
  params[0].InitAsDescriptorTable(1, &ranges[0]);
  params[1].InitAsDescriptorTable(1, &ranges[1]);
  params[2].InitAsDescriptorTable(1, &ranges[2]);

  std::vector<D3D12_STATIC_SAMPLER_DESC> samplers{
      defined_sampler::CreateStaticSampler(
          0, D3D12_FILTER::D3D12_FILTER_MIN_MAG_MIP_LINEAR,
          D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP),
      defined_sampler::CreateStaticSampler(
          1, D3D12_FILTER::D3D12_FILTER_MIN_MAG_MIP_POINT,
          D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP),
  };

  CD3DX12_ROOT_SIGNATURE_DESC root_signature_desc = {};
  root_signature_desc.Init(
      static_cast<u32>(params.size()), params.data(),
      static_cast<u32>(samplers.size()), samplers.data(),
      D3D12_ROOT_SIGNATURE_FLAGS::
          D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

  ComPtr<ID3DBlob> signature;
  ComPtr<ID3DBlob> error;
  if (FAILED(D3D12SerializeRootSignature(
          &root_signature_desc,
          D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1, &signature,
          &error))) {
    char* mes = static_cast<char*>(error->GetBufferPointer());
    MY_LOG(L"D3D12SerializeRootSignature failed");
    return false;
  }

  if (FAILED(accessor.GetDevice()->CreateRootSignature(
          0, signature->GetBufferPointer(), signature->GetBufferSize(),
          IID_PPV_ARGS(&root_signature_)))) {
    MY_LOG(L"CreateRootSignature failed");
    return false;
  }

  if (FAILED(this->root_signature_->SetName(name.c_str()))) {
    return false;
  }

  return true;
}

//コマンドリストにセットする
void RootSignature::SetGraphicsCommandList(IDirectXAccessor& accessor) {
  accessor.GetCommandList()->SetGraphicsRootSignature(root_signature_.Get());
}

}  // namespace shader
}  // namespace directx
}  // namespace legend