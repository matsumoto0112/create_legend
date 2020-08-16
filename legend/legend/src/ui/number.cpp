#include "src/ui/number.h"
namespace legend {
namespace ui {

//コンストラクタ
Number::Number() {}

//デストラクタ
Number::~Number() {}

//初期化
bool Number::Init(
    const std::wstring& name,
    directx::descriptor_heap::heap_parameter::LocalHeapID heap_id) {
  if (!Image::Init(name, heap_id)) {
    return false;
  }
  SetNumber(0);
  return true;
}

//数値の設定
void Number::SetNumber(int num) {
  MY_ASSERTION(0 <= num && num <= 9, L"数値が範囲外です。");
  const float unit = 0.1f;
  math::Rect rect(unit * num, 0.0f, unit, 1.0f);
  sprite_.SetRect(rect);
  sprite_.SetScale(math::Vector2(unit, 1.0f));
}
}  // namespace ui
}  // namespace legend