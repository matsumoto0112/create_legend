#include "src/audio/audio_source.h"

//�R���X�g���N�^
legend::audio::AudioSource::AudioSource()
    : direct_sound_(nullptr),
      is_playing_(false),
      is_loop_(false),
      mute_(false),
      volume_(1.0f) {}

//�f�X�g���N�^
legend::audio::AudioSource::~AudioSource() { CloseHandle(&direct_sound_); }

bool legend::audio::AudioSource::Init(HWND* window) {
  //�T�E���h�f�o�C�X�쐬
  if (DirectSoundCreate8(NULL, &direct_sound_, NULL) != S_OK) return false;

  //�������x����ݒ�
  if (direct_sound_->SetCooperativeLevel(*window, DSSCL_NORMAL) != DS_OK)
    return false;

  //�v���C�}���T�E���h�o�b�t�@�̍쐬
  DSBUFFERDESC direct_sound_buffer_;
  memset(&direct_sound_buffer_, 0, sizeof(direct_sound_buffer_));
  direct_sound_buffer_.dwSize = sizeof(direct_sound_buffer_);
  direct_sound_buffer_.dwFlags = DSBCAPS_GLOBALFOCUS;
  direct_sound_buffer_.dwBufferBytes = 0;
  direct_sound_buffer_.lpwfxFormat = NULL;
  //if (direct_sound_->CreateSoundBuffer(&direct_sound_buffer_,
  //    &direct_sound_buffer_, NULL))
  //  ;
}

bool legend::audio::AudioSource::Play() { return false; }

void legend::audio::AudioSource::Pause() {}

void legend::audio::AudioSource::Stop() {}
