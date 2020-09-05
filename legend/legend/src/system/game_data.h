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
  struct ResultData {
    GameEndType end_type;
    i32 player_power;
    i32 play_turn;
  };

  struct PlayStageData {
    std::string stage_name;
  };

 public:
  void SetResultData(const ResultData& data) { this->result_data_ = data; }
  ResultData GetResultData() const { return result_data_; }
  void SetPlayStageData(const PlayStageData& data) {
    this->play_stage_data_ = data;
  };
  PlayStageData GetPlayStageData() const { return play_stage_data_; }

 private:
  ResultData result_data_;
  PlayStageData play_stage_data_;

 protected:
  GameDataStorage()
      : result_data_{GameEndType::PLAYER_DEAD, 100, 1},
        play_stage_data_{"tutorial_01"} {}
  virtual ~GameDataStorage() {}
};

}  // namespace system
}  // namespace legend
#endif  //! LEGEND_SYSTEM_GAME_DATA_H
