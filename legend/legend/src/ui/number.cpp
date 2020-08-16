#include "src/ui/number.h"
namespace legend {
namespace ui {

//�R���X�g���N�^
Number::Number() {}

//�f�X�g���N�^
Number::~Number() {}

//������
bool Number::Init(
    const std::wstring& name,
    directx::descriptor_heap::heap_parameter::LocalHeapID heap_id) {
  if (!Image::Init(name, heap_id)) {
    return false;
  }
  SetNumber(0);
  return true;
}

//���l�̐ݒ�
void Number::SetNumber(int num) {
  MY_ASSERTION(0 <= num && num <= 9, L"���l���͈͊O�ł��B");
  const float unit = 0.1f;
  math::Rect rect(unit * num, 0.0f, unit, 1.0f);
  sprite_.SetRect(rect);
  sprite_.SetScale(math::Vector2(unit, 1.0f));
}
}  // namespace ui
}  // namespace legend