#include "src/audio/audio_source.h"

//�R���X�g���N�^
legend::audio::AudioSource::AudioSource(HWND* window) {
  //�T�E���h�f�o�C�X�쐬
  DirectSoundCreate8(NULL, &direct_sound_, NULL);

  //�������x����ݒ�
  direct_sound_->SetCooperativeLevel(*window, DSSCL_NORMAL);
}

//�f�X�g���N�^
legend::audio::AudioSource::~AudioSource() { 

    CloseHandle(direct_sound_);
}
