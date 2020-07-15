#include "src/draw/texture_string.h"

namespace legend {
namespace draw {

TextureString::TextureString() {}
TextureString::~TextureString() {}

bool TextureString::Init(const std::wstring& str) {
  math::Vector2 position = math::Vector2::kZeroVector;
  for (auto&& c : str) {
    TextureChar& ch = chars_.emplace_back(TextureChar());
    if (!ch.Init(c)) {
      return false;
    }
    ch.SetPosition(position);
    position.x += ch.GetContentSize().x;
  }

  this->str_ = str;
  this->position_ = math::Vector2::kZeroVector;
  this->scale_ = math::Vector2::kUnitVector;
  return true;
}
void TextureString::SetPosition(const math::Vector2& position) {
  this->position_ = position;
  math::Vector2 pos = position;
  for (auto&& ch : chars_) {
    ch.SetPosition(pos);
    pos.x += ch.GetContentSize().x * scale_.x;
  }
}

void TextureString::SetScale(const math::Vector2& scale) {
  this->scale_ = scale;
  for (auto&& ch : chars_) {
    ch.SetScale(scale);
  }
  SetPosition(position_);
}

void TextureString::Draw(SpriteRenderer& sprite_renderer) {
  for (auto&& ch : chars_) {
    sprite_renderer.AddDrawItems(&ch);
  }
}

bool TextureString::Append(const std::wstring& str) {
  math::Vector2 position = position_;
  if (chars_.size() != 0) {
    position = chars_.back().GetPosition();
    position.x += chars_.back().GetContentSize().x * chars_.back().GetScale().x;
  }

  for (auto&& c : str) {
    TextureChar& ch = chars_.emplace_back(TextureChar());
    if (!ch.Init(c)) {
      return false;
    }
    ch.SetPosition(position);
    position.x += ch.GetContentSize().x;
  }

  this->str_ += str;
  return true;
}

void TextureString::Clear() {
  chars_.clear();
  str_.clear();
}

}  // namespace draw
}  // namespace legend