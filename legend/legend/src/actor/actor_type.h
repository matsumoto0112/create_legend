#ifndef LEGEND_ACTOR_ACTOR_TYPE_H
#define LEGEND_ACTOR_ACTOR_TYPE_H

/**
 * @file actor_type.h
 * @brief
 */
namespace legend {
namespace actor {

/**
 * @brief ����̃Q�[���ɓo�ꂷ��A�N�^�[�̎�ޒ�`
 */

enum class ActorType {
  PLAYER,
  ENEMY,
  BOSS,
  DESK,
  FRAGMENT,
  STATIONERY,
  OBSTACLE,
};

}  // namespace actor
}  // namespace legend

#endif  //! LEGEND_ACTOR_ACTOR_TYPE_H
