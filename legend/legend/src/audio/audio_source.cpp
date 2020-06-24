#include "src/audio/audio_source.h"

//コンストラクタ
legend::audio::AudioSource::AudioSource(HWND* window) {
  //サウンドデバイス作成
  DirectSoundCreate8(NULL, &direct_sound_, NULL);

  //協調レベルを設定
  direct_sound_->SetCooperativeLevel(*window, DSSCL_NORMAL);
}

//デストラクタ
legend::audio::AudioSource::~AudioSource() { 

    CloseHandle(direct_sound_);
}
