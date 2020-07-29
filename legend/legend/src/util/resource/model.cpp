#include "src/util/resource/model.h"

namespace legend {
namespace util {
namespace resource {

Model::Model() {}

Model::~Model() {}

bool Model::Init() {
  resources_.clear();
  return true;
}

bool Model::Load(ModelID key, const std::filesystem::path& filepath) {
  MY_ASSERTION(!IsLoaded(key), L"“o˜^Ï‚İ‚ÌƒL[‚ªÄ“o˜^‚³‚ê‚æ‚¤‚Æ‚µ‚Ä‚¢‚Ü‚·B");

  auto model = std::make_shared<draw::Model>();
  if (!model->Init(filepath)) {
    MY_LOG(L"ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ‚É¸”s‚µ‚Ü‚µ‚½B");
    return false;
  }

  resources_.emplace(key, model);
  return true;
}

bool Model::Unload(ModelID key) {
  MY_ASSERTION(IsLoaded(key), L"–¢“o˜^‚ÌƒL[‚ªíœ‚³‚ê‚æ‚¤‚Æ‚µ‚Ä‚¢‚Ü‚·B");
  return resources_.erase(key) == 1;
}

}  // namespace resource
}  // namespace util
}  // namespace legend