#include "src/search/search_manager.h"

namespace legend {
namespace search {

SearchManager::SearchManager() {}

SearchManager::~SearchManager() {}

math::Vector3 SearchManager::NextSearch(math::Vector3 _position) {
  return math::Vector3();
}

void SearchManager::SetCourse(SearchAI* sStart, SearchAI* sEnd) {}

void SearchManager::ChaseCourse() {}

math::Vector3 SearchManager::NextCourse(math::Vector3 _position) {
  return math::Vector3();
}

std::vector<SearchCourse*> SearchManager::SetChild(
    SearchCourse* course, std::vector<SearchCourse*> searched) {
  return std::vector<SearchCourse*>();
}

SearchAI* SearchManager::GetRandomSearch(std::vector<SearchAI*> remove) {
  return nullptr;
}

}  // namespace search
}  // namespace legend