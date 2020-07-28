#ifndef LEGEND_DIRECTX_MATERIAL_MATERIAL_H_
#define LEGEND_DIRECTX_MATERIAL_MATERIAL_H_

/**
 * @file material.h
 * @brief マテリアルインターフェース定義
 */

#include "src/directx/directx_accessor.h"
#include "src/directx/shader/graphics_pipeline_state.h"

namespace legend {
namespace directx {
namespace material {
class IMaterial {
 public:
  virtual ~IMaterial() = default;
  virtual bool Init(IDirectXAccessor& accessor) = 0;
  virtual void SetVertexShader(const std::filesystem::path& filepath);
  virtual void SetToGraphicsCommandList(IDirectXAccessor& accessor) = 0;
};
}  // namespace material
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_MATERIAL_MATERIAL_H_
