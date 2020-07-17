#ifndef LEGEND_DRAW_TEXTURE_CHAR_H_
#define LEGEND_DRAW_TEXTURE_CHAR_H_

/**
 * @file texture_char.h
 * @brief �t�H���g�e�N�X�`��
 */

#include "src/directx/buffer/texture_2d.h"
#include "src/draw/sprite_2d.h"

namespace legend {
namespace draw {

/**
 * @brief �����e�N�X�`���N���X
 */
class TextureChar : public Sprite2D {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  TextureChar();
  /**
   * @brief �f�X�g���N�^
   */
  ~TextureChar();
  /**
   * @brief ������
   * @param c ���������镶��
   * @param font �t�H���g
   * @param font_size �t�H���g�T�C�Y
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(wchar_t c, const std::wstring& font, i32 font_size);

 private:
  /**
   * @brief �������쐬����
   * @param c ���������镶��
   * @param font �t�H���g
   * @param font_size �t�H���g�T�C�Y
   * @param data �쐬���ꂽ�f�[�^
   * @param width �쐬���ꂽ�e�N�X�`���̕�
   * @param height �쐬���ꂽ�e�N�X�`���̍���
   * @return �쐬�ɐ���������true��Ԃ�
   */
  bool CreateChar(wchar_t c, const std::wstring& font, i32 font_size,
                  std::vector<u8>* data, u32* width, u32* height);
};

}  // namespace draw
}  // namespace legend

#endif  //! LEGEND_DRAW_TEXTURE_CHAR_H_
