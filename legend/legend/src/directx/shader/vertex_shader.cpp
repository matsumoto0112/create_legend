#include "src/directx/shader/vertex_shader.h"

#include "src/directx/directx_helper.h"
#include "src/util/byte_reader.h"

namespace {
/**
 * @brief コンポーネントの種類とマスクからフォーマットを取得する
 * @details
 * 例えばcomponent_type=D3D_REGISTER_COMPONENT_FLOATでmask=0111ならDXGI_FORMAT_R32G32B32_FLOATとなる
 */
DXGI_FORMAT GetFormat(D3D_REGISTER_COMPONENT_TYPE component_type, BYTE mask) {
  // maskの立っているビット数に応じて入力のコンポーネント数が決まる
  legend::u32 cnt = 0;
  for (legend::u32 i = 0; i <= 3; i++) {
    cnt += (mask >> i) & 1;
  }

  if (cnt == 0) {
    return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
  }

  //コンポーネントの種類とその数によって返す列挙子が変わる
  switch (component_type) {
    case D3D_REGISTER_COMPONENT_UINT32:
      switch (cnt) {
        case 1:
          return DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
        case 2:
          return DXGI_FORMAT::DXGI_FORMAT_R32G32_UINT;
        case 3:
          return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_UINT;
        default:
          return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_UINT;
      }
      break;
    case D3D_REGISTER_COMPONENT_SINT32:
      switch (cnt) {
        case 1:
          return DXGI_FORMAT::DXGI_FORMAT_R32_SINT;
        case 2:
          return DXGI_FORMAT::DXGI_FORMAT_R32G32_SINT;
        case 3:
          return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_SINT;
        default:
          return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_SINT;
      }
      break;
    case D3D_REGISTER_COMPONENT_FLOAT32:
      switch (cnt) {
        case 1:
          return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
        case 2:
          return DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
        case 3:
          return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
        default:
          return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
      }
      break;
    case D3D_REGISTER_COMPONENT_UNKNOWN:
    default:
      return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
      break;
  }
}
}  // namespace

namespace legend {
namespace directx {
namespace shader {

using directx_helper::Failed;

//コンストラクタ
VertexShader::VertexShader() {}

//デストラクタ
VertexShader::~VertexShader() {}

//初期化
bool VertexShader::Init(device::IDirectXAccessor& accessor,
                        const std::filesystem::path& filepath,
                        const std::vector<D3D12_INPUT_ELEMENT_DESC>& elements) {
  if (!ShaderBase::Init(accessor, filepath)) {
    return false;
  }

  this->elements_ = elements;
  return true;
}

//初期化
bool VertexShader::Init(device::IDirectXAccessor& accessor,
                        const std::filesystem::path& filepath) {
  if (!ShaderBase::Init(accessor, filepath)) {
    return false;
  }

  //リフレクションによって入力レイアウトを解析する
  if (Failed(D3DReflect(shader_code_.data(), shader_code_.size(),
                        IID_PPV_ARGS(&shader_refrection_)))) {
    return false;
  }

  D3D12_SHADER_DESC shader_desc;
  if (Failed(shader_refrection_->GetDesc(&shader_desc))) {
    return false;
  }

  this->elements_.resize(shader_desc.InputParameters);
  //各パラメータを読み取る
  for (u32 i = 0; i < shader_desc.InputParameters; i++) {
    D3D12_INPUT_ELEMENT_DESC& desc = elements_[i];
    D3D12_SIGNATURE_PARAMETER_DESC pd;
    shader_refrection_->GetInputParameterDesc(i, &pd);
    desc.SemanticName = pd.SemanticName;
    desc.SemanticIndex = pd.SemanticIndex;
    desc.InputSlot = 0;
    desc.Format = GetFormat(pd.ComponentType, pd.Mask);
    desc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
    desc.InputSlotClass =
        D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
    desc.InstanceDataStepRate = 0;
  }

  return true;
}
}  // namespace shader
}  // namespace directx
}  // namespace legend
