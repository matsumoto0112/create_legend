#ifndef LEGEND_AUDIO_AUDIO_SOURCE_H_
#define LEGEND_AUDIO_AUDIO_SOURCE_H_

#include <xaudio2.h>

#include "src/audio/audio_type.h"
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
  bool LoadWav(IXAudio2* p_xaudio2, std::wstring filepath,
               std::wstring filename, AudioType audio_type);
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
  /**
   * @brief �X�V����
   */
  void Update();
  /**
   * @brief �Đ������ǂ���
   */
  bool IsEnd() const;
  /**
   * @brief ���ʂ̐ݒ�
   */
  void SetVolume(float volume, float master_volume);
  /**
   * @brief ���ʂ̎擾
   */
  float GetVolume();
  /**
   * @brief ���[�v�t���O�̐ݒ�
   */
  void SetLoopFlag(bool loop);
  /**
   * @brief �s�b�`��ݒ�
   */
  void SetPitch(float pitch);
  /**
   * @brief �s�b�`���擾
   */
  float GetPitch();
  /**
   * @brief �R�s�[
   */
  bool Copy(IXAudio2* p_xaudio2, const AudioSource& other_audiosource);
  /**
   * @brief �t�@�C���p�X���擾
   */
  std::wstring GetFilePath();
  /**
   * @brief �t�@�C���l�[�����擾
   */
  std::wstring GetFileName();

 public:
  //! ���[�v�Đ����邩�ǂ���
  bool is_loop_;
  //! �~���[�g���ǂ���
  // bool mute_;

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
  unsigned char* buffer_;
  unsigned char* ptr_;
  LONG read_len_;
  //! �g�`�f�[�^�̒���?
  i32 buffer_len_;
  //! �ǂݍ��݃J�E���^�\
  i32 buffer_count_;

  //! �Đ������ǂ���
  bool is_playing_;
  //! �ꎞ��~�����ǂ���
  bool is_pause_;
  //! ����
  float volume_;
  //! �s�b�`
  float pitch_;

  //! ���̎��
  AudioType audio_type_;

  //! �ǂݍ��񂾃t�@�C���p�X
  std::wstring file_path_;
  //! �ǂݍ��񂾃t�@�C���l�[��
  std::wstring file_name_;
};

}  // namespace audio
}  // namespace legend

#endif  //! LEGEND_AUDIO_AUDIO_SOURCE_H_