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
constexpr name ARROW = L"arrow.glb";
constexpr name ENEMY = L"enemy_eraser_01.glb";
constexpr name BOSS = L"boss_01.glb";
constexpr name DESK = L"desk.glb";
constexpr name PLAYER = L"eraser_01.glb";
constexpr name FRAGMENT_01 = L"eraser_fragment_01.glb";
constexpr name STATIONERY_01 = L"stationery_01.glb";
constexpr name STATIONERY_02 = L"stationery_02.glb";
constexpr name STATIONERY_03 = L"stationery_03.glb";
constexpr name STATIONERY_04 = L"stationery_04.glb";
constexpr name STATIONERY_05 = L"stationery_05.glb";
constexpr name OBSTACLE = L"field_object_01.glb";
constexpr name GRAFFITI = L"graffiti_01.glb";
constexpr name BACK_FIELD_01 = L"back_field_01.glb";
constexpr name ITEM_CAPSULE_01 = L"item_capsule_01.glb";
constexpr name ITEM_PLANE_01 = L"item_plane_01.glb";
constexpr name ITEM_PLANE_02 = L"item_plane_02.glb";
constexpr name GLUE_PLANE = L"glue_plane.glb";
constexpr name CLASSROOM_BACKWALL = L"classroom_backwall.glb";
constexpr name CLASSROOM_CEILING = L"classroom_ceiling.glb";
constexpr name CLASSROOM_FLOOR = L"classroom_floor.glb";
constexpr name CLASSROOM_FORWARDWALL = L"classroom_forwardwall.glb";
constexpr name CLASSROOM_LEFTWALL = L"classroom_leftwall.glb";
constexpr name CLASSROOM_RIGHTWALL = L"classroom_rightwall.glb";
}  // namespace model

namespace texture {
constexpr name FADE_IMAGE = L"fade_image.png";
constexpr name PARTICLE_FALLLOCUS_1 = L"Particle_fallLocus_1.png";
constexpr name PARTICLE_FALLLOCUS_2 = L"Particle_fallLocus_2.png";
constexpr name PARTICLE_SMOKE_1 = L"Particle_smoke_1.png";
constexpr name PARTICLE_SMOKE_2 = L"Particle_smoke_2.png";
constexpr name PARTICLE_SMOKE_3 = L"Particle_smoke_3.png";
constexpr name PARTICLE_SPARKING_1 = L"Particle_sparking_1.png";
constexpr name PARTICLE_SPARKING_2 = L"Particle_sparking_2.png";
constexpr name PARTICLE_SPARKING_3 = L"Particle_sparking_3.png";
constexpr name RESULT_GAMEOVER = L"Result_gameOver.png";
constexpr name RESULT_PLAYERPOWER_TEXT_1 = L"Result_playerPower_text_1.png";
constexpr name RESULT_PLAYERPOWER_TEXT_2 = L"Result_playerPower_text_2.png";
constexpr name RESULT_STAGECLEAR = L"Result_stageClear.png";
constexpr name RESULT_TOTALTURN_TEXT_1 = L"Result_totalTurn_text_1.png";
constexpr name RESULT_TOTALTURN_TEXT_2 = L"Result_totalTurn_text_2.png";
constexpr name STAGESELECT_ARROW_LEFT = L"StageSelect_arrow_left.png";
constexpr name STAGESELECT_ARROW_RIGHT = L"StageSelect_arrow_right.png";
constexpr name STAGESELECT_STAGE_FRAME = L"StageSelect_stage_frame.png";
constexpr name TEX = L"tex.png";
constexpr name TITLE_LOGO = L"TItle_logo.png";
constexpr name TITLE_PUSHBUTTON_GUIDE = L"Title_pushButton_guide.png";
constexpr name UI_NUMBER_1 = L"UI_number_1.png";
constexpr name UI_PLAYERFORM_FRAME = L"UI_playerForm_frame.png";
constexpr name UI_PLAYERFORM_PLAYER = L"UI_playerForm_player.png";
constexpr name UI_PLAYERFORM_SKILL_1 = L"UI_playerForm_skill_1.png";
constexpr name UI_PLAYERFORM_SKILL_2 = L"UI_playerForm_skill_2.png";
constexpr name UI_PLAYERPOWER_FRAME = L"UI_playerPower_frame.png";
constexpr name UI_PLAYERPOWER_GAUGE_0TO100 = L"UI_playerPower_gauge_0to100.png";
constexpr name UI_PLAYERPOWER_GAUGE_101TO200 =
    L"UI_playerPower_gauge_101to200.png";
constexpr name UI_PLAYERPOWER_GAUGE_201TO300 =
    L"UI_playerPower_gauge_201to300.png";
constexpr name UI_PLAYERPOWER_GAUGE_FRAME = L"UI_playerPower_gauge_frame.png";
constexpr name UI_PLAYER_ESTIMATE = L"UI_player_estimate.png";
constexpr name UI_POSE_BACK = L"UI_pose_back.png";
constexpr name UI_POWERGAUGE = L"UI_powerGauge.png";
constexpr name UI_POWERGAUGE_BLURRYPOINT = L"UI_powerGauge_blurryPoint.png";
constexpr name UI_POWERGAUGE_FRAME = L"UI_powerGauge_frame.png";
constexpr name UI_SKILL_EXPLANATION = L"UI_skill_explanation.png";
constexpr name UI_SKILL_FRAME = L"UI_skill_frame.png";
constexpr name UI_SKILL_FRAME_WHOLE = L"UI_skill_frame_whole.png";
constexpr name UI_SKILL_ICON_1 = L"UI_skill_icon_1.png";
constexpr name UI_SKILL_ICON_2 = L"UI_skill_icon_2.png";
constexpr name UI_SKILL_ICON_3 = L"UI_skill_icon_3.png";
constexpr name UI_SKILL_ICON_4 = L"UI_skill_icon_4.png";
constexpr name UI_SKILL_ICON_5 = L"UI_skill_icon_5.png";
constexpr name UI_SKILL_SELECT_FRAME = L"UI_skill_select_frame.png";
}  // namespace texture

