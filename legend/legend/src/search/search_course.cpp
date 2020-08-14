#include "src/search/search_course.h"
#include "src/game/game_device.h"

namespace legend {
namespace search {

SearchCourse::SearchCourse(SearchAI* _baseSearch)
    : baseSearch_(_baseSearch), parentSearch_(nullptr) {}

SearchCourse::SearchCourse(SearchAI* _baseSearch, SearchCourse* _parentSearch)
    : baseSearch_(_baseSearch), parentSearch_(_parentSearch) {}

SearchCourse::~SearchCourse() {}

SearchAI* SearchCourse::GetBaseSeach() { return baseSearch_; }

std::vector<SearchCourse*> SearchCourse::SetChild(std::vector<SearchCourse*> remove) {
  childSearch_.clear();

  //for (auto b : baseSearch_->GetBranch()) {
  //  if (std::find(remove.begin(), remove.end(), b) != remove.end()) {
  //    continue;
  //  }
  //  childSearch_.emplace_back(&SearchCourse(b, this));
  //}

  return childSearch_;
}

std::vector<SearchCourse*> SearchCourse::GetParents() {
  std::vector<SearchCourse*> _parents;
  return GetParents(_parents);
}

std::vector<SearchCourse*> SearchCourse::GetParents(
    std::vector<SearchCourse*> _parents) {
  if (parentSearch_ == nullptr) {
    std::reverse(_parents.begin(), _parents.end());
    return _parents;
  }
  return GetParents(_parents);
}

SearchCourse* SearchCourse::GetMostParent() {
  return (parentSearch_ != nullptr) ? parentSearch_->GetMostParent() : this;
}

const float SearchCourse::Length() {
  auto parents = GetParents();
  float length = 0.0f;
  for (int i = 0; i < parents.size() - 1; i++) {
    auto child = parents[i]->baseSearch_->GetPosition();
    auto parent = parents[i + 1]->baseSearch_->GetPosition();
    length += (parent - child).Magnitude();
  }
  return length;
}

}  // namespace search
}  // namespace legend