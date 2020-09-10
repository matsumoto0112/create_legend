#include "audio_manager.h"

namespace legend {
namespace audio {

legend::audio::AudioManager::AudioManager() {
  path_ = util::Path::GetInstance()->exe() / L"assets" / L"audios";
  master_volume_ = 1.0f;
  bgm_volume_ = 0.75f;
  se_volume_ = 1.0f;
}

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

bool AudioManager::LoadWav(std::wstring filename, AudioType audio_type,
                           AudioSplitType audio_split_type) {
  //既に読み込み済みかチェック
  if (base_audiosources_.find(filename) != base_audiosources_.end()) {
    MY_LOG(L"既に読み込み済みです。");
    return false;
  }

  base_audiosources_[filename] = std::make_unique<AudioSource>();

  std::wstring filepath = path_;

  if (!FindDirectory(&filepath, filename)) {
    MY_LOG(L"ファイルが見つけられませんでした。");
    return false;
  }

  // wavの読み込み
  if (!base_audiosources_[filename]->LoadWav(p_xaudio2_, filepath, filename,
                                             audio_type, audio_split_type)) {
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

  float type_volume =
      base_audiosources_[filename]->GetAudioType() == AudioType::BGM
          ? bgm_volume_
          : se_volume_;
  audiosources_[play_count_]->SetVolume(volume, master_volume_ * type_volume);
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
  audiosources_[key]->SetLoopFlag(false);
  audiosources_[key]->Stop();
}

void AudioManager::SetMasterVolume(float volume) {
  master_volume_ = volume;
  for (auto&& audio : audiosources_) {
    SetVolume(audio.first, audio.second->GetVolume());
  }
}

void AudioManager::SetBGMVolume(float volume) {
  bgm_volume_ = volume;
  for (auto&& audio : audiosources_) {
    SetVolume(audio.first, audio.second->GetVolume());
  }
}

void AudioManager::SetSEVolume(float volume) {
  se_volume_ = volume;
  for (auto&& audio : audiosources_) {
    SetVolume(audio.first, audio.second->GetVolume());
  }
}

float AudioManager::GetMasterVolume() { return master_volume_; }

float AudioManager::GetBGMVolume() { return bgm_volume_; }

float AudioManager::GetSEVolume() { return se_volume_; }

void AudioManager::SetVolume(i32 key, float volume) {
  //エラーチェック
  if (audiosources_.count(key) == 0) {
    MY_LOG(L"存在しないキーが指定されました。\n");
    return;
  }

  float type_volume = audiosources_[key]->GetAudioType() == AudioType::BGM
                          ? bgm_volume_
                          : se_volume_;

  audiosources_[key]->SetVolume(volume, master_volume_ * type_volume);
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
      LoopStart(itr->first, itr->second->GetFileName(),
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

bool AudioManager::FindDirectory(std::wstring* filepath,
                                 std::wstring filename) {
  WIN32_FIND_DATA find_data;
  HANDLE h_file = ::FindFirstFile((*filepath + L"\\*.*").c_str(), &find_data);
  if (INVALID_HANDLE_VALUE == h_file) {
    MY_LOG(L"指定されたファイルがありません。");
  }

  do {
    TCHAR* wp_file_name = find_data.cFileName;

    //フォルダの判断
    if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      if (L'.' == wp_file_name[0]) {
        if ((L'\0' == wp_file_name[1]) ||
            (L'.' == wp_file_name[1] && L'\0' == wp_file_name[2])) {
          continue;
        }
      }

      //フルパスの生成
      std::wstring fullpath = *filepath + L"\\" + wp_file_name;

      //再起してサブフォルダを巡回
      if (FindDirectory(&fullpath, filename)) {
        *filepath = fullpath;
        return true;
      }
    } else if (wp_file_name == filename) {
      //フルパスの生成
      *filepath = *filepath + L"\\" + wp_file_name;
      return true;
    }
  } while (::FindNextFile(h_file, &find_data));
  //検索ハンドルを閉じる
  ::FindClose(h_file);
  return false;
}

}  // namespace audio
}  // namespace legend