#include "search_ai.h"
#include "src/game/game_device.h"

namespace legend {
namespace search {
SearchAI::SearchAI(math::Vector3 _position) : position_(_position) {}

SearchAI::~SearchAI() {}

math::Vector3 SearchAI::GetPosition() { return position_; }

std::vector<SearchAI*> legend::search::SearchAI::GetBranch() { return branch_; }

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
}  // namespace search
}  // namespace legend