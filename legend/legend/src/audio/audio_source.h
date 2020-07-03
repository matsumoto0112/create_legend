#ifndef LEGEND_AUDIO_AUDIO_SOURCE_H_
#define LEGEND_AUDIO_AUDIO_SOURCE_H_

#include <xaudio2.h>

#include "src/audio/xaudio2_callback.h"

#pragma comment(lib, "xaudio2.lib")
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
  bool Init(IXAudio2* p_xaudio2, std::wstring filename);
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
  /**
   * @brief �X�V����
   */
  void Update();
  /**
   * @brief �R�s�[
   */
  bool Copy(const AudioSource& other_audiosource);

 public:
  //! ���[�v�Đ����邩�ǂ���
  // bool is_loop_;
  //! �~���[�g���ǂ���
  // bool mute_;
  // !����
  // float volume_;

 private:
  //! �ǂݍ��񂾉���(XAudio2)
  IXAudio2SourceVoice* p_source_voice;
  //! �T�E���h�o�b�t�@(XAudio2)
  XAUDIO2_BUFFER xaudio2_buffer_;
  //! �R�[���o�b�N�N���X(XAudio2)
  XAudio2CallBack callback_;
  //! �I�[�f�B�I�f�o�C�X�̏��(Xaudio2)
  XAUDIO2_VOICE_STATE state_;

  //! MMIO
  HMMIO mmio_;
  //! �g�`�f�[�^�t�H�[�}�b�g
  WAVEFORMATEX wav_format_;
  //! �g�`�f�[�^�o�b�t�@�[
  //BYTE** buffer_;
  unsigned char* buffer_;
  unsigned char* ptr_;
  LONG read_len_;
  //! �g�`�f�[�^�̒���?
  i32 buffer_len_;
  //! �ǂݍ��݃J�E���g
  i32 buffer_count_;

  //! �Đ������ǂ���
  bool is_playing_;
  //! �Đ�����

  //! �ǂݍ��񂾃t�@�C���p�X
  std::wstring file_path_;
};

}  // namespace audio
}  // namespace legend

#endif  //! LEGEND_AUDIO_AUDIO_SOURCE_H_