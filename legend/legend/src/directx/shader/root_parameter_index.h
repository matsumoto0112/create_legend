#ifndef LEGEND_DIRECTX_SHADER_ROOT_PARAMETER_INDEX_H_
#define LEGEND_DIRECTX_SHADER_ROOT_PARAMETER_INDEX_H_

/**
 * @file root_parameter_index.h
 * @brief ルートシグネチャのパラメータインデックス定義
 */

namespace legend {
namespace directx {
namespace shader {

/**
 * @brief ルートシグネチャのパラメータインデックス
 * @details ０番はCBV、１番はSRV、２番はUAVを使用する
 */
namespace root_parameter_index {
enum Enum : u32 { CBV, SRV, UAV, MAX };

}  // namespace root_parameter_index
}  // namespace shader
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_SHADER_ROOT_PARAMETER_INDEX_H_
