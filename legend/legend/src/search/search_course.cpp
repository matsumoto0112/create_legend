#include "src/search/search_course.h"
#include <algorithm>
#include "src/game/game_device.h"

namespace legend {
namespace search {

// コンストラクタ
SearchCourse::SearchCourse(SearchAI* _baseSearch)
    : baseSearch_(_baseSearch), parentSearch_(-1) {}

// コンストラクタ
SearchCourse::SearchCourse(SearchAI* _baseSearch, i32 _parentSearch)
    : baseSearch_(_baseSearch), parentSearch_(_parentSearch) {}

// デスクトラクタ
SearchCourse::~SearchCourse() {}

// 原点取得
SearchAI* SearchCourse::GetBaseSearch() { return baseSearch_; }

// 探索先設定
void SearchCourse::SetChild(std::vector<SearchCourse>& children,
                            std::vector<SearchCourse>& searched) {
  // 分岐先に削除対象がいるか確認
  for (auto b : GetBaseSearch()->GetBranch()) {
    bool contains = false;
    for (auto r : searched) {
      if (b == r.GetBaseSearch()) {
        contains = true;
      }
    }

    // 削除対象がいる場合、探索先から削除
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

// 原点までの分岐元取得
std::vector<i32> SearchCourse::GetParents(std::vector<SearchCourse>& searched) {
  std::vector<i32> _parents;
  return GetParents(searched, _parents);
}

// 分岐元の原点までの距離
std::vector<i32> SearchCourse::GetParents(std::vector<SearchCourse>& searched,
                                          std::vector<i32>& _parents) {
  if (parentSearch_ < 0) {
    std::reverse(_parents.begin(), _parents.end());
    return _parents;
  }
  _parents.emplace_back(parentSearch_);
  return searched[parentSearch_].GetParents(searched, _parents);
}

// 分岐元の原点までの距離
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