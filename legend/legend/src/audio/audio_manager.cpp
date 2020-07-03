#include "audio_manager.h"

namespace legend {
namespace audio {

legend::audio::AudioManager::AudioManager() {}

legend::audio::AudioManager::~AudioManager() {
  //配列に入っているAudioSourceを削除
  audiosources_.clear();

  // MasterungVoiceの破棄
  if (p_xaudio2_mastering_voice_ != nullptr) {
    p_xaudio2_mastering_voice_->DestroyVoice();
    p_xaudio2_mastering_voice_ = nullptr;
  }

  // XAudio2の破棄
  if (p_xaudio2_ != nullptr) {
    p_xaudio2_->Release();
    p_xaudio2_ = nullptr;
  }

  // COMの終了
  CoUninitialize();
}

bool AudioManager::Init() {
  // COMの初期化
  if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED))) {
    return false;
  }

  //サウンドデバイス作成
  if (FAILED(XAudio2Create(&p_xaudio2_, 0))) {
    return false;
  }

  // MasteringVoice作成
  if (FAILED(p_xaudio2_->CreateMasteringVoice(&p_xaudio2_mastering_voice_))) {
    return false;
  }

  return true;
}

// bool AudioManager::LoadWav(std::wstring filename) {
//  //既に読み込み済みかチェック
//  if (base_audiosources_.find(filename) != base_audiosources_.end()) {
//    MY_LOG(L"既に読み込み済みです。");
//    return false;
//  }
//
//  base_audiosources_[filename] = std::make_unique<AudioSource>();
//
//  // wavの読み込み
//  if (!base_audiosources_[filename]->Init(p_xaudio2_, filename)) {
//    MY_LOG(L"wavの読み込みに失敗しました。\n");
//    base_audiosources_.erase(filename);
//    return false;
//  }
//
//  return true;
//}

bool AudioManager::Play(std::wstring filename) {
  ////指定したファイルが読み込まれているかチェック
  // if (base_audiosources_.find(filename) == base_audiosources_.end()) {
  //  MY_LOG(L"読み込まれていないファイルを再生しようとしました。\n");
  //  return false;
  //}

  audiosources_.push_back(std::make_unique<AudioSource>());
  // audiosources_[audiosources_.size() -
  // 1]->Copy(*base_audiosources_[filename]);
  audiosources_[audiosources_.size() - 1]->Init(p_xaudio2_, filename);
  audiosources_[audiosources_.size() - 1]->Play();

  return true;
}

void AudioManager::Update() {
  // base_audiosources_[L"../legend/assets/audios/free_3.wav"]->Update();

  for (int i = 0; i < audiosources_.size(); i++) {
    audiosources_[i]->Update();
    if (!audiosources_[i]->IsPlaying()) {
      audiosources_.erase(audiosources_.begin() + i);
      i--;
    }
  }
}

}  // namespace audio
}  // namespace legend