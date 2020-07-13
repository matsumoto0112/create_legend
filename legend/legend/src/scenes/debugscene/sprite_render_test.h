#ifndef LEGEND_SCENES_DEBUGSCENE_SPRITE_RENDER_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_SPRITE_RENDER_TEST_H_

/**
 * @file sprite_render_test.h
 * @brief スプライト描画テストシーン
 */

#include "src/draw/sprite_renderer.h"
#include "src/scenes/scene.h"

namespace legend {
namespace scenes {
namespace debugscene {
class SpriteRenderTest : public Scene {
 public:
  SpriteRenderTest(ISceneChange* scene_change);
  virtual ~SpriteRenderTest();
  virtual bool Initialize() override;
  virtual bool Update() override;
  virtual void Draw() override;
  virtual void Finalize() override;

 private:
  draw::SpriteRenderer sprite_renderer_;
  std::vector<draw::Sprite2D> sprite_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_SPRITE_RENDER_TEST_H_
