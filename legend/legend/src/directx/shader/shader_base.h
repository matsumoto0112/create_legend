#ifndef LEGEND_DIRECTX_SHADER_SHADER_BASE_H_
#define LEGEND_DIRECTX_SHADER_SHADER_BASE_H_

/**
 * @file shader_base.h
 */

#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {
namespace shader {
class ShaderBase {
 public:
  ShaderBase(){};
  virtual ~ShaderBase() = default;
  virtual bool Init(device::IDirectXAccessor& accessor,
                    const std::filesystem::path& filepath);
  /**
   * @brief シェーダーコードとして返す
   */
  CD3DX12_SHADER_BYTECODE GetShaderBytecode() const {
    return CD3DX12_SHADER_BYTECODE{shader_code_.data(), shader_code_.size()};
  }

 protected:
  std::vector<u8> shader_code_;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_SHADER_SHADER_BASE_H_
