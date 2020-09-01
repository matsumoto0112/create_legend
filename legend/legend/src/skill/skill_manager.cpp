#include "src/skill/skill_manager.h"

#include "src/game/game_device.h"
#include "src/player/player.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace skill {

//�R���X�g���N�^
SkillManager::SkillManager() {}

//�f�X�g���N�^
SkillManager::~SkillManager() {}

//������
void SkillManager::Init(actor::IActorMediator* mediator,
                        player::Player* player) {
  this->mediator_ = mediator;
  player_ = player;
  select_ui_.Init();
  select_move_ = false;
  player_ui_.Init();
  skill_max_count_ = 5;
  current_mode_ = Mode::NONE;
  is_equipment_production_ = false;
  complete_eqquipment_ = false;
}

//�X�L���擾��
void SkillManager::GetSkill(std::shared_ptr<Skill> skill) {
  skill->Init(mediator_, player_);
  this_turn_get_skills_.push_back(skill);
}

//�X�L���̊l��
void SkillManager::AddSkill(std::shared_ptr<Skill> skill) {
  skills_.push_back(skill);
  select_ui_.AddSkill(skill.get());
  player_ui_.AddEquipmentUI(skill.get());
}

//�X�V
void SkillManager::Update() {
  i32 i = 0;
  for (auto&& skill : skills_) {
    skill->Update();
    if (skill->ProductionFlag()) {
      i++;
      continue;
    }

    SetPosition(skill, i);
    i++;
  }

  player_ui_.Update();
}

void SkillManager::EquipmentProductionUpdate() {
  //���̃^�[���ɉ�������Ă��Ȃ���΃^�[����؂�ւ���
  if (this_turn_get_skills_.empty()) {
    mediator_->PlayerCompleteEquipment();
    return;
  }

  //�����ǉ����I���ĂȂ���Ώ㏸��Ԃɐ؂�ւ�
  if (!complete_eqquipment_) {
    player_->GetCollider()->ApplyCentralImpulse(math::Vector3(0, 1, 0));
    current_mode_ = Mode::RISE_PLAYER;
  }

  //���݂̏����X�L�������������E���ȉ��ł���Βǉ�
  if (skills_.size() < skill_max_count_) {
    //�㏸
    if (current_mode_ == Mode::RISE_PLAYER) {
      //���̍����܂ōs�����瑕����ǉ�
      if (player_->GetTransform().GetPosition().y >= 30.0f) {
        if (!complete_eqquipment_) {
          for (auto&& skill : this_turn_get_skills_) {
            AddSkill(skill);
          }
          complete_eqquipment_ = true;
        }
        //���~��Ԃɐ؂�ւ�
        current_mode_ = Mode::FALL_PLAYER;
        player_->GetCollider()->ApplyCentralImpulse(
            player_->GetCollider()->GetVelocity() * -1);
      }
    }
    //���~
    if (current_mode_ == Mode::FALL_PLAYER) {
      //���̍����܂ōs������^�[����؂�ւ���
      if (player_->GetTransform().GetPosition().y <= 1.8f) {
        current_mode_ = Mode::NONE;
        mediator_->PlayerCompleteEquipment();
        this_turn_get_skills_.clear();
        complete_eqquipment_ = false;
      }
    }
  }
}

//�v���C���[�s����̏���
void SkillManager::PlayerTurnEnd() {
  //�v���C���[�̍s����ɏI���X�L���̍X�V����
  for (auto&& skill : skills_) {
    if (!skill->EndSkillProduction()) continue;
    if (skill->GetEndTiming() != SkillEffectEndTiming::PLAYER_TURN_END)
      continue;
    skill->EndAction();
  }

  //�v���C���[�s����ɔ�������X�L���̔���
  for (auto&& skill : skills_) {
    if (!skill->GetUseFlag()) continue;
    if (skill->GetActivetionTiming() != SkillActivationTiming::PLAYER_TURN_END)
      continue;
    skill->Action();
  }
  //���͂Ȃ�
}

//�G�l�~�[�s����̏���
void SkillManager::EnemyTurnEnd() {
  //�G�l�~�[�̍s�����I������ۂɍX�V�������e
  for (auto&& skill : skills_) {
    if (!skill->EndSkillProduction()) continue;
    if (skill->GetEndTiming() != SkillEffectEndTiming::ENEMY_TURN_END) continue;
    skill->EndAction();
  }

  //�G�l�~�[�s����ɔ�������X�L���̔���
  for (auto&& skill : skills_) {
    if (!skill->GetUseFlag()) continue;
    if (skill->GetActivetionTiming() != SkillActivationTiming::ENEMY_TURN_END)
      continue;
    skill->Action();
  }

  //�X�L���̍Ďg�p�^�[���̍X�V
  for (auto&& skill : skills_) {
    skill->RemaingRecastTurnUpdate();
  }
}

//�`��
void SkillManager::Draw() {
  for (auto&& skill : skills_) {
    skill->Draw();
  }
  select_ui_.Draw();
  player_ui_.Draw();
}

//���o����
bool SkillManager::IsProductionNow() {
  for (auto&& skill : skills_) {
    if (skill->ProductionFlag()) return true;
  }
  return false;
}

//�X�L�����X�g�̎擾
std::vector<std::shared_ptr<Skill>> SkillManager::GetSkillList() {
  return skills_;
}

//�X�L���̍폜
void SkillManager::RemoveSkill() {
  for (i32 i = 0; i < skills_.size(); i++) {
    if (skills_[i]->GetRemainingUsableCount() >= 1) continue;

    skills_[i]->RemoveCollider();
    skills_.erase(skills_.begin() + i);
    select_ui_.RemoveSkillUI(i);
    player_ui_.RemoveEquipmentUI(i);
    i--;
    select_ui_.SelectSkillNumber(0);
  }
}

//�X�L���̑I��
bool SkillManager::SelectSkill() {
  if (IsProductionNow()) return true;

  auto& input = game::GameDevice::GetInstance()->GetInput();
  if (input.GetGamepad()->GetButtonDown(input::joy_code::LB)) {
    select_ui_.ChangeIsSelectMode();
  }

  if (select_ui_.GetIsSelectMode()) {
    i32 number = select_ui_.GetSkillNumber();
    //�ړ������1�񂾂��ɂ���
    if (input.GetGamepad()->GetStickLeft().x >= 0.8f && !select_move_) {
      number++;
      select_move_ = true;
    } else if (input.GetGamepad()->GetStickLeft().x <= -0.8f && !select_move_) {
      number--;
      select_move_ = true;
    }
    if (input.GetGamepad()->GetStickLeft().x > -0.8f &&
        input.GetGamepad()->GetStickLeft().x < 0.8f)
      select_move_ = false;
    select_ui_.SelectSkillNumber(number);
    return true;
  } else {
    return false;
  }
}

//�X�L���̎g�p
void SkillManager::UseSkill() {
  if (!select_ui_.GetIsSelectMode() || IsProductionNow()) return;

  auto& input = game::GameDevice::GetInstance()->GetInput();
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  i32 skill_num = select_ui_.GetSkillNumber();
  if (input.GetGamepad()->GetButtonDown(input::joy_code::A)) {
    skills_[skill_num]->Use();
    audio.Start(util::resource::resource_names::audio::SKILL_DECISION, 1.0f);
    if (skills_[skill_num]->GetActivetionTiming() !=
        SkillActivationTiming::NOW) {
      select_ui_.ChangeIsSelectMode();
    }
  }
}

//�X�L���I��
void SkillManager::EndSkill() {
  for (auto&& skill : skills_) {
    if (skill->EndSkillProduction()) {
      if (skill->GetActivetionTiming() == SkillActivationTiming::NOW) {
        select_ui_.ChangeIsSelectMode();
      }
      break;
    }
  }
}

void SkillManager::SetPosition(std::shared_ptr<Skill> skill, i32 skill_num) {
  math::Vector3 pos;
  if (skill_num == 0)
    pos = math::Vector3::kUpVector;
  else if (skill_num == 1)
    pos = math::Vector3::kRightVector * 1.5f;
  else if (skill_num == 2)
    pos = math::Vector3::kLeftVector * 1.5f;
  else if (skill_num == 3)
    pos = math::Vector3::kUpVector + math::Vector3::kRightVector +
          math::Vector3::kBackwardVector;
  else
    pos = math::Vector3::kUpVector + math::Vector3::kLeftVector +
          math::Vector3::kBackwardVector;
  pos = math::Matrix4x4::MultiplyCoord(
      pos, player_->GetTransform().GetRotation().ToMatrix());
  skill->AdjustPosition(pos);
}
}  // namespace skill
}  // namespace legend