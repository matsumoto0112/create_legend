#include "src/draw/texture_string.h"

#include "src/game/game_device.h"

namespace legend {
namespace draw {

//コンストラクタ
TextureString::TextureString() {}

//デストラクタ
TextureString::~TextureString() { Clear(); }

//初期化
bool TextureString::Init(
    const std::wstring& str, u32 register_num,
    directx::descriptor_heap::heap_parameter::LocalHeapID use_heap_id,
    const std::wstring& font, i32 font_size) {
  this->position_ = math::Vector2::kZeroVector;
  this->scale_ = math::Vector2::kUnitVector;
  this->font_ = font;
  this->font_size_ = font_size;
  this->register_num_ = register_num;
  this->use_heap_id_ = use_heap_id;

  return  Append(str);;
}

//座標を設定する
void TextureString::SetPosition(const math::Vector2& position) {
  this->position_ = position;
  math::Vector2 pos = position;
  for (auto&& ch : chars_) {
    ch.SetPosition(pos);
    pos.x += ch.GetContentSize().x * scale_.x;
  }
}

//スケーリングを設定する
void TextureString::SetScale(const math::Vector2& scale) {
  this->scale_ = scale;
  for (auto&& ch : chars_) {
    ch.SetScale(scale);
  }
  //スケーリングが変わることによって座標がずれるので再設定する
  SetPosition(position_);
}

//描画
void TextureString::Draw(SpriteRenderer& sprite_renderer) {
  for (auto&& ch : chars_) {
    sprite_renderer.AddDrawItems(&ch);
  }
}

//追加
bool TextureString::Append(const std::wstring& str) {
  math::Vector2 position = position_;
  if (chars_.size() != 0) {
    position = chars_.back().GetPosition();
    position.x += chars_.back().GetContentSize().x * chars_.back().GetScale().x;
  }

  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();
  for (auto&& c : str) {
    TextureChar& ch = chars_.emplace_back(TextureChar());
    if (!ch.Init(c, font_, font_size_, register_num_,
                 device.GetLocalHeapHandle(use_heap_id_))) {
      return false;
    }
    ch.SetPosition(position);
    position.x += ch.GetContentSize().x;
  }

  this->str_ += str;
  return true;
}

//クリア
void TextureString::Clear() {
  chars_.clear();
  str_.clear();
}

}  // namespace draw
}  // namespace legend