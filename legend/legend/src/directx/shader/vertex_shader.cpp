#include "src/directx/shader/vertex_shader.h"

#include "src/directx/directx_helper.h"
#include "src/util/byte_reader.h"

namespace {
/**
 * @brief �R���|�[�l���g�̎�ނƃ}�X�N����t�H�[�}�b�g���擾����
 * @details
 * �Ⴆ��component_type=D3D_REGISTER_COMPONENT_FLOAT��mask=0111�Ȃ�DXGI_FORMAT_R32G32B32_FLOAT�ƂȂ�
 */
DXGI_FORMAT GetFormat(D3D_REGISTER_COMPONENT_TYPE component_type, BYTE mask) {
  // mask�̗����Ă���r�b�g���ɉ����ē��͂̃R���|�[�l���g�������܂�
  legend::u32 cnt = 0;
  for (legend::u32 i = 0; i <= 3; i++) {
    cnt += (mask >> i) & 1;
  }

  if (cnt == 0) {
    return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
  }

  //�R���|�[�l���g�̎�ނƂ��̐��ɂ���ĕԂ��񋓎q���ς��
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

//�R���X�g���N�^
VertexShader::VertexShader() {}

//�f�X�g���N�^
VertexShader::~VertexShader() {}

//������
bool VertexShader::Init(device::IDirectXAccessor& accessor,
                        const std::filesystem::path& filepath,
                        const std::vector<D3D12_INPUT_ELEMENT_DESC>& elements) {
  if (!ShaderBase::Init(accessor, filepath)) {
    return false;
  }

  this->elements_ = elements;
  return true;
}

//������
bool VertexShader::Init(device::IDirectXAccessor& accessor,
                        const std::filesystem::path& filepath) {
  if (!ShaderBase::Init(accessor, filepath)) {
    return false;
  }

  //���t���N�V�����ɂ���ē��̓��C�A�E�g����͂���
  if (Failed(D3DReflect(shader_code_.data(), shader_code_.size(),
                        IID_PPV_ARGS(&shader_refrection_)))) {
    return false;
  }

  D3D12_SHADER_DESC shader_desc;
  if (Failed(shader_refrection_->GetDesc(&shader_desc))) {
    return false;
  }

  this->elements_.resize(shader_desc.InputParameters);
  //�e�p�����[�^��ǂݎ��
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
