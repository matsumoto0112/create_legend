#include "src/system/turn_system.h"

namespace legend {
namespace system {

//�R���X�g���N�^
TurnSystem::TurnSystem() : current_turn_(1) {}

//�f�X�g���N�^
TurnSystem::~TurnSystem() {}

//�^�[�����̑���
void TurnSystem::AddCurrentTurn() { current_turn_++; }

//���݂̃^�[�������擾
i32 TurnSystem::GetCurrentTurn() { return current_turn_; }
}  // namespace system
}  // namespace legend