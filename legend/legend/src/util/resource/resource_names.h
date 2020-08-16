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
using name = const wchar_t*;
namespace model {
//! 敵モデル
constexpr name ENEMY = L"enemy_eraser_01.glb";
//! 机モデル
constexpr name DESK = L"desk.glb";
//! プレイヤーモデル
constexpr name PLAYER = L"eraser_01.glb";
//! 消しカスモデル
constexpr name FRAGMENT_01 = L"eraser_fragment_01.glb";
//! 文房具1
constexpr name STATIONERY_01 = L"stationery_01.glb";
//! 文房具2
constexpr name STATIONERY_02 = L"stationery_02.glb";
//! 文房具3
constexpr name STATIONERY_03 = L"stationery_03.glb";
constexpr name STATIONERY_04 = L"stationery_04.glb";
constexpr name STATIONERY_05 = L"stationery_05.glb";
//! 障害物
constexpr name OBSTACLE = L"field_object_01.glb";
//! 落書き
constexpr name GRAFFITI = L"graffiti_01.glb";
constexpr name BACK_FIELD_01 = L"back_field_01.glb";
}  // namespace model

namespace texture {
//テストテクスチャ
constexpr name TEX = L"tex.png";
constexpr name UI_PLAYERFORM_FRAME = L"UI_playerForm_frame.png";
constexpr name UI_PLAYERPOWER_FRAME = L"UI_playerPower_frame.png";
constexpr name UI_PLAYERPOWER_GAUGE_FRAME = L"UI_playerPower_gauge_frame.png";
constexpr name UI_PLAYERPOWER_GAUGE_0_TO_100 =
    L"UI_playerPower_gauge_0to100.png";
constexpr name UI_PLAYERPOWER_GAUGE_101_TO_200 =
    L"UI_playerPower_gauge_101to200.png";
constexpr name UI_PLAYERPOWER_GAUGE_201_TO_300 =
    L"UI_playerPower_gauge_201to300.png";
constexpr name UI_PLAYERPOWER_GAUGE_BLURRYPOINT =
    L"UI_powerGauge_blurryPoint.png";
constexpr name UI_POWERGAUGE_FRAME = L"UI_powerGauge_frame.png";
constexpr name UI_POWERGAUGE = L"UI_powerGauge.png";
}  // namespace texture

namespace vertex_shader {
constexpr name SPRITE = L"sprite_vs.cso";
constexpr name GRAFFITI = L"graffiti_vs.cso";
constexpr name MODEL_VIEW = L"model_view_vs.cso";
constexpr name GPU_PARTICLE = L"gpu_particle_vs.cso";
constexpr name OBB = L"obb_vs.cso";
}  // namespace vertex_shader

namespace pixel_shader {
constexpr name SPRITE = L"sprite_ps.cso";
constexpr name GRAFFITI = L"graffiti_ps.cso";
constexpr name MODEL_VIEW = L"model_view_ps.cso";
constexpr name GPU_PARTICLE = L"gpu_particle_ps.cso";
constexpr name OBB = L"obb_ps.cso";
}  // namespace pixel_shader

namespace geometry_shader {
constexpr name GPU_PARTICLE = L"gpu_particle_gs.cso";
}  // namespace geometry_shader

namespace compute_shader {
constexpr name GPU_PARTICLE = L"gpu_particle_cs.cso";
}  // namespace compute_shader

namespace pipeline {
constexpr name SPRITE = L"sprite";
constexpr name GRAFFITI = L"graffiti";
constexpr name MODEL_VIEW = L"model_view";
constexpr name GPU_PARTICLE = L"gpu_particle";
constexpr name OBB = L"obb";
}  // namespace pipeline

namespace audio {}  // namespace audio

}  // namespace resource_names
}  // namespace resource
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_RESOURCE_NAMES_H
