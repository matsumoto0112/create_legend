#ifndef LEGEND_SYSTEM_GAME_DATA_H
#define LEGEND_SYSTEM_GAME_DATA_H

/**
 * @file game_data.h
 * @brief
 */

#include "src/util/singleton.h"

namespace legend {
namespace system {
class GameDataStorage : public util::Singleton<GameDataStorage> {
 public:
  enum class GameEndType {
    PLAYER_DEAD,
    BOSS_KILLED,
  };
  struct GameData {
    GameEndType end_type;
    i32 play_turn;
  };

 public:
  void Set(const GameData& data) { this->data_ = data; }
  GameData Get() const { return data_; }

 private:
  GameData data_;

 protected:
  GameDataStorage() {}
  virtual ~GameDataStorage() {}
};

}  // namespace system
}  // namespace legend
#endif  //! LEGEND_SYSTEM_GAME_DATA_H
