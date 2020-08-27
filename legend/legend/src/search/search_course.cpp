#include "src/search/search_course.h"
#include <algorithm>
#include "src/game/game_device.h"

namespace legend {
namespace search {

SearchCourse::SearchCourse(SearchAI* _baseSearch)
    : baseSearch_(_baseSearch), parentSearch_(-1) {}

SearchCourse::SearchCourse(SearchAI* _baseSearch, i32 _parentSearch)
    : baseSearch_(_baseSearch), parentSearch_(_parentSearch) {}

SearchCourse::~SearchCourse() {}

SearchAI* SearchCourse::GetBaseSearch() { return baseSearch_; }

void SearchCourse::SetChild(std::vector<SearchCourse>& children,
                            std::vector<SearchCourse>& searched) {
  // •ªŠòæ‚Éíœ‘ÎÛ‚ª‚¢‚é‚©Šm”F
  for (auto b : GetBaseSearch()->GetBranch()) {
    bool contains = false;
    for (auto r : searched) {
      if (b == r.GetBaseSearch()) {
        contains = true;
      }
    }

    // íœ‘ÎÛ‚ª‚¢‚éê‡A’Tõæ‚©‚çíœ
    if (contains) {
      for (i32 j = 0; j < children.size(); j++) {
        if (GetBaseSearch() == children[j].GetBaseSearch()) {
          children.erase(children.begin() + j);
          j--;
        }
      }
    }
  }
}

std::vector<i32> SearchCourse::GetParents(std::vector<SearchCourse>& searched) {
  std::vector<i32> _parents;
  return GetParents(searched, _parents);
}

std::vector<i32> SearchCourse::GetParents(std::vector<SearchCourse>& searched,
                                          std::vector<i32>& _parents) {
  if (parentSearch_ < 0) {
    std::reverse(_parents.begin(), _parents.end());
    return _parents;
  }
  _parents.emplace_back(parentSearch_);
  return searched[parentSearch_].GetParents(searched, _parents);
}

float SearchCourse::Length(std::vector<SearchCourse>& searched) {
  auto parents = GetParents(searched);
  float length = 0.0f;
  if (0 < parents.size()) {
    length += (searched[parents[0]].GetBaseSearch()->GetPosition() -
               GetBaseSearch()->GetPosition())
                  .Magnitude();
  }
  for (int i = 0; i < parents.size() - 1; i++) {
    auto child = searched[parents[i]].GetBaseSearch();
    auto parent = searched[parents[i + 1]].GetBaseSearch();
    auto cPos = child->GetPosition();
    auto pPos = parent->GetPosition();
    length += (pPos - cPos).Magnitude();
  }
  return length;
}

}  // namespace search
}  // namespace legend