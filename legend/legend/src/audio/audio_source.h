#ifndef LEGEND_AUDIO_AUDIO_SOURCE_H_
#define LEGEND_AUDIO_AUDIO_SOURCE_H_

#include <dsound.h>
#include <src/util/debug.h>

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")

namespace legend {
namespace audio {
/**
 * @file audio_source.h
 * @brief �I�[�f�B�I�Đ��N���X
 */
class AudioSource {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  AudioSource();
  /**
   * @brief �f�X�g���N�^
   */
  ~AudioSource();
  /**
   * @brief ����������
   * @return �������������I��������true��Ԃ�
   */
  bool Init(HWND* window, const wchar_t* file);
  /**
   * @brief �Đ�
   * @return �Đ��ɐ���������true��Ԃ�
   */
  bool Play();
  /**
   * @brief �ꎞ��~
   */
  // void Pause();
  /**
   * @brief ��~
   */
  void Stop();

 public:
  //! ���[�v�Đ����邩�ǂ���
  // bool is_loop_;
  //! �~���[�g���ǂ���
  // bool mute_;
  // !����
  // float volume_;

 private:
  /**
   * @brief �v���C�}���o�b�t�@�̍쐬
   * @return �������������I��������true��Ԃ�
   */
  bool CreatePrimaryBuffer();
  /**
   * @brief �T�E���h�o�b�t�@�̍쐬�̍쐬
   * @return �������������I��������true��Ԃ�
   */
  bool CreateSoundBuffer(const wchar_t* file);

 private:
  //! �T�E���h�f�o�C�X
  IDirectSound8* directsound_;
  //! �v���C�}���T�E���h�o�b�t�@
  LPDIRECTSOUNDBUFFER primary_;
  //! �Z�J���_���T�E���h�o�b�t�@
  LPDIRECTSOUNDBUFFER secondary_;
  //! �Đ������ǂ���
  bool is_playing_;
  //! �Đ�����

  //! �ǂݍ��񂾃t�@�C���p�X
  const wchar_t* file_path_;
};

}  // namespace audio
}  // namespace legend

#endif  //! LEGEND_AUDIO_AUDIO_SOURCE_H_