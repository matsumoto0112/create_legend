#include "src/util/resource/pipeline.h"

namespace legend {
namespace util {
namespace resource {

//コンストラクタ
Pipeline::Pipeline() {}

//デストラクタ
Pipeline::~Pipeline() {}

//初期化
bool Pipeline::Init() {
  resources_.clear();
  return true;
}

//破棄
bool Pipeline::Unload(PipelineID key) {
  MY_ASSERTION(IsLoaded(key), L"未登録のキーが削除されようとしています。");
  return resources_.erase(key) == 1;
}

}  // namespace resource
}  // namespace util
}  // namespace legend