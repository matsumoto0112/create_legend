#include "audio_manager.h"

namespace legend {
namespace audio {

legend::audio::AudioManager::AudioManager()
    : master_volume_(1.0f), file_path_(L"../legend/assets/audios/") {}

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

bool AudioManager::LoadWav(std::wstring filename) {
  //���ɓǂݍ��ݍς݂��`�F�b�N
  if (base_audiosources_.find(filename) != base_audiosources_.end()) {
    MY_LOG(L"���ɓǂݍ��ݍς݂ł��B");
    return false;
  }

  base_audiosources_[filename] = std::make_unique<AudioSource>();

  // wav�̓ǂݍ���
  if (!base_audiosources_[filename]->LoadWav(p_xaudio2_, filename)) {
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
  audiosources_[play_count_]->SetVolume(volume, master_volume_);
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
  audiosources_[key]->Stop();
}

void AudioManager::SetMasterVolume(float volume) { master_volume_ = volume; }

float AudioManager::GetMasterVolume() { return master_volume_; }

void AudioManager::SetVolume(i32 key, float volume) {
  //�G���[�`�F�b�N
  if (audiosources_.count(key) == 0) {
    MY_LOG(L"���݂��Ȃ��L�[���w�肳��܂����B\n");
    return;
  }
  audiosources_[key]->SetVolume(volume, master_volume_);
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