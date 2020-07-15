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

  return true;
}
}  // namespace draw
}  // namespace legend