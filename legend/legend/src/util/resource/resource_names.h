#ifndef LEGEND_UTIL_RESOURCE_RESOURCE_NAMES_H
#define LEGEND_UTIL_RESOURCE_RESOURCE_NAMES_H

/**
 * @file resource_names.h
 * @brief 使用するリソース名定義
 */

namespace legend {

namespace util {
namespace resource {
namespace resource_names {

namespace model {
//! 敵モデル
constexpr const wchar_t* ENEMY = L"enemy_eraser_01.glb";
//! 机モデル
constexpr const wchar_t* DESK = L"desk.glb";
//! プレイヤーモデル
constexpr const wchar_t* PLAYER = L"eraser_01.glb";
//! 消しカスモデル
constexpr const wchar_t* FRAGMENT_01 = L"eraser_fragment_01.glb";
//! 文房具1
constexpr const wchar_t* STATIONERY_01 = L"stationery_01.glb";
//! 文房具2
constexpr const wchar_t* STATIONERY_02 = L"stationery_02.glb";
//! 文房具3
constexpr const wchar_t* STATIONERY_03 = L"stationery_03.glb";
constexpr const wchar_t* STATIONERY_04 = L"stationery_04.glb";
constexpr const wchar_t* STATIONERY_05 = L"stationery_05.glb";
//! 障害物
constexpr const wchar_t* OBSTACLE = L"field_object_01.glb";
//! 落書き
constexpr const wchar_t* GRAFFITI = L"graffiti_01.glb";
constexpr const wchar_t* BACK_FIELD_01 = L"back_field_01.glb";
}  // namespace model

namespace texture {
//テストテクスチャ
constexpr const wchar_t* TEX = L"tex.png";
}  // namespace texture

namespace vertex_shader {
constexpr const wchar_t* SPRITE = L"sprite_vs.cso";
constexpr const wchar_t* GRAFFITI = L"graffiti_vs.cso";
constexpr const wchar_t* MODEL_VIEW = L"model_view_vs.cso";
constexpr const wchar_t* GPU_PARTICLE = L"gpu_particle_vs.cso";
constexpr const wchar_t* OBB = L"obb_vs.cso";
}  // namespace vertex_shader

namespace pixel_shader {
constexpr const wchar_t* SPRITE = L"sprite_ps.cso";
constexpr const wchar_t* GRAFFITI = L"graffiti_ps.cso";
constexpr const wchar_t* MODEL_VIEW = L"model_view_ps.cso";
constexpr const wchar_t* GPU_PARTICLE = L"gpu_particle_ps.cso";
constexpr const wchar_t* OBB = L"obb_ps.cso";
}  // namespace pixel_shader

namespace geometry_shader {
constexpr const wchar_t* GPU_PARTICLE = L"gpu_particle_gs.cso";
}  // namespace geometry_shader

namespace compute_shader {
constexpr const wchar_t* GPU_PARTICLE = L"gpu_particle_cs.cso";
}  // namespace compute_shader

namespace pipeline {
constexpr const wchar_t* SPRITE = L"sprite";
constexpr const wchar_t* GRAFFITI = L"graffiti";
constexpr const wchar_t* MODEL_VIEW = L"model_view";
constexpr const wchar_t* GPU_PARTICLE = L"gpu_particle";
constexpr const wchar_t* OBB = L"obb";
}  // namespace pipeline

namespace audio {}  // namespace audio

}  // namespace resource_names
}  // namespace resource
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_RESOURCE_NAMES_H
