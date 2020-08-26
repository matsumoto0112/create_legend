#include "src/search/search_course.h"
#include "src/game/game_device.h"
#include <algorithm>

namespace legend {
namespace search {

SearchCourse::SearchCourse(actor::IActorMediator* _mediator,
                           SearchAI* _baseSearch)
    : mediator_(_mediator), baseSearch_(_baseSearch), parentSearch_(nullptr) {}

SearchCourse::SearchCourse(actor::IActorMediator* _mediator,
                           SearchAI* _baseSearch, SearchCourse* _parentSearch)
    : mediator_(_mediator),
      baseSearch_(_baseSearch),
      parentSearch_(_parentSearch) {}

SearchCourse::~SearchCourse() {}

SearchAI* SearchCourse::GetBaseSeach() { return baseSearch_; }

void SearchCourse::SetChild(std::vector<SearchCourse>& children,
                            std::vector<SearchCourse>& remove,
                            actor::Actor* ignore) {
  childSearch_.clear();
  std::vector<SearchCourse> searched = {};

  for (auto b : GetBaseSeach()->GetBranch()) {
    bool contains = false;
    for (auto r : remove) {
      if (b == r.GetBaseSeach()) {
        contains = true;
        break;
      }
    }

	if (!contains) {
      searched.emplace_back(SearchCourse(mediator_, b, this));
    }
  }

  std::sort(
      searched.begin(), searched.end(),
      [](SearchCourse a, SearchCourse b) { return (a.Length() < b.Length()); });

  //*------Õ“Ë”»’è--------
  for (i32 i = 0; i < searched.size(); i++) {
    auto sBase = GetBaseSeach();
    auto eBase = searched[i].GetBaseSeach();
    auto sPos = sBase->GetPosition();
    auto ePos = eBase->GetPosition();
    auto length = (sPos - ePos).Magnitude();
    if (!OnCollision(ignore, sPos, ePos)) {
      children.emplace_back(SearchCourse(mediator_, sBase, this));
      childSearch_.emplace_back(SearchCourse(mediator_, sBase, this));
      remove.emplace_back(SearchCourse(mediator_, sBase, this));
    }
  }
  //*----------------------
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
  _parents.emplace_back(parentSearch_);
  return parentSearch_->GetParents(_parents);
}

SearchCourse* SearchCourse::GetMostParent() {
  return (parentSearch_ != nullptr) ? parentSearch_->GetMostParent() : this;
}

const float SearchCourse::Length() {
  auto parents = GetParents();
  float length = 0.0f;
  for (int i = 0; i < parents.size() - 1; i++) {
    auto child = parents[i]->GetBaseSeach();
    auto parent = parents[i + 1]->GetBaseSeach();
    auto cPos = child->GetPosition();
    auto pPos = parent->GetPosition();
    length += (pPos - cPos).Magnitude();
  }
  return length;
}

bool SearchCourse::OnCollision(actor::Actor* ignore,math::Vector3 start,
                               math::Vector3 end){
  const auto raycast = mediator_->RayCast(start, end);
  auto objs = raycast.m_collisionObjects;

  for (i32 index = 0; index < objs.size(); index++) {
    bullet::Collider* act =
        static_cast<bullet::Collider*>(objs[index]->getUserPointer());
    if (act->GetOwner() == ignore) continue;
    return true;
  }
  //}
  return false;
}

}  // namespace search
}  // namespace legend