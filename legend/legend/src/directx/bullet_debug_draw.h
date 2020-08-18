#ifndef LEGEND_DIRECTX_BULLET_DEBUG_DRAW_H
#define LEGEND_DIRECTX_BULLET_DEBUG_DRAW_H

/**
 * @file bullet_debug_draw.h
 * @brief bullet用デバッグ描画機能定義
 */

#include <LinearMath/btIDebugDraw.h>

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/math/matrix_4x4.h"
#include "src/util/color_4.h"

namespace legend {
namespace directx {
class BulletDebugDraw : public btIDebugDraw {
 public:
  BulletDebugDraw();
  virtual ~BulletDebugDraw();
  bool Init(ID3D12Device* device);
  void Render(const math::Matrix4x4& view, const math::Matrix4x4& projection,
              device::CommandList& command_list);
  virtual void drawLine(const btVector3& from, const btVector3& to,
                        const btVector3& fromColor, const btVector3& toColor);
  virtual void drawLine(const btVector3& from, const btVector3& to,
                        const btVector3& color);

  //未対応
  virtual void drawContactPoint(const btVector3& PointOnB,
                                const btVector3& normalOnB, btScalar distance,
                                int lifeTime, const btVector3& color) {}
  virtual void reportErrorWarning(const char* warningString) {}
  virtual void draw3dText(const btVector3& location, const char* textString) {}

  virtual void setDebugMode(int debugMode) { bitDebugMode = debugMode; }
  virtual int getDebugMode() const { return bitDebugMode; }

 private:
  static constexpr u32 MAX_LINE_NUM = 10000;
  int bitDebugMode;

  struct LineVertex {
    btVector3 pos;
    btVector3 color;
    LineVertex() = default;
    LineVertex(const btVector3& pos, const btVector3 color)
        : pos(pos), color(color) {}
  };

  struct Line {
    LineVertex from, to;
    Line() = default;
    Line(const btVector3& from, const btVector3& fromColor, const btVector3& to,
         const btVector3& toColor)
        : from(from, fromColor), to(to, toColor) {}
  };

  //! 描画する線分
  //! 描画範囲内のデータはそもそもシェーダーに渡されないようになっているので、固定数の配列を確保
  std::array<Line, MAX_LINE_NUM> lines_;
  u32 index;

  ID3D12Device* device_;
  ComPtr<ID3D12Resource> vertex_buffer_;
  ComPtr<ID3D12Resource> vertex_buffer_upload_;

  buffer::ConstantBuffer<buffer::constant_buffer_structure::WorldContext>
      world_cb_;
};

}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BULLET_DEBUG_DRAW_H
