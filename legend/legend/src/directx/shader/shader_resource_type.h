#ifndef LEGEND_DIRECTX_SHADER_SHADER_RESOURCE_TYPE_H_
#define LEGEND_DIRECTX_SHADER_SHADER_RESOURCE_TYPE_H_

/**
 * @file shader_resource_type.h
 * @brief シェーダーリソースの種類定義
 */

namespace legend {
namespace directx {
namespace shader {

/**
 * @enum ResourceType
 * @brief リソースの種類
 */
enum class ResourceType : u32 {
  CBV,
  SRV,
  UAV,
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_SHADER_RESOURCE_TYPE_H_
