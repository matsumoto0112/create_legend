#include "src/system/turn_system.h"

namespace legend {
namespace system {

//コンストラクタ
TurnSystem::TurnSystem() : current_turn_(1) {}

//デストラクタ
TurnSystem::~TurnSystem() {}

//ターン数の増加
void TurnSystem::AddCurrentTurn() { current_turn_++; }

//現在のターン数を取得
i32 TurnSystem::GetCurrentTurn() { return current_turn_; }
}  // namespace system
}  // namespace legend