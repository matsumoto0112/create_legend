#include "src/search/search_manager.h"

#include <algorithm>
#include <iostream>

#include "src/game/game_device.h"

namespace legend {
namespace search {

SearchManager::SearchManager() {}

SearchManager::~SearchManager() {}

void SearchManager::Initialize(actor::IActorMediator* mediator) {
  mediator_ = mediator;
}

void SearchManager::Make(std::filesystem::path filepath) {
  search_list_.clear();

  auto indexs_ = LoadStringStageData(filepath);

  if (indexs_.empty() || indexs_[0] == "error") {
    MY_LOG(L"データが読み込まれていないか、読み込みに失敗しています。");
    return;
  }

  struct paramater {
    i32 index;
    math::Vector3 pos;
    std::vector<i32> indexs;
  };

  std::vector<paramater> searchs;
  for (auto&& index : indexs_) {
    //文字列を分割
    std::vector<std::string> infomation = StringSplit(index, ',');

    if (infomation[0] == "search") {
      auto x = std::stof(infomation[0].c_str());
      auto y = std::stof(infomation[1].c_str());
      auto z = std::stof(infomation[2].c_str());

      paramater search;
      search.index = static_cast<i32>(searchs.size());
      search.pos = math::Vector3(x, y, z);

      for (i32 i = 3; i < infomation.size(); i++) {
        search.indexs.emplace_back(std::stoi(infomation[i].c_str()));
        Add(search.pos);
      }
      searchs.emplace_back(search);
    }
  }

  for (auto search: searchs) {
    SetBranch(search.index, search.indexs);
  }
}

std::vector<std::string> SearchManager::LoadStringStageData(
    std::filesystem::path filepath) {
  //ファイルパスからテキストデータを読み込み
  std::ifstream ifstream(filepath);
  std::vector<std::string> indexs;
  if (ifstream.fail()) {
    MY_LOG(L"ステージデータを開けませんでした。");
    indexs.push_back("error");
    return indexs;
  }
  std::string index;
  i32 counter = 0;
  while (std::getline(ifstream, index)) {
    indexs.push_back(index);
  }

  return indexs;
}

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
  if (mediator_->GetPlayer() == nullptr) {
    return math::Vector3::kZeroVector;
  }

  ignore_enemy_ = _enemy;
  enemys_ = _enemys;
  auto start = _enemy->GetOwner()->GetTransform().GetPosition();
  auto end = mediator_->GetPlayer()->GetPosition();
  course_list_.clear();

  if (OnCollision(start, end)) {
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

    //*------衝突判定--------
    for (i32 j = 0; j < child.size(); j++) {
      auto sPos = searched[i]->GetBaseSeach()->GetPosition();
      auto ePos = child[i]->GetBaseSeach()->GetPosition();
      if (OnCollision(sPos, ePos)) {
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
    //*------衝突判定--------
    if (OnCollision(start, end)) {
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
    //*------衝突判定--------
    if (OnCollision(_position, end)) {
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
      //*------衝突判定--------
      if (OnCollision(_position, search_list_[i]->GetPosition())) {
        result = search_list_[i].get();
      }
      //*----------------------
    }
  }

  return result;
}

bool SearchManager::OnCollision(math::Vector3 start, math::Vector3 end) {
  // for (i32 index = 0; index < enemys_.size(); index++) {
  // auto enemy = enemys_[index];
  // if (enemy == ignore_enemy_) continue;
  const auto raycast = mediator_->RayCast(start, end);
  auto objs = raycast.m_collisionObjects;

  for (i32 index = 0; index < objs.size(); index++) {
    bullet::Collider* act =
        static_cast<bullet::Collider*>(objs[index]->getUserPointer());
    if (act->GetOwner() == ignore_enemy_->GetOwner()) continue;
    return true;
  }
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