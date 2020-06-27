#ifndef LEGEND_DIRECTX_RESOURCE_TYPE_H_
#define LEGEND_DIRECTX_RESOURCE_TYPE_H_

/**
 * @file resource_type.h
 * @brief リソースの種類定義
 */

namespace legend {
namespace directx {
/**
 * @brief リソースの種類
 */
enum class ResourceType : u32 {
  Cbv,
  Srv,
  Uav,
};

}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RESOURCE_TYPE_H_
