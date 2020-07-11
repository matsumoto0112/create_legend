#include "audio_manager.h"

namespace legend {
namespace audio {

legend::audio::AudioManager::AudioManager()
    : master_volume_(1.0f), file_path_(L"../legend/assets/audios/") {}

legend::audio::AudioManager::~AudioManager() {
  //配列に入っているAudioSourceを削除
  base_audiosources_.clear();
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
    MY_LOG(L"サウンドデバイスの作成に失敗しました\n");
    return false;
  }

  // MasteringVoice作成
  if (FAILED(p_xaudio2_->CreateMasteringVoice(&p_xaudio2_mastering_voice_))) {
    MY_LOG(L"マスターボイスの作成に失敗しました\n");
    return false;
  }

  return true;
}

bool AudioManager::LoadWav(std::wstring filename) {
  //既に読み込み済みかチェック
  if (base_audiosources_.find(filename) != base_audiosources_.end()) {
    MY_LOG(L"既に読み込み済みです。");
    return false;
  }

  base_audiosources_[filename] = std::make_unique<AudioSource>();

  // wavの読み込み
  if (!base_audiosources_[filename]->LoadWav(p_xaudio2_, filename)) {
    MY_LOG(L"wavの読み込みに失敗しました。\n");
    base_audiosources_.erase(filename);
    return false;
  }

  return true;
}

i32 AudioManager::Start(std::wstring filename, float volume, bool loop) {
  //指定したファイルが読み込まれているかチェック
  if (base_audiosources_.find(filename) == base_audiosources_.end()) {
    MY_LOG(L"読み込まれていないファイルを再生しようとしました。\n");
    return false;
  }

  audiosources_[play_count_] = std::make_unique<AudioSource>();
  audiosources_[play_count_]->Copy(p_xaudio2_, *base_audiosources_[filename]);
  // audiosources_[play_count_]->LoadWav(p_xaudio2_, filename);
  audiosources_[play_count_]->SetLoopFlag(loop);
  audiosources_[play_count_]->SetVolume(volume, master_volume_);
  audiosources_[play_count_]->SetPitch(1.0f);
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

void AudioManager::Stop(i32 key) {
  //エラーチェック
  if (audiosources_.count(key) == 0) {
    MY_LOG(L"存在しないキーが指定されました。\n");
    return;
  }
  audiosources_[key]->Stop();
}

void AudioManager::SetMasterVolume(float volume) { master_volume_ = volume; }

float AudioManager::GetMasterVolume() { return master_volume_; }

void AudioManager::SetVolume(i32 key, float volume) {
  //エラーチェック
  if (audiosources_.count(key) == 0) {
    MY_LOG(L"存在しないキーが指定されました。\n");
    return;
  }
  audiosources_[key]->SetVolume(volume, master_volume_);
}

void AudioManager::SetLoopFlag(i32 key, bool loop) {
  //エラーチェック
  if (audiosources_.count(key) == 0) {
    MY_LOG(L"存在しないキーが指定されました。\n");
    return;
  }
  audiosources_[key]->SetLoopFlag(loop);
}

void AudioManager::SetPitch(i32 key, float pitch) {
  //エラーチェック
  if (audiosources_.count(key) == 0) {
    MY_LOG(L"存在しないキーが指定されました。\n");
    return;
  }
  audiosources_[key]->SetPitch(pitch);
}

void AudioManager::Update() {
  //空なら何もしない
  if (audiosources_.empty()) return;

  for (auto&& audio : audiosources_) {
    audio.second->Update();
  }

  for (auto itr = audiosources_.begin(); itr != audiosources_.end();) {
    // itr->second->Update();
    //再生し終わっていたら削除
    if (itr->second->IsEnd() && !itr->second->is_loop_) {
      itr = audiosources_.erase(itr);
    } else if (itr->second->IsEnd() && itr->second->is_loop_) {
      LoopStart(itr->first, itr->second->GetFilePath(),
                itr->second->GetVolume(), itr->second->GetPitch());
      ++itr;
    } else {
      ++itr;
    }
  }
}

void AudioManager::LoopStart(i32 key, std::wstring filename, float volume,
                             float pitch) {
  audiosources_[key] = std::make_unique<AudioSource>();
  audiosources_[key]->Copy(p_xaudio2_, *base_audiosources_[filename]);
  // audiosources_[play_count_]->LoadWav(p_xaudio2_, filename);
  audiosources_[key]->SetLoopFlag(true);
  audiosources_[key]->SetVolume(volume, master_volume_);
  audiosources_[key]->SetPitch(pitch);
  audiosources_[key]->Play();
}

}  // namespace audio
}  // namespace legend