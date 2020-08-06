#include "src/directx/shader/root_signature.h"

#include "src/directx/directx_helper.h"
#include "src/directx/shader/defined_static_sampler.h"
#include "src/directx/shader/root_parameter_index.h"
#include "src/directx/shader/shader_register_id.h"

namespace {
constexpr legend::u32 CBV_DESCRIPTOR_NUM = 16;
constexpr legend::u32 SRV_DESCRIPTOR_NUM = 48;
constexpr legend::u32 UAV_DESCRIPTOR_NUM = 16;
}  // namespace

namespace legend {
namespace directx {
namespace shader {

using directx_helper::Failed;

//コンストラクタ
RootSignature::RootSignature() {}

//デストラクタ
RootSignature::~RootSignature() {}

bool RootSignature::InitByDefault(device::IDirectXAccessor& accessor,
                                  const std::wstring& name) {
  //すべてのシェーダーで使いまわせるように最大限の領域を確保する
  std::array<CD3DX12_DESCRIPTOR_RANGE, root_parameter_index::MAX> ranges;
  ranges[root_parameter_index::CBV].Init(
      D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
      CBV_DESCRIPTOR_NUM, 0);
  ranges[root_parameter_index::SRV].Init(
      D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
      SRV_DESCRIPTOR_NUM, 0);
  ranges[root_parameter_index::UAV].Init(
      D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
      UAV_DESCRIPTOR_NUM, 0);

  std::array<CD3DX12_ROOT_PARAMETER, root_parameter_index::MAX> params;
  params[root_parameter_index::CBV].InitAsDescriptorTable(
      1, &ranges[root_parameter_index::CBV]);
  params[root_parameter_index::SRV].InitAsDescriptorTable(
      1, &ranges[root_parameter_index::SRV]);
  params[root_parameter_index::UAV].InitAsDescriptorTable(
      1, &ranges[root_parameter_index::UAV]);

  std::vector<D3D12_STATIC_SAMPLER_DESC> samplers{
      defined_static_sampler::Warp(SamplerRegisterID::WARP),
      defined_static_sampler::Clamp(SamplerRegisterID::CLAMP),
  };

  CD3DX12_ROOT_SIGNATURE_DESC root_signature_desc = {};
  root_signature_desc.Init(
      static_cast<u32>(params.size()), params.data(),
      static_cast<u32>(samplers.size()), samplers.data(),
      D3D12_ROOT_SIGNATURE_FLAGS::
          D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

  ComPtr<ID3DBlob> signature;
  ComPtr<ID3DBlob> error;
  if (Failed(D3D12SerializeRootSignature(
          &root_signature_desc,
          D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1, &signature,
          &error))) {
    MY_LOG(L"D3D12SerializeRootSignature failed");
    if (error) {
      OutputDebugStringA(static_cast<char*>(error->GetBufferPointer()));
    }
    return false;
  }

  if (Failed(accessor.GetDevice()->CreateRootSignature(
          0, signature->GetBufferPointer(), signature->GetBufferSize(),
          IID_PPV_ARGS(&root_signature_)))) {
    return false;
  }

  root_signature_->SetName(name.c_str());

  return true;
}

void RootSignature::SetGraphicsCommandList(device::CommandList& command_list) {
  command_list.GetCommandList()->SetGraphicsRootSignature(
      root_signature_.Get());
}

}  // namespace shader
}  // namespace directx
}  // namespace legend