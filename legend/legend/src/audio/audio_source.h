#ifndef LEGEND_AUDIO_AUDIO_SOURCE_H_
#define LEGEND_AUDIO_AUDIO_SOURCE_H_

#include <dsound.h>

#pragma comment(lib, "dsound.lib")

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
  bool Init(HWND* window);
  /**
   * @brief �Đ�
   * @return �Đ��ɐ���������true��Ԃ�
   */
  bool Play();
  /**
   * @brief �ꎞ��~
   */
  void Pause();
  /**
   * @brief ��~
   */
  void Stop();

 public:
  //! ���[�v�Đ����邩�ǂ���
  bool is_loop_;
  //! �~���[�g���ǂ���
  bool mute_;
  // !����
  float volume_;

 private:
  //! �T�E���h�f�o�C�X
  IDirectSound8* direct_sound_;
  //�v���C�}���T�E���h�o�b�t�@
  //LPDIRECTSOUNDBUFFER* direct_sound_buffer_;
  //! �Đ������ǂ���
  bool is_playing_;
};

}  // namespace audio
}  // namespace legend

#endif  //! LEGEND_AUDIO_AUDIO_SOURCE_H_
