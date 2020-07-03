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

bool AudioManager::Play(std::wstring filename) {
  ////�w�肵���t�@�C�����ǂݍ��܂�Ă��邩�`�F�b�N
  // if (base_audiosources_.find(filename) == base_audiosources_.end()) {
  //  MY_LOG(L"�ǂݍ��܂�Ă��Ȃ��t�@�C�����Đ����悤�Ƃ��܂����B\n");
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