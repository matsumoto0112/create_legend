#include "src/util/resource/model.h"

namespace legend {
namespace util {
namespace resource {

//コンストラクタ
Model::Model() {}

//デストラクタ
Model::~Model() {}

//読み込み
bool Model::Load(ModelID key, const std::filesystem::path& filepath) {
  MY_ASSERTION(!IsLoaded(key), L"登録済みのキーが再登録されようとしています。");

  auto model = std::make_shared<draw::Model>();
  if (!model->Init(filepath)) {
    MY_LOG(L"モデルの読み込みに失敗しました。");
    return false;
  }

  resources_.emplace(key, model);
  return true;
}

}  // namespace resource
}  // namespace util
}  // namespace legend