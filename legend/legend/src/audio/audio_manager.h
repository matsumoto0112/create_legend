#ifndef LEGEND_AUDIO_AUDIO_MANAGER_H_
#define LEGEND_AUDIO_AUDIO_MANAGER_H_

/**
 * @file audio_manager.h
 * @brief �����Ǘ��N���X
 */

#include <xaudio2.h>

#include <vector>

#include "src/audio/audio_source.h"
#include "src/util/path.h"

#pragma comment(lib, "dsound.lib")

namespace legend {
namespace audio {

class AudioManager {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  AudioManager();
  /**
   * @brief �f�X�g���N�^
   */
  ~AudioManager();
  /**
   * @brief ����������
   * @return �������������I��������true��Ԃ�
   */
  bool Init(/*HWND* window*/);
  /**
   * @brief wav�ǂݍ���
   * @return �������������I��������true��Ԃ�
   */
  bool LoadWav(std::wstring filename, AudioType audio_type);
  /**
   * @brief �Đ�����
   * @return �������������I��������key��Ԃ�
   * @return �����Ɏ��s������-1��Ԃ�
   */
  i32 Start(std::wstring filename, float volume, bool loop = false);
  /**
   * @brief �Đ�����
   */
  void Play(i32 key);
  /**
   * @brief �|�[�Y�����鏈��
   */
  void Pause(i32 key);
  /**
   * @brief ��~�����鏈��
   */
  void Stop(i32 key);
  /**
   * @brief �}�X�^�[���ʂ̐ݒ�
   */
  void SetMasterVolume(float volume);
  /**
   * @brief �}�X�^�[���ʂ̎擾
   */
  float GetMasterVolume();
  /**
   * @brief ���ʂ̐ݒ�
   */
  void SetVolume(i32 key, float volume);
  /**
   * @brief ���ʂ̐ݒ�
   */
  void SetLoopFlag(i32 key, bool loop);
  /**
   * @brief �s�b�`�̎w��
   */
  void SetPitch(i32 key, float pitch);
  /**
   * @brief �X�V����
   */
  void Update();

private :
    void LoopStart(i32 key, std::wstring filename, float volume, float pitch);

    bool FindDirectory(std::wstring *filepath, std::wstring filename);

 public:
  //! AudioSource�z��
  std::unordered_map<i32, std::unique_ptr<AudioSource>> audiosources_;
  //! �ǂݍ���AudioSource��ۑ�
  std::unordered_map<std::wstring, std::unique_ptr<AudioSource>>
      base_audiosources_;

 private:
  //! �T�E���h�f�o�C�X
  IXAudio2* p_xaudio2_;
  //! �ŏI���B�n�_
  IXAudio2MasteringVoice* p_xaudio2_mastering_voice_;
  //! �Đ������T�E���h��
  i32 play_count_;

  //! �t�@�C���p�X
  std::filesystem::path path_;

  //! �S�̂̉���
  float master_volume_;
};

}  // namespace audio
}  // namespace legend

#endif  //! LEGEND_AUDIO_AUDIO_MANAGER_H_
