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
  //�z��ɓ����Ă���AudioSource���폜
  base_audiosources_.clear();
  audiosources_.clear();

  // MasterungVoice�̔j��
  if (p_xaudio2_mastering_voice_ != nullptr) {
    p_xaudio2_mastering_voice_->DestroyVoice();
    p_xaudio2_mastering_voice_ = nullptr;
  }

  // XAudio2�̔j��
  if (p_xaudio2_ != nullptr) {
    p_xaudio2_->Release();
    p_xaudio2_ = nullptr;
  }

  // COM�̏I��
  CoUninitialize();
}

bool AudioManager::Init() {
  play_count_ = 0;

  // COM�̏�����
  if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED))) {
    return false;
  }

  //�T�E���h�f�o�C�X�쐬
  if (FAILED(XAudio2Create(&p_xaudio2_, 0))) {
    MY_LOG(L"�T�E���h�f�o�C�X�̍쐬�Ɏ��s���܂���\n");
    return false;
  }

  // MasteringVoice�쐬
  if (FAILED(p_xaudio2_->CreateMasteringVoice(&p_xaudio2_mastering_voice_))) {
    MY_LOG(L"�}�X�^�[�{�C�X�̍쐬�Ɏ��s���܂���\n");
    return false;
  }

  return true;
}

bool AudioManager::LoadWav(std::wstring filename, AudioType audio_type,
                           AudioSplitType audio_split_type) {
  //���ɓǂݍ��ݍς݂��`�F�b�N
  if (base_audiosources_.find(filename) != base_audiosources_.end()) {
    MY_LOG(L"���ɓǂݍ��ݍς݂ł��B");
    return false;
  }

  base_audiosources_[filename] = std::make_unique<AudioSource>();

  std::wstring filepath = path_;

  if (!FindDirectory(&filepath, filename)) {
    MY_LOG(L"�t�@�C�����������܂���ł����B");
    return false;
  }

  // wav�̓ǂݍ���
  if (!base_audiosources_[filename]->LoadWav(p_xaudio2_, filepath, filename,
                                             audio_type, audio_split_type)) {
    MY_LOG(L"wav�̓ǂݍ��݂Ɏ��s���܂����B\n");
    base_audiosources_.erase(filename);
    return false;
  }

  return true;
}

i32 AudioManager::Start(std::wstring filename, float volume, bool loop) {
  //�w�肵���t�@�C�����ǂݍ��܂�Ă��邩�`�F�b�N
  if (base_audiosources_.find(filename) == base_audiosources_.end()) {
    MY_LOG(L"�ǂݍ��܂�Ă��Ȃ��t�@�C�����Đ����悤�Ƃ��܂����B\n");
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

  //�J�E���g���X�V
  play_count_++;

  return play_count_ - 1;
}

void AudioManager::Play(i32 key) {
  //�G���[�`�F�b�N
  if (audiosources_.count(key) == 0) {
    MY_LOG(L"���݂��Ȃ��L�[���w�肳��܂����B\n");
    return;
  }

  audiosources_[key]->Play();
}

void AudioManager::Pause(i32 key) {
  //�G���[�`�F�b�N
  if (audiosources_.count(key) == 0) {
    MY_LOG(L"���݂��Ȃ��L�[���w�肳��܂����B\n");
    return;
  }
  audiosources_[key]->Pause();
}

void AudioManager::Stop(i32 key) {
  //�G���[�`�F�b�N
  if (audiosources_.count(key) == 0) {
    MY_LOG(L"���݂��Ȃ��L�[���w�肳��܂����B\n");
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
  //�G���[�`�F�b�N
  if (audiosources_.count(key) == 0) {
    MY_LOG(L"���݂��Ȃ��L�[���w�肳��܂����B\n");
    return;
  }

  float type_volume = audiosources_[key]->GetAudioType() == AudioType::BGM
                          ? bgm_volume_
                          : se_volume_;

  audiosources_[key]->SetVolume(volume, master_volume_ * type_volume);
}

void AudioManager::SetLoopFlag(i32 key, bool loop) {
  //�G���[�`�F�b�N
  if (audiosources_.count(key) == 0) {
    MY_LOG(L"���݂��Ȃ��L�[���w�肳��܂����B\n");
    return;
  }
  audiosources_[key]->SetLoopFlag(loop);
}

void AudioManager::SetPitch(i32 key, float pitch) {
  //�G���[�`�F�b�N
  if (audiosources_.count(key) == 0) {
    MY_LOG(L"���݂��Ȃ��L�[���w�肳��܂����B\n");
    return;
  }
  audiosources_[key]->SetPitch(pitch);
}

void AudioManager::Update() {
  //��Ȃ牽�����Ȃ�
  if (audiosources_.empty()) return;

  for (auto&& audio : audiosources_) {
    audio.second->Update();
  }

  for (auto itr = audiosources_.begin(); itr != audiosources_.end();) {
    // itr->second->Update();
    //�Đ����I����Ă�����폜
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
    MY_LOG(L"�w�肳�ꂽ�t�@�C��������܂���B");
  }

  do {
    TCHAR* wp_file_name = find_data.cFileName;

    //�t�H���_�̔��f
    if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      if (L'.' == wp_file_name[0]) {
        if ((L'\0' == wp_file_name[1]) ||
            (L'.' == wp_file_name[1] && L'\0' == wp_file_name[2])) {
          continue;
        }
      }

      //�t���p�X�̐���
      std::wstring fullpath = *filepath + L"\\" + wp_file_name;

      //�ċN���ăT�u�t�H���_������
      if (FindDirectory(&fullpath, filename)) {
        *filepath = fullpath;
        return true;
      }
    } else if (wp_file_name == filename) {
      //�t���p�X�̐���
      *filepath = *filepath + L"\\" + wp_file_name;
      return true;
    }
  } while (::FindNextFile(h_file, &find_data));
  //�����n���h�������
  ::FindClose(h_file);
  return false;
}

}  // namespace audio
}  // namespace legend