namespace vertex_shader {
constexpr name SPRITE = L"sprite_vs.cso";
constexpr name GRAFFITI = L"graffiti_vs.cso";
constexpr name MODEL_VIEW = L"model_view_vs.cso";
constexpr name PLAYER_MOVE_PARTICLE = L"player_move_particle_vs.cso";
constexpr name OBB = L"obb_vs.cso";
constexpr name GAUGE = L"ui_gauge_vs.cso";
constexpr name QUARTER_GAUGE = L"ui_quarter_gauge_vs.cso";
constexpr name DIFFERED_RENDERING_PRE = L"differed_rendering_pre_vs";
constexpr name DIFFERED_RENDERING = L"differed_rendering_vs";
constexpr name SHADOW_MAP = L"shadow_map_vs";
constexpr name FIRE_PARTICLE = L"fire_particle_vs.cso";
}  // namespace vertex_shader

namespace pixel_shader {
constexpr name SPRITE = L"sprite_ps.cso";
constexpr name GRAFFITI = L"graffiti_ps.cso";
constexpr name MODEL_VIEW = L"model_view_ps.cso";
constexpr name PLAYER_MOVE_PARTICLE = L"player_move_particle_ps.cso";
constexpr name FIRE_PARTICLE = L"player_move_particle_ps.cso";
constexpr name OBB = L"obb_ps.cso";
constexpr name GAUGE = L"ui_gauge_ps.cso";
constexpr name QUARTER_GAUGE = L"ui_quarter_gauge_ps.cso";
constexpr name DIFFERED_RENDERING_PRE = L"differed_rendering_pre_ps";
constexpr name DIFFERED_RENDERING = L"differed_rendering_ps";
}  // namespace pixel_shader

namespace geometry_shader {
constexpr name PLAYER_MOVE_PARTICLE = L"player_move_particle_gs.cso";
constexpr name FIRE_PARTICLE = L"fire_particle_gs.cso";
}  // namespace geometry_shader

namespace compute_shader {
constexpr name PLAYER_MOVE_PARTICLE = L"player_move_particle_cs.cso";
constexpr name FIRE_PARTICLE = L"fire_particle_cs.cso";
}  // namespace compute_shader

namespace pipeline {
constexpr name SPRITE = L"sprite";
constexpr name GRAFFITI = L"graffiti";
constexpr name MODEL_VIEW = L"model_view";
constexpr name PLAYER_MOVE_PARTICLE = L"player_move_particle";
constexpr name OBB = L"obb";
constexpr name GAUGE = L"gauge";
constexpr name QUARTER_GAUGE = L"quarter_gauge";
constexpr name DIFFERED_RENDERING_PRE = L"differed_rendering_pre";
constexpr name DIFFERED_RENDERING = L"differed_rendering";
constexpr name SHADOW_MAP = L"shadow_map";
}  // namespace pipeline

namespace audio {
constexpr name ENEMY_TURN_END = L"enemy_turn_end.wav";
constexpr name ERASE_GRAFFITI = L"erase_graffiti.wav";
constexpr name PLAYER_ENEMY_HIT_BIG = L"player_enemy_hit_big.wav";
constexpr name PLAYER_ENEMY_HIT_SMALL = L"player_enemy_hit_small.wav";
constexpr name PLAYER_FALL_OUT = L"player_fall_out.wav";
constexpr name PLAYER_GET_FRAGMENT = L"player_get_fragment.wav";
constexpr name PLAYER_GET_STATIONERY = L"player_get_stationery.wav";
constexpr name PLAYER_MOVE_SELECT_SKILL_MODE =
    L"player_move_select_skill_mode.wav";
constexpr name PLAYER_MOVING = L"player_moving.wav";
constexpr name PLAYER_OBSTACLE_HIT = L"player_obstacle_hit.wav";
constexpr name PLAYER_POWER_CHARGE = L"player_power_charge.wav";
constexpr name PLAYER_RETURN_FROM_SELECT_SKILL_MODE =
    L"player_return_from_select_skill_mode.wav";
constexpr name PLAYER_SNAP = L"player_snap.wav";
constexpr name PLAYER_TURN_END = L"player_turn_end.wav";
constexpr name SKILL_DECISION = L"skill_decision.wav";
constexpr name SKILL_PENCIL_HIT = L"skill_pencil_hit.wav";
constexpr name SKILL_PENCIL_SHOT = L"skill_pencil_shot.wav";
constexpr name SKILL_SELECT = L"skill_select.wav";

constexpr name TITLE_DECISION = L"title_decision.wav";
// constexpr name RESULT_STAGE_CLEAR = L"free_2.wav";
// constexpr name RESULT_GAMEOVER = L"free_2.wav";
// constexpr name RESULT_PRINT_TEXT = L"free_2.wav";
constexpr name RESULT_MOVE_OTHER_SCENE = L"result_move_other_scene.wav";

constexpr name BGM_MAIN = L"BGM_main.wav";
constexpr name BGM_TITLE = L"BGM_title.wav";
constexpr name BGM_RESULT = L"BGM_result.wav";
}  // namespace audio

}  // namespace resource_names
}  // namespace resource
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_RESOURCE_NAMES_H
