#ifndef LEGEND_DIRECTX_VERTEX_H_
#define LEGEND_DIRECTX_VERTEX_H_

/**
 * @file vertex.h
 * @brief ���_�\���̒�`
 */

#include "src/math/vector_2.h"
#include "src/math/vector_3.h"
#include "src/math/vector_4.h"

namespace legend {
namespace directx {
/**
 * @brief �ʏ탂�f�����_
 */
struct Vertex {
  math::Vector3 position;
  math::Vector3 normal;
  math::Vector2 uv;
  math::Vector4 tangent;
};

/**
 * @brief 2D�X�v���C�g�p
 */
struct Sprite {
  math::Vector3 position;
  math::Vector2 uv;
};

struct BoundingBox {
    math::Vector3 position;
};

namespace input_element {

/**
 * @brief �^�ɉ��������̓��C�A�E�g��Ԃ�
 * @tparam T ���_�\����
 * @return T�ɑΉ��������̓��C�A�E�g��Ԃ�
 * @details ����`�̏ꍇ�̓R���p�C���G���[�ƂȂ�
 */
template <class T>
inline std::vector<D3D12_INPUT_ELEMENT_DESC> GetElementDescs() {
  static_assert(false);
}

template <>
inline std::vector<D3D12_INPUT_ELEMENT_DESC> GetElementDescs<Vertex>() {
  return {
      {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0},
      {"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0},
      {"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0},
      {"TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0}};
};

template <>
inline std::vector<D3D12_INPUT_ELEMENT_DESC> GetElementDescs<Sprite>() {
  return {
      {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0},
      {"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0}};
};

}  // namespace input_element

}  // namespace directx
}  // namespace legend

#endif  // !LEGEND_DIRECTX_VERTEX_H_
