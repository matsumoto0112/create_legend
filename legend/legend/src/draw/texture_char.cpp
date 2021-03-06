#include "src/draw/texture_char.h"

#include "src/game/game_device.h"

namespace legend {
namespace draw {

//コンストラクタ
TextureChar::TextureChar() {}

//デストラクタ
TextureChar::~TextureChar() {}

//初期化
bool TextureChar::Init(
    directx::device::CommandList& command_list, wchar_t c,
    const std::wstring& font, i32 font_size,
    const directx::descriptor_heap::DescriptorHandle& handle) {
  std::vector<u8> data;
  u32 width, height;
  if (!CreateChar(command_list, c, font, font_size, &data, &width, &height)) {
    return false;
  }

  auto texture = std::make_shared<directx::buffer::Texture2D>();
  const directx::buffer::Texture2D::Desc desc{
      DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
      width,
      height,
      handle,
      L"TextureChar_" + c,
  };

  if (!texture->Init(game::GameDevice::GetInstance()->GetDevice(), desc)) {
    return false;
  }

  texture->WriteResource(command_list, data.data());
  if (!Sprite2D::Init(
          texture,
          directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID)) {
    return false;
  }
  return true;
}

//文字の作成
bool TextureChar::CreateChar(directx::device::CommandList& command_list,
                             wchar_t c, const std::wstring& font, i32 font_size,
                             std::vector<u8>* data, u32* width, u32* height) {
  //フォント情報の作成
  LOGFONT lf = {font_size,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                SHIFTJIS_CHARSET,
                OUT_TT_ONLY_PRECIS,
                CLIP_DEFAULT_PRECIS,
                PROOF_QUALITY,
                FIXED_PITCH || FF_MODERN};
  wcscpy_s(lf.lfFaceName, font.c_str());
  HFONT fn = CreateFontIndirectW(&lf);
  if (!fn) {
    return false;
  }

  //フォントビットマップ取得
  HDC hdc = GetDC(nullptr);
  HFONT old_font = static_cast<HFONT>(SelectObject(hdc, fn));
  TEXTMETRIC tm;
  GetTextMetricsW(hdc, &tm);
  GLYPHMETRICS gm;
  const MAT2 mat = {{0, 1}, {0, 0}, {0, 0}, {0, 1}};
  DWORD size =
      GetGlyphOutlineW(hdc, c, GGO_GRAY4_BITMAP, &gm, 0, nullptr, &mat);
  BYTE* ptr = new BYTE[size];
  DWORD ret = GetGlyphOutlineW(hdc, c, GGO_GRAY4_BITMAP, &gm, size, ptr, &mat);

  SelectObject(hdc, old_font);
  DeleteObject(fn);
  ReleaseDC(nullptr, hdc);

  const u32 tex_width = gm.gmBlackBoxX + (4 - (gm.gmBlackBoxX % 4)) % 4;
  const u32 tex_height = tm.tmHeight;
  const u32 bmp_w = gm.gmBlackBoxX + (4 - (gm.gmBlackBoxX % 4)) % 4;
  const u32 bmp_h = gm.gmBlackBoxY;
  const int level = 17;

  std::vector<byte> texture_data(tex_width * tex_height * 4);
  for (u32 y = 0; y < bmp_h; y++) {
    for (u32 x = 0; x < bmp_w; x++) {
      const u32 pos = x + bmp_w * y;
      const DWORD alpha = (255 * ptr[pos]) / (level - 1);
      const DWORD color = 0x00ffffff | (alpha << 24);
      //色情報はARGBの順に8bit単位で入っている
      const u8 r = color >> 16 & 0xff;
      const u8 g = color >> 8 & 0xff;
      const u8 b = color >> 0 & 0xff;
      const u8 a = color >> 24 & 0xff;
      texture_data[pos * 4 + 0] = r;
      texture_data[pos * 4 + 1] = g;
      texture_data[pos * 4 + 2] = b;
      texture_data[pos * 4 + 3] = a;
    }
  }
  delete[] ptr;
  ptr = nullptr;

  *width = tex_width;
  *height = tex_height;
  *data = texture_data;
  return true;
}

}  // namespace draw
}  // namespace legend
