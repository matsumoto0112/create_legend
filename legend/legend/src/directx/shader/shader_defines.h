#ifndef LEGEND_DIRECTX_SHADER_SHADER_DEFINES_H_
#define LEGEND_DIRECTX_SHADER_SHADER_DEFINES_H_

namespace legend {
namespace directx {
namespace shader {

//! �V�F�[�_�[�̃R���p�C���t���O
// TODO: �V�F�[�_�[�����O�R���p�C������悤�ȃV�X�e���\�z
#if defined(_DEBUG)
static constexpr int COMPILE_FLAGS =
    D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
static constexpr int COMPILE_FLAGS = 0;
#endif
;

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_SHADER_DEFINES_H_
