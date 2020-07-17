#ifndef LEGEND_DRAW_TEXTURE_STRING_H_
#define LEGEND_DRAW_TEXTURE_STRING_H_

/**
 * @file texture_string.h
 * @brief ������e�N�X�`��
 */

#include "src/draw/sprite_renderer.h"
#include "src/draw/texture_char.h"

namespace legend {
namespace draw {

/**
 * @brief ������e�N�X�`��
 */
class TextureString {
  using Chars = std::vector<TextureChar>;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  TextureString();
  /**
   * @brief �f�X�g���N�^
   */
  ~TextureString();

  /**
   * @brief ������
   * @param str ���������镶����
   * @param font �g�p����t�H���g
   * @param font_size �t�H���g�T�C�Y
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(const std::wstring& str, const std::wstring& font = L"���C���I",
            i32 font_size = 32);

  /**
   * @brief ���W��ݒ肷��
   */
  void SetPosition(const math::Vector2& position);
  /**
   * @brief ���W���擾����
   */
  inline math::Vector2 GetPosition() const { return position_; }
  /**
   * @brief �X�P�[�����O��ݒ肷��
   */
  void SetScale(const math::Vector2& scale);
  /**
   * @brief �X�P�[�����O���擾����
   */
  inline math::Vector2 GetScale() const { return scale_; }
  /**
   * @brief �`�悷��
   * @param sprite_renderer �X�v���C�g�`���
   */
  void Draw(SpriteRenderer& sprite_renderer);
  /**
   * @brief ��������������
   * @param str �������镶����
   * @return �����ɐ���������true��Ԃ�
   */
  bool Append(const std::wstring& str);
  /**
   * @brief ��������N���A����
   */
  void Clear();

 private:
  //! �\�����镶����
  std::wstring str_;
  //! ������e�N�X�`��
  Chars chars_;
  //! ���W
  math::Vector2 position_;
  //! �X�P�[�����O
  math::Vector2 scale_;
  //! �t�H���g��
  std::wstring font_;
  //! �t�H���g�̑傫��
  i32 font_size_;
};

}  // namespace draw
}  // namespace legend

#endif  //! LEGEND_DRAW_TEXTURE_STRING_H_
