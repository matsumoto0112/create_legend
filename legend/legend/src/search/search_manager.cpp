#include "src/search/search_manager.h"

#include <algorithm>
#include <iostream>

#include "src/game/game_device.h"

namespace legend {
namespace search {

SearchManager::SearchManager() {}

SearchManager::~SearchManager() {}

void SearchManager::Initialize(bullet::Collider* _player_obb) {
    player_collider_ = _player_obb;
}

void SearchManager::Add(math::Vector3 _position) {
  auto search = std::make_unique<SearchAI>(_position);
  search_list_.emplace_back(std::move(search));
}

void SearchManager::Add(std::vector<math::Vector3> _positions) {
  for (auto p : _positions) {
    Add(p);
  }
}

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

math::Vector3 SearchManager::NextSearch(
    bullet::Collider* _enemy, std::vector<bullet::Collider*> _enemys) {
  if (player_collider_ == nullptr) {
    return math::Vector3::kZeroVector;
  }

  ignore_enemy_ = _enemy;
  enemys_ = _enemys;
  auto start = _enemy->GetOwner()->GetTransform().GetPosition();
  auto end = player_collider_->GetOwner()->GetTransform().GetPosition();
  auto vector = (end - start);
  vector.y = 0;
  course_list_.clear();

  if (OnCollision(start, vector)) {
    SetCourse(NearSearch(start), NearSearch(end));
    ChaseCourse();
  }

  return (course_list_.size() <= 0 ? end : NextCourse(start));
}

void SearchManager::SetCourse(SearchAI* sStart, SearchAI* sEnd) {
  course_list_.clear();
  if ((sStart == nullptr) || (sEnd == nullptr)) return;

  auto start = SearchCourse(sStart);
  auto searched = std::vector<SearchCourse*>{&start};
  auto root = std::vector<SearchCourse*>{};

  for (i32 i = 0; i < searched.size(); i++) {
    auto child = SetChild(searched[i], searched);

    //*------è’ìÀîªíË--------
    for (i32 j = 0; j < child.size(); j++) {
      auto sPos = searched[i]->GetBaseSeach()->GetPosition();
      auto ePos = child[i]->GetBaseSeach()->GetPosition();
      auto vector = (ePos - sPos);
      if (OnCollision(sPos, vector)) {
        searched.emplace_back(child[j]);
      }
    }
    //*----------------------

    std::sort(searched.begin(), searched.end(),
              [](SearchCourse* a, SearchCourse* b) {
                return (a->Length() < b->Length());
              });

    if (searched[i]->GetBaseSeach() == sEnd) {
      root = searched[i]->GetParents();
      break;
    }
  }

  for (auto r : root) {
    course_list_.emplace_back(r->GetBaseSeach());
  }
}

void SearchManager::ChaseCourse() {
  if (course_list_.size() <= 0) return;

  for (i32 i = 0; i < course_list_.size(); i++) {
    if ((course_list_.size() - 1) <= (i + 2)) break;

    auto start = course_list_[i]->GetPosition();
    auto end = course_list_[i + 2]->GetPosition();
    auto vector = (end - start);
    //*------è’ìÀîªíË--------
    if (OnCollision(start, vector)) {
      course_list_.erase(course_list_.begin() + i);
      i--;
    }
    //*----------------------
  }
}

math::Vector3 SearchManager::NextCourse(math::Vector3 _position) {
  if (course_list_.size() <= 0) return _position;

  for (i32 i = 0; i < course_list_.size(); i++) {
    auto end = course_list_[i]->GetPosition();
    auto vector = (end - _position);
    //*------è’ìÀîªíË--------
    if (OnCollision(_position, vector)) {
      if (i <= 0) {
        return course_list_[0]->GetPosition();
      } else {
        return course_list_[i - 1]->GetPosition();
      }
    }
    //*----------------------
  }
  return course_list_[0]->GetPosition();
}

std::vector<SearchCourse*> SearchManager::SetChild(
    SearchCourse* course, std::vector<SearchCourse*> searched) {
  return course->SetChild(searched);
}

SearchAI* SearchManager::GetRandomSearch(std::vector<SearchAI*> remove) {
  std::vector<i32> indexs;

  for (i32 i = 0; i < search_list_.size(); i++) {
    if ((search_list_[i]->GetBranch().size() <= 0) ||
        (std::find(remove.begin(), remove.end(), search_list_[i].get()) !=
         remove.end())) {
      continue;
    }
    indexs.emplace_back(i);
  }

  switch (indexs.size()) {
    case 0:
      return nullptr;
    case 1:
      return search_list_[0].get();
  }

  i32 index = game::GameDevice::GetInstance()->GetRandom().Range(
      0, static_cast<i32>(indexs.size()));
  return search_list_[indexs[index]].get();
}

SearchAI* SearchManager::NearSearch(math::Vector3 _position) {
  if (search_list_.size() <= 0) {
    return nullptr;
  }

  SearchAI* result = nullptr;
  for (i32 i = 0; i < search_list_.size(); i++) {
    if ((result == nullptr) ||
        ((search_list_[i]->GetPosition() - _position).Magnitude() <
         (result->GetPosition() - _position).Magnitude())) {
      auto vector = (search_list_[i]->GetPosition() - _position);
      //*------è’ìÀîªíË--------
      if (OnCollision(_position, vector)) {
        result = search_list_[i].get();
      }
      //*----------------------
    }
  }

  return result;
}

bool SearchManager::OnCollision(math::Vector3 start, math::Vector3 direction) {
  // physics::Ray ray(start, direction);
  // for (i32 index = 0; index < enemys_.size(); index++) {
  //  auto enemy = enemys_[index];
  //  if (enemy == ignore_enemy_) continue;
  //  auto length = direction.Magnitude();
  //  if (physics::Collision::GetInstance()->Collision_Ray_OBB(ray, *enemy,
  //                                                            &length)) {
  //    return true;
  //  }
  //}
  return false;
}

void SearchManager::DebugDraw(directx::device::CommandList& command_list) {
  for (i32 i = 0; i < search_list_.size(); i++) {
    search_list_[i]->DebugDraw(command_list);
  }
}

}  // namespace search
}  // namespace legend