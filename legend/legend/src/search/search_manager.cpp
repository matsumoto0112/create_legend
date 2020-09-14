#include "src/search/search_manager.h"

#include <algorithm>
#include <iostream>

#include "src/enemy/boss.h"
#include "src/enemy/enemy.h"
#include "src/enemy/enemy_type.h"
#include "src/game/game_device.h"
#include "src/object/fragment.h"
#include "src/skill/skill_item_box.h"

namespace legend {
namespace search {

// �R���X�g���N�^
SearchManager::SearchManager() {}

// �f�X�N�g���N�^
SearchManager::~SearchManager() {
  auto searchCount = search_list_.size();
  for (i32 i = 0; i < searchCount; i++) {
    search_list_.erase(search_list_.begin());
  }
}

// �����ݒ�
void SearchManager::Initialize(actor::IActorMediator* mediator) {
  mediator_ = mediator;
}

// ����
void SearchManager::Make(std::filesystem::path filepath) {
  search_list_.clear();

  // �T���ӏ��e�L�X�g�擾
  auto indexs_ = LoadStringStageData(filepath);
  if (indexs_.empty() || indexs_[0] == "error") {
    MY_LOG(L"�f�[�^���ǂݍ��܂�Ă��Ȃ����A�ǂݍ��݂Ɏ��s���Ă��܂��B");
    return;
  }

  // �T���ӏ��p�����[�^
  struct paramater {
    //! �ԍ�
    i32 index;
    //! ���W
    math::Vector3 pos;
    //! �����ԍ�
    std::vector<i32> indexs;
  };

  std::vector<paramater> searchs;
  for (auto&& index : indexs_) {
    //������𕪊�
    std::vector<std::string> infomation = StringSplit(index, ',');

    if (infomation[0] == "search") {
      paramater search;
      auto x = std::stof(infomation[1].c_str());
      auto y = std::stof(infomation[2].c_str());
      auto z = std::stof(infomation[3].c_str());

      search.index = static_cast<i32>(searchs.size());
      search.pos = math::Vector3(x, y, z);

      for (i32 i = 4; i < infomation.size(); i++) {
        search.indexs.emplace_back(std::stoi(infomation[i].c_str()));
      }
      Add(search.pos);
      searchs.emplace_back(search);
    }
  }

  // �����ݒ�
  for (auto search : searchs) {
    SetBranch(search.index, search.indexs);
  }
}

// �e�L�X�g�̓ǂݍ��ݏ���
std::vector<std::string> SearchManager::LoadStringStageData(
    std::filesystem::path filepath) {
  //�t�@�C���p�X����e�L�X�g�f�[�^��ǂݍ���
  std::ifstream ifstream(filepath);
  std::vector<std::string> indexs;
  if (ifstream.fail()) {
    MY_LOG(L"�X�e�[�W�f�[�^���J���܂���ł����B");
    indexs.push_back("error");
    return indexs;
  }
  std::string index;
  while (std::getline(ifstream, index)) {
    indexs.push_back(index);
  }

  return indexs;
}

// String��String���X�g�֕ϊ�
std::vector<std::string> SearchManager::StringSplit(const std::string& string,
                                                    char border) {
  std::vector<std::string> elements;
  std::stringstream ss(string);
  std::string item;

  while (std::getline(ss, item, border)) {
    if (item.empty()) continue;

    elements.push_back(item);
  }

  return elements;
}

// ���W���番��_�ǉ�
void SearchManager::Add(math::Vector3 _position) {
  auto search = std::make_unique<SearchAI>(_position);
  search_list_.emplace_back(std::move(search));
}

// ���W���X�g���番��_�ǉ�
void SearchManager::Add(std::vector<math::Vector3> _positions) {
  for (auto p : _positions) {
    Add(p);
  }
}

// ���W���番��_�ǉ�
void SearchManager::SetBranch(i32 index, std::vector<i32> branch) {
  if (index < 0 || search_list_.size() <= index) {
    return;
  }
  for (i32 i = 0; i < branch.size(); i++) {
    if (branch[i] < 0 || search_list_.size() <= branch[i]) {
      return;
    }
    auto add = search_list_[branch[i]].get();
    search_list_[index].get()->AddBranch(add);
  }
}

// ���̌o�H���W
math::Vector3 SearchManager::NextSearch(
    bullet::Collider* _enemy, std::vector<bullet::Collider*> _enemys) {
  if (mediator_->GetPlayer() == nullptr) {
    return math::Vector3::kZeroVector;
  }

  ignore_enemy_ = _enemy;
  enemys_ = _enemys;
  auto start = _enemy->GetOwner()->GetTransform().GetPosition();
  auto end = mediator_->GetPlayer()->GetPosition();
  auto point = start + (end - start) / 2.0f;
  course_list_.clear();

  // �Փ˂�����T���o�H
  if (OnCollision(start, end) ||
      !OnCollision(point, point + math::Vector3::kDownVector * 15.0f)) {
    SetCourse(NearSearch(start), NearSearch(end));
    ChaseCourse();
    end = (course_list_.size() <= 0 ? end : course_list_[0]->GetPosition());
  }

  return end;
}

// �o�H�T��
void SearchManager::SetCourse(SearchAI* sStart, SearchAI* sEnd) {
  // �o�H������
  course_list_.clear();
  if ((sStart == nullptr) || (sEnd == nullptr)) return;

  //! �n�_
  auto start = SearchCourse(sStart);
  //! �o�H���X�g
  auto searched = std::vector<SearchCourse>{start};
  //! �o�H�ԍ����X�g
  auto root = std::vector<i32>{};

  // �T������
  for (i32 i = 0; i < searched.size(); i++) {
    // �T������擾
    std::vector<SearchCourse> children = {};
    auto branch = GetBaseSearch(searched[i].GetBaseSearch())->GetBranch();
    for (auto b : branch) {
      children.emplace_back(SearchCourse(GetBaseSearch(b), i));
    }

    // �폜�Ώۂɔ��ꍇ�A�T���悩��폜
    for (i32 s = 0; s < searched.size(); s++) {
      for (i32 j = 0; j < children.size(); j++) {
        auto search = GetBaseSearch(searched[s].GetBaseSearch());
        auto child = GetBaseSearch(children[j].GetBaseSearch());
        if (search == child) {
          children.erase(children.begin() + j);
          j--;
        }
      }
    }

    // ��������\�[�g
    auto indexs = std::vector<i32>{};
    for (i32 c_index = 0; c_index < children.size(); c_index++) {
      bool is_size = (indexs.size() <= 0);
      if (indexs.size() <= 0) {
        indexs.emplace_back(c_index);
      } else {
        for (i32 i_index = 0; i_index < indexs.size(); i_index++) {
          float i_length = (children[indexs[i_index]].Length(searched));
          float c_length = children[c_index].Length(searched);
          if (c_length < i_length) {
            indexs.insert(indexs.begin() + i_index, c_index);
            break;
          } else if (i_index + 1 <= indexs.size()) {
            indexs.emplace_back(c_index);
            break;
          }
        }
      }
    }

    // �Փ˔���
    for (i32 j = 0; j < children.size(); j++) {
      auto c_index = indexs[j];
      auto sPos = searched[i].GetBaseSearch()->GetPosition();
      auto ePos = children[c_index].GetBaseSearch()->GetPosition();
      auto length = children[c_index].Length(searched);
      // �Փ˂��Ȃ�������T���ӏ��ɒǉ�
      if (!OnCollision(sPos, ePos)) {
        searched.emplace_back(
            SearchCourse(GetBaseSearch(children[j].GetBaseSearch()), i));
      }
    }

    // �ŏI�n�_�Ɣ�����烋�[�g�ɒǉ�
    if (GetBaseSearch(searched[i].GetBaseSearch()) == GetBaseSearch(sEnd)) {
      root = searched[i].GetParents(searched);
      break;
    }
  }

  // ���[�g�ɒǉ�����Ă��镪��_�̍��W�����[�g�ɒǉ�
  for (auto r : root) {
    course_list_.emplace_back(GetBaseSearch(searched[r].GetBaseSearch()));
  }
}

// �ǐՌo�H
void SearchManager::ChaseCourse() {
  if (course_list_.size() <= 0) return;

  // ���΂��ɏՓ˔�����s���A�Փ˂��Ȃ���ΊԂ̍��W���Ȃ�
  auto start = ignore_enemy_->GetOwner()->GetTransform().GetPosition();
  for (i32 i = 1; i < course_list_.size(); i++) {
    auto end = course_list_[i]->GetPosition();
    auto point = start + (end - start) / 2.0f;
    // �Փ˔���
    if (!OnCollision(start, end) &&
        OnCollision(point, point + math::Vector3::kDownVector * 15.0f)) {
      course_list_.erase(course_list_.begin() + i - 1);
      i--;
    }
  }
}

// ���W����ߏ�̒T���ӏ����擾
SearchAI* SearchManager::NearSearch(math::Vector3 _position) {
  if (search_list_.size() <= 0) {
    return nullptr;
  }

  // ���g�̍��W�ɋ߂��A�ԂɏՓ˓X���Ȃ����̂��擾
  SearchAI* result = nullptr;
  for (i32 i = 0; i < search_list_.size(); i++) {
    if ((result == nullptr) ||
        ((search_list_[i]->GetPosition() - _position).Magnitude() <
         (result->GetPosition() - _position).Magnitude())) {
      // �Փ˔���
      if (!OnCollision(_position, search_list_[i]->GetPosition())) {
        result = search_list_[i].get();
      }
    }
  }

  return result;
}

// ���_�̃|�C���^���擾
SearchAI* SearchManager::GetBaseSearch(SearchAI* _search) {
  // �|�C���^���擾
  for (auto& s : search_list_) {
    if (s.get() == _search) return s.get();
  }
  return nullptr;
}

// �Փˏ����擾
bool SearchManager::OnCollision(math::Vector3 start, math::Vector3 end) {
  // �Փ˔���
  const auto raycast = mediator_->RayCast(start, end);
  auto objs = raycast.m_collisionObjects;

  // �ړ�����
  enemy::enemy_type::MoveType move_type;
  auto ea = dynamic_cast<enemy::EnemyActor*>(ignore_enemy_->GetOwner());
  if (ea != nullptr) {
    move_type = ea->GetMoveType();
  }

  for (i32 index = 0; index < objs.size(); index++) {
    bullet::Collider* act =
        static_cast<bullet::Collider*>(objs[index]->getUserPointer());
    // �ғ����̓G�Ƃ̏Փ˂𖳎�
    if (act->GetOwner() == ignore_enemy_->GetOwner()) continue;
    // �v���C���[�Ƃ̏Փ˂𖳎�
    if (act->GetOwner() == mediator_->GetPlayer()->GetCollider()->GetOwner())
      continue;
    // �G�����i���Ă����ꍇ�A�ق��̓G�Ƃ̏Փ˂𖳎�
    if (move_type == enemy::enemy_type::Move_Straight) {
      if (dynamic_cast<enemy::EnemyActor*>(act->GetOwner()) != nullptr) {
        continue;
      }
    }
    // �X�L���Ƃ̏Փ˂𖳎�
    if (dynamic_cast<skill::Skill*>(act->GetOwner()) != nullptr) {
      continue;
    }
    // �X�L���{�b�N�X�Ƃ̏Փ˂𖳎�
    if (dynamic_cast<skill::SkillItemBox*>(act->GetOwner()) != nullptr) {
      continue;
    }

	// �����J�X�Ƃ̏Փ˂𖳎�
    if (dynamic_cast<object::Fragment*>(act->GetOwner()) != nullptr) {
      continue;
    }
    return true;
  }
  return false;
}

// �f�o�b�O�`��
void SearchManager::DebugDraw(bullet::PhysicsField* physics_field_) {
  for (i32 i = 0; i < search_list_.size(); i++) {
    search_list_[i]->DebugDraw(physics_field_);
  }
}

}  // namespace search
}  // namespace legend