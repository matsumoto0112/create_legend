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
  play_count_ = 0;

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

i32 AudioManager::Play(std::wstring filename, float volume, i32 loop_count) {
  ////指定したファイルが読み込まれているかチェック
  // if (base_audiosources_.find(filename) == base_audiosources_.end()) {
  //  MY_LOG(L"読み込まれていないファイルを再生しようとしました。\n");
  //  return false;
  //}

  audiosources_[play_count_] = std::make_unique<AudioSource>();
  // audiosources_[play_count_]->Copy(*base_audiosources_[filename]);
  audiosources_[play_count_]->LoadWav(p_xaudio2_, filename);
  audiosources_[play_count_]->SetLoopCount(loop_count);
  audiosources_[play_count_]->SetVolume(volume);
  audiosources_[play_count_]->Play();

  //カウントを更新
  play_count_++;

  return play_count_ - 1;
}

void AudioManager::Play(i32 key) {
  //エラーチェック
  if (audiosources_.count(key) == 0) {
    MY_LOG(L"存在しないキーが指定されました。\n");
    return;
  }

  audiosources_[key]->Play();
}

void AudioManager::Pause(i32 key) {
  //エラーチェック
  if (audiosources_.count(key) == 0) {
    MY_LOG(L"存在しないキーが指定されました。\n");
    return;
  }
  audiosources_[key]->Pause();
}

void AudioManager::Update() {
  //空なら何もしない
  if (audiosources_.empty()) return;

  for (auto itr = audiosources_.begin(); itr != audiosources_.end();) {
    itr->second->Update();
    //再生し終わっていたら削除
    if (itr->second->IsEnd()) {
      itr = audiosources_.erase(itr);
    } else {
      ++itr;
    }
  }
}

}  // namespace audio
}  // namespace legend