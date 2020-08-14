#ifndef LEGEND_SKILL_SKILL_TYPE_H_
#define LEGEND_SKILL_SKILL_TYPE_H_

/**
 * @brief �X�L���̔����^�C�~���O
 */
enum class SkillActivationTiming {
    NOW = 0,
    PASSIVE,
    PLAYER_TURN_END,
    ENEMY_TURN_END,
};

/**
 * @brief �X�L�����ʂ̏I���^�C�~���O
 */
enum class SkillEffectEndTiming {
    NOW = 0,
    PLAYER_TURN_END,
    ENEMY_TURN_END,
};

#endif //! LEGEND_SKILL_SKILL_TYPE_H_
