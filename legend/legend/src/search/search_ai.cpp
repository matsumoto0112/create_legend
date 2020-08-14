#include "search_ai.h"

namespace legend {
namespace search {
SearchAI::SearchAI(math::Vector3 _position) : position_(_position) {}

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
  line->Init();
  lines.emplace_back(std::move(line));
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
void SearchAI::DebugDraw(directx::device::CommandList& command_list) {
  for (i32 index = 0; index < branch_.size();index++) {
    auto end = branch_[index]->GetPosition();
    auto vector = (end - GetPosition());
    lines[index]->SetTransform(util::Transform(position_,
                                            math::Quaternion::FromEular(vector),
                                            math::Vector3::kUnitVector * 100));
    lines[index]->Render(command_list);
  }
}
}  // namespace search
}  // namespace legend