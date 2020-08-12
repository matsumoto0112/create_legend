#ifndef LEGEND_SKILL_SKILL_TYPE_H_
#define LEGEND_SKILL_SKILL_TYPE_H_

/**
 * @brief スキルの発動タイミング
 */
enum class SkillActivationTiming {
    NOW = 0,
    PASSIVE,
    PLAYER_TURN_END,
    ENEMY_TURN_END,
};

/**
 * @brief スキル効果の終了タイミング
 */
enum class SkillEffectEndTiming {
    NOW = 0,
    PLAYER_TURN_END,
    ENEMY_TURN_END,
};

#endif //! LEGEND_SKILL_SKILL_TYPE_H_
