#include "audio_manager.h"

namespace legend {
namespace audio {

legend::audio::AudioManager::AudioManager() {}

legend::audio::AudioManager::~AudioManager() {
  //�z��ɓ����Ă���AudioSource���폜
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
    return false;
  }

  // MasteringVoice�쐬
  if (FAILED(p_xaudio2_->CreateMasteringVoice(&p_xaudio2_mastering_voice_))) {
    return false;
  }

  return true;
}

// bool AudioManager::LoadWav(std::wstring filename) {
//  //���ɓǂݍ��ݍς݂��`�F�b�N
//  if (base_audiosources_.find(filename) != base_audiosources_.end()) {
//    MY_LOG(L"���ɓǂݍ��ݍς݂ł��B");
//    return false;
//  }
//
//  base_audiosources_[filename] = std::make_unique<AudioSource>();
//
//  // wav�̓ǂݍ���
//  if (!base_audiosources_[filename]->Init(p_xaudio2_, filename)) {
//    MY_LOG(L"wav�̓ǂݍ��݂Ɏ��s���܂����B\n");
//    base_audiosources_.erase(filename);
//    return false;
//  }
//
//  return true;
//}

i32 AudioManager::Play(std::wstring filename, float volume, i32 loop_count) {
  ////�w�肵���t�@�C�����ǂݍ��܂�Ă��邩�`�F�b�N
  // if (base_audiosources_.find(filename) == base_audiosources_.end()) {
  //  MY_LOG(L"�ǂݍ��܂�Ă��Ȃ��t�@�C�����Đ����悤�Ƃ��܂����B\n");
  //  return false;
  //}

  audiosources_[play_count_] = std::make_unique<AudioSource>();
  // audiosources_[play_count_]->Copy(*base_audiosources_[filename]);
  audiosources_[play_count_]->LoadWav(p_xaudio2_, filename);
  audiosources_[play_count_]->SetLoopCount(loop_count);
  audiosources_[play_count_]->SetVolume(volume);
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

void AudioManager::Update() {
  //��Ȃ牽�����Ȃ�
  if (audiosources_.empty()) return;

  for (auto itr = audiosources_.begin(); itr != audiosources_.end();) {
    itr->second->Update();
    //�Đ����I����Ă�����폜
    if (itr->second->IsEnd()) {
      itr = audiosources_.erase(itr);
    } else {
      ++itr;
    }
  }
}

}  // namespace audio
}  // namespace legend