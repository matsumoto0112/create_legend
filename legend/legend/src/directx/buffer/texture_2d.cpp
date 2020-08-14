#include "src/directx/buffer/texture_2d.h"

#include "src/directx/directx_helper.h"
#include "src/util/loader/texture_loader.h"

namespace legend {
namespace directx {
namespace buffer {

//�R���X�g���N�^
Texture2D::Texture2D()
    : texture_{},
      texture_immediate_{},
      format_{DXGI_FORMAT::DXGI_FORMAT_UNKNOWN},
      width_{0},
      height_{0},
      handle_{} {}

//�f�X�g���N�^
Texture2D::~Texture2D() {}

//������
bool Texture2D::Init(device::IDirectXAccessor& accessor, const Desc& desc) {
  return InitTexBuffer(accessor, desc);
}

//�������Ə�������
bool Texture2D::InitAndWrite(device::IDirectXAccessor& accessor,
                             device::CommandList& command_list,
                             const std::filesystem::path& filename,
                             const descriptor_heap::DescriptorHandle& handle) {
  //�e�N�X�`����ǂݍ���
  const util::loader::texture_loader::LoadedTextureData data =
      util::loader::texture_loader::Load(filename);

  const Desc desc{DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, data.width,
                  data.height, handle, data.name};
  if (!InitTexBuffer(accessor, desc)) {
    return false;
  }

  WriteResource(command_list, data.pixels.data());
  return true;
}

//�������Ə�������
bool Texture2D::InitAndWrite(device::IDirectXAccessor& accessor,
                             device::CommandList& command_list,
                             DXGI_FORMAT format, const std::vector<u8>& data,
                             const descriptor_heap::DescriptorHandle& handle,
                             const std::wstring& name) {
  const util::loader::texture_loader::LoadedTextureData loaded_data =
      util::loader::texture_loader::LoadFromMemory(data);

  const Desc desc{format, loaded_data.width, loaded_data.height, handle, name};
  if (!Init(accessor, desc)) {
    return false;
  }
  WriteResource(command_list, loaded_data.pixels.data());
  return true;
}

//���\�[�X�f�[�^����������
void Texture2D::WriteResource(device::CommandList& command_list,
                              const void* data) {
  const u64 row = width_ * directx_helper::CalcPixelSizeFromFormat(format_);
  const u64 slice = row * height_;

  texture_.Transition(command_list,
                      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);

  CommittedResource::UpdateSubresource(command_list, &texture_,
                                       &texture_immediate_, data, row, slice);
  // TODO:�s�N�Z���V�F�[�_�[���ł����g�p���Ȃ��Ȃ�STATE_PIXEL_SHADER_RESOURCE���g�p����悤�ɋ@�\�ǉ�
  texture_.Transition(command_list,
                      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
}

//�q�[�v�ɒǉ�����
void Texture2D::RegisterHandle(device::IDirectXAccessor& accessor,
                               u32 register_num) {
  accessor.RegisterHandle(register_num, shader::ResourceType::SRV, handle_);
}

//�e�N�X�`���o�b�t�@������������
bool Texture2D::InitTexBuffer(device::IDirectXAccessor& accessor,
                              const Desc& desc) {
  const CommittedResource::Tex2DDesc buffer_desc{
      desc.name,
      desc.format,
      desc.width,
      desc.height,
      D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE,
      CD3DX12_CLEAR_VALUE(),
      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ};

  if (!texture_.InitAsTex2D(accessor, buffer_desc)) {
    return false;
  }

  //�R�s�[�Ɏg�p����ꎞ�I�ȃ��\�[�X���쐬����
  const u64 immediate_size =
      GetRequiredIntermediateSize(texture_.GetResource(), 0, 1);
  CommittedResource::BufferDesc immediate_desc = {
      desc.name + L"_Immediate", immediate_size,
      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ};
  if (!texture_immediate_.InitAsBuffer(accessor, immediate_desc)) {
    return false;
  }

  //�p�����[�^�̐ݒ�
  this->format_ = desc.format;
  this->width_ = desc.width;
  this->height_ = desc.height;
  this->handle_ = desc.handle;

  D3D12_SHADER_RESOURCE_VIEW_DESC srv_view = {};
  srv_view.Texture2D.MipLevels = 1;
  srv_view.Format = desc.format;
  srv_view.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
  srv_view.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
  accessor.GetDevice()->CreateShaderResourceView(
      texture_.GetResource(), &srv_view, handle_.cpu_handle_);

  return true;
}  // namespace buffer

}  // namespace buffer
}  // namespace directx
}  // namespace legend
