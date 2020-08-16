#ifndef LEGEND_SCENES_DEBUGSCENE_SKILL_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_SKILL_TEST_H_

#include "src/scenes/scene.h"
#include "src/skill/skill_manager.h"
#include "src/system/turn_system.h"

namespace legend {
namespace scenes {
namespace debugscene {

class SkillTest : public Scene {

public:
    /**
     * @brief コンストラクタ
     */
    SkillTest(ISceneChange* scene_change);
    /**
     * @brief デストラクタ
     */
    ~SkillTest();
    /**
     * @brief 初期化
     */
    bool Initialize() override;
    /**
     * @brief 更新
     */
    bool Update() override;
    /**
     * @brief 描画
     */
    void Draw() override;
    /**
     * @brief 終了処理
     */
    void Finalize() override;
private:
    skill::SkillManager skill_manager_;
    system::TurnSystem turn_system_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_SKILL_TEST_H_
