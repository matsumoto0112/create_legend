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
     * @brief �R���X�g���N�^
     */
    SkillTest(ISceneChange* scene_change);
    /**
     * @brief �f�X�g���N�^
     */
    ~SkillTest();
    /**
     * @brief ������
     */
    bool Initialize() override;
    /**
     * @brief �X�V
     */
    bool Update() override;
    /**
     * @brief �`��
     */
    void Draw() override;
    /**
     * @brief �I������
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
