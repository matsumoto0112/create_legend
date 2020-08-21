#include "search_ai.h"

#include "src/bullet/bullet_helper.h"

namespace legend {
namespace search {
SearchAI::SearchAI(math::Vector3 _position) : position_(_position) {
  box_ = std::make_unique<primitive::Box>();
  box_->Init(directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY);
}

SearchAI::~SearchAI() {}

math::Vector3 SearchAI::GetPosition() { return position_; }

std::vector<SearchAI*> legend::search::SearchAI::GetBranch() { return branch_; }

std::vector<SearchAI*> SearchAI::SetBranch(std::vector<SearchAI*> _branch) {
  branch_ = _branch;
  return branch_;
}

std::vector<SearchAI*> SearchAI::AddBranch(SearchAI* _branch) {
  if ((std::find(branch_.begin(), branch_.end(), _branch) != branch_.end())) {
    return branch_;
  }
  if ((std::find(branch_.begin(), branch_.end(), this) != branch_.end())) {
    return branch_;
  }

  branch_.emplace_back(_branch);

  auto line = std::make_unique<primitive::Line>();
  line->Init(directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY);
  lines_.emplace_back(std::move(line));
  return branch_;
}

SearchAI* legend::search::SearchAI::GetRandomSearch(
    std::vector<SearchAI*> remove) {
  std::vector<SearchAI*> result;
  for (auto s : branch_) {
    if (std::find(remove.begin(), remove.end(), s) != remove.end()) {
      continue;
    }
    result.emplace_back(s);
  }

  if (result.size() <= 0) {
    return nullptr;
  }

  i32 index = game::GameDevice::GetInstance()->GetRandom().Range(
      0, static_cast<i32>(result.size()));
  return result[index];
}
void SearchAI::DebugDraw(bullet::PhysicsField* physics_field_) {
  for (auto b : branch_) {
    auto from = bullet::helper::TobtVector3(GetPosition());
    auto to = bullet::helper::TobtVector3(b->GetPosition());
    auto color = bullet::helper::TobtVector3(math::Vector3(1, 0, 0));
    physics_field_->DrawLine(from, to, color);
  }
}
}  // namespace search
}  // namespace legend