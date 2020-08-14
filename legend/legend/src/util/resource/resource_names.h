#ifndef LEGEND_UTIL_RESOURCE_RESOURCE_NAMES_H
#define LEGEND_UTIL_RESOURCE_RESOURCE_NAMES_H

/**
 * @file resource_names.h
 * @brief �g�p���郊�\�[�X����`
 */

namespace legend {

namespace util {
namespace resource {
namespace resource_names {

namespace model {
//! �G���f��
constexpr const wchar_t* ENEMY = L"enemy_eraser_01.glb";
//! �����f��
constexpr const wchar_t* DESK = L"desk.glb";
//! �v���C���[���f��
constexpr const wchar_t* PLAYER = L"eraser_01.glb";
//! �����J�X���f��
constexpr const wchar_t* FRAGMENT = L"eraser_fragment_01.glb";
//! ���[��1
constexpr const wchar_t* STATIONARY = L"accessory_01.glb";
//! ��Q��
constexpr const wchar_t* OBSTACLE = L"accessory_01.glb";
constexpr const wchar_t* GRAFFITI = L"1000cmObject.glb";
}  // namespace model

namespace texture {
//�e�X�g�e�N�X�`��
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
