#include "src/search/search_manager.h"
#include "src/game/game_device.h"

#include <algorithm>
#include <iostream>

namespace legend {
namespace search {

SearchManager::SearchManager() {
}

SearchManager::~SearchManager() {}

void SearchManager::Add(math::Vector3 _position) {
  auto search = std::make_unique<SearchAI>(_position);
  searchList.emplace_back(std::move(search));
}

void SearchManager::Add(std::vector<math::Vector3> _positions) {
  for (auto p : _positions) {
    Add(p);
  }
}

math::Vector3 SearchManager::NextSearch(math::Vector3 _position) {
  auto start = _position;
  return math::Vector3();
}

void SearchManager::SetCourse(SearchAI* sStart, SearchAI* sEnd) {
  courseList.clear();
  if ((sStart == nullptr) || (sEnd == nullptr)) return;

  auto start = SearchCourse(sStart);
  auto searched = std::vector<SearchCourse*>{&start};
  auto root = std::vector<SearchCourse*>{};

  for (i32 i = 0; i < searched.size(); i++) {
    auto child = SetChild(searched[i], searched);

    for (i32 j = 0; j < child.size(); j++) {
      auto sPos = searched[i]->GetBaseSeach()->GetPosition();
      auto ePos = child[i]->GetBaseSeach()->GetPosition();
      //*------è’ìÀîªíË--------
      //*
      //*----------------------
    }

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
    courseList.emplace_back(r->GetBaseSeach());
  }
}

void SearchManager::ChaseCourse() {
  if (courseList.size() <= 0) return;

  for (i32 i = 0; i < courseList.size(); i++) {
    if ((courseList.size() - 1) <= (i + 2)) break;

    auto start = courseList[i]->GetPosition();
    auto end = courseList[i + 2]->GetPosition();
    auto vector = (end - start);
    //*------è’ìÀîªíË--------
    //*
    //*----------------------
  }
}

math::Vector3 SearchManager::NextCourse(math::Vector3 _position) {
  if (courseList.size() <= 0) return _position;

  for (i32 i = 0; i < courseList.size(); i++) {
    auto end = courseList[i]->GetPosition();
    auto vector = (end - _position);
    //*------è’ìÀîªíË--------
    //*
    //*----------------------
  }
  return courseList[0]->GetPosition();
}

std::vector<SearchCourse*> SearchManager::SetChild(
    SearchCourse* course, std::vector<SearchCourse*> searched) {
  return course->SetChild(searched);
}

SearchAI* SearchManager::GetRandomSearch(std::vector<SearchAI*> remove) {
  std::vector<i32> indexs;

  for (i32 i = 0; i < searchList.size(); i++) {
    if ((searchList[i]->GetBranch().size() <= 0) ||
        (std::find(remove.begin(), remove.end(), searchList[i].get()) !=
               remove.end())) {
      continue;
    }
    indexs.emplace_back(i);
  }

  switch (indexs.size()) {
    case 0:
      return nullptr;
    case 1:
      return searchList[0].get();
  }

  i32 index = game::GameDevice::GetInstance()->GetRandom().Range(
      0, static_cast<i32>(indexs.size()));
  return searchList[indexs[index]].get();
}

SearchAI* SearchManager::NearSearch(math::Vector3 _position) {
  if (searchList.size() <= 0) {
    return nullptr;
  }

  SearchAI* result = nullptr;
  for (i32 i = 0; i < searchList.size(); i++) {
    if ((result == nullptr) ||
        ((searchList[i]->GetPosition() - _position).Magnitude() <
         (result->GetPosition() - _position).Magnitude())) {
      auto vector = (searchList[i]->GetPosition() - _position);
      //*------è’ìÀîªíË--------
      //*
      //*----------------------
    }
  }

  return result;
}

}  // namespace search
}  // namespace legend