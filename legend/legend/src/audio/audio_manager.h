#ifndef LEGEND_AUDIO_AUDIO_MANAGER_H_
#define LEGEND_AUDIO_AUDIO_MANAGER_H_

/**
 * @file audio_manager.h
 * @brief �����Ǘ��N���X
 */

#include <vector>
#include <xaudio2.h>

#include "src/audio/audio_source.h"

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
  //bool LoadWav(std::wstring filename);
  /**
   * @brief �Đ�����
   * @return �������������I��������true��Ԃ�
   */
  bool Play(std::wstring filename);
  /**
   * @brief �X�V����
   */
  void Update();


 private:
  //! �T�E���h�f�o�C�X
  IXAudio2* p_xaudio2_;
  //! �ŏI���B�n�_
  IXAudio2MasteringVoice* p_xaudio2_mastering_voice_;
  //�ǂݍ���AudioSource��ۑ�
  std::unordered_map<std::wstring, std::unique_ptr<AudioSource>>
      base_audiosources_;
  // AudioSource�z��
  std::vector<std::unique_ptr<AudioSource>> audiosources_;
};

}  // namespace audio
}  // namespace legend

#endif  //! LEGEND_AUDIO_AUDIO_MANAGER_H_
