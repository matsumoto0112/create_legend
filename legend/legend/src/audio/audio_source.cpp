#include "src/audio/audio_source.h"

namespace legend {
namespace audio {

//�R���X�g���N�^
AudioSource::AudioSource()
    : is_playing_(false),
      is_pause_(false) /*,
        // is_loop_(false),
        // mute_(false),
        // volume_(1.0f),
        file_path_(),
        p_source_voice(NULL)*/
{}

//�f�X�g���N�^
AudioSource::~AudioSource() {
  if (buffer_ != NULL) delete[] buffer_;
  if (p_source_voice != NULL) p_source_voice->DestroyVoice();
  if (mmio_ != NULL) mmioClose(mmio_, MMIO_FHOPEN);
}

bool AudioSource::LoadWav(IXAudio2* p_xaudio2, std::wstring filename) {
  buffer_ = NULL;
  buffer_count_ = 0;

  mmio_ = NULL;
  MMIOINFO info_ = {0};
  mmio_ = mmioOpen((LPWSTR)filename.c_str(), &info_, MMIO_READ);

  if (!mmio_) {
    MY_LOG(L"�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B\n");
    return false;
  }

  MMRESULT mmresult_;
  MMCKINFO riff_chunk_;
  riff_chunk_.fccType = mmioFOURCC('W', 'A', 'V', 'E');
  mmresult_ = mmioDescend(mmio_, &riff_chunk_, NULL, MMIO_FINDRIFF);
  if (mmresult_ != MMSYSERR_NOERROR) {
    MY_LOG(L"�g�`�f�[�^�̓ǂݍ��݂Ɏ��s���܂����B");
    return false;
  }

  // fmt�`�����N�̓ǂݍ���
  MMCKINFO chunk_;
  chunk_.ckid = mmioFOURCC('f', 'm', 't', ' ');
  mmresult_ = mmioDescend(mmio_, &chunk_, &riff_chunk_, MMIO_FINDCHUNK);
  if (mmresult_ != MMSYSERR_NOERROR) {
    MY_LOG(L"fmt�`�����N�̓ǂݍ��݂Ɏ��s���܂����B\n");
    return false;
  }

  // �g�`�t�H�[�}�b�g�̐ݒ�
  wav_format_ = {0};
  DWORD size = mmioRead(mmio_, (HPSTR)&wav_format_, chunk_.cksize);
  if (size != chunk_.cksize) {
    MY_LOG(L"MMIO�̓ǂݍ��݂Ɏ��s���܂����B\n");
    return false;
  }

  //�f�[�^�`�����N
  chunk_.ckid = mmioFOURCC('d', 'a', 't', 'a');
  mmresult_ = mmioDescend(mmio_, &chunk_, &riff_chunk_, MMIO_FINDCHUNK);
  if (mmresult_ != MMSYSERR_NOERROR) {
    MY_LOG(L"�f�[�^�`�����N�̓ǂݍ��݂Ɏ��s���܂����B\n");
    return false;
  }

  // �g�`�t�H�[�}�b�g������SourceVoice�̐���
  mmresult_ = p_xaudio2->CreateSourceVoice(
      &p_source_voice, &wav_format_, XAUDIO2_VOICE_NOPITCH,
      XAUDIO2_DEFAULT_FREQ_RATIO, &callback_);

  if (FAILED(mmresult_)) {
    MY_LOG(L"SourceVoice�̐����Ɏ��s���܂����B\n");
    return false;
  }

  buffer_len_ = wav_format_.nAvgBytesPerSec / 4;
  buffer_ = new unsigned char[buffer_len_ * 5];
  ptr_ = buffer_;
  buffer_count_ = (buffer_count_ + 1) % 5;
  read_len_ = mmioRead(mmio_, (HPSTR)ptr_, buffer_len_);
  if (read_len_ <= 0) {
    MY_LOG(L"�f�[�^������܂���B");
    return false;
  }

  //�����ǂݍ���
  memset(&xaudio2_buffer_, 0x00, sizeof(xaudio2_buffer_));
  xaudio2_buffer_.Flags =
      ((i32)read_len_ >= buffer_len_) ? 0 : XAUDIO2_END_OF_STREAM;
  xaudio2_buffer_.AudioBytes = read_len_;
  xaudio2_buffer_.pAudioData = ptr_;
  xaudio2_buffer_.PlayBegin = 0;
  xaudio2_buffer_.PlayLength = read_len_ / wav_format_.nBlockAlign;

  file_path_ = filename;

  return true;
}

//�Đ�����
bool AudioSource::Play() {
  if (file_path_.empty()) {
    MY_LOG(L"�t�@�C�����ǂݍ��܂�Ă��܂���B\n");
    return false;
  }

  p_source_voice->SubmitSourceBuffer(&xaudio2_buffer_);

  //�Đ����J�n
  p_source_voice->Start();

  //�e�t���O��ύX
  is_playing_ = true;
  is_pause_ = false;
  return true;
}

void legend::audio::AudioSource::Pause() {
  //��~������
  p_source_voice->Stop();

  //�e�t���O��ύX
  is_playing_ = false;
  is_pause_ = true;
}

// ��~����
void AudioSource::Stop() {
  //��~������
  p_source_voice->Stop();

  //�e�t���O��ύX
  is_playing_ = false;
  is_pause_ = false;
}

//�X�V����
void AudioSource::Update() {
  //�Đ����łȂ��Ȃ牽���s��Ȃ�
  if (!is_playing_) return;

  p_source_voice->GetState(&state_);
  if (state_.BuffersQueued == 0) {
    Stop();
    return;
  }
  while (state_.BuffersQueued < 4 && mmio_ != NULL) {
    ptr_ = buffer_ + buffer_len_ * buffer_count_;
    buffer_count_ = (buffer_count_ + 1) % 5;
    read_len_ = mmioRead(mmio_, (HPSTR)ptr_, buffer_len_);
    if (read_len_ <= 0) break;
    xaudio2_buffer_.Flags =
        ((i32)read_len_ >= buffer_len_) ? 0 : XAUDIO2_END_OF_STREAM;
    xaudio2_buffer_.AudioBytes = read_len_;
    xaudio2_buffer_.pAudioData = ptr_;
    xaudio2_buffer_.PlayBegin = 0;
    xaudio2_buffer_.PlayLength = read_len_ / wav_format_.nBlockAlign;
    p_source_voice->SubmitSourceBuffer(&xaudio2_buffer_, NULL);
    state_.BuffersQueued++;
    if (xaudio2_buffer_.Flags & XAUDIO2_END_OF_STREAM) {
      mmioClose(mmio_, 0);
      mmio_ = NULL;
    }
  }
}

bool AudioSource::IsEnd() const { return (!is_playing_ && !is_pause_); }

void AudioSource::SetVolume(float volume, float master_volume) {
  p_source_voice->SetVolume(volume * master_volume);
  volume_ = volume * master_volume;
}

void AudioSource::SetLoopFlag(bool loop) { is_loop_ = loop; }

// �R�s�[
bool AudioSource::Copy(IXAudio2* p_xaudio2, const AudioSource& other) {
  buffer_ = NULL;
  buffer_count_ = 0;
  is_playing_ = false;
  is_loop_ = other.is_loop_;
  wav_format_ = other.wav_format_;
  file_path_ = other.file_path_;

  mmio_ = NULL;
  MMIOINFO info_ = {0};
  mmio_ = mmioOpen((LPWSTR)file_path_.c_str(), &info_, MMIO_READ);

  MMRESULT mmresult_;
  // �g�`�t�H�[�}�b�g������SourceVoice�̐���
  mmresult_ = p_xaudio2->CreateSourceVoice(
      &p_source_voice, &wav_format_, XAUDIO2_VOICE_NOPITCH,
      XAUDIO2_DEFAULT_FREQ_RATIO, &callback_);

  if (FAILED(mmresult_)) {
    MY_LOG(L"SourceVoice�̐����Ɏ��s���܂����B\n");
    return false;
  }

  buffer_len_ = wav_format_.nAvgBytesPerSec / 4;
  buffer_ = new unsigned char[buffer_len_ * 5];
  ptr_ = buffer_;
  buffer_count_ = (buffer_count_ + 1) % 5;
  read_len_ = mmioRead(mmio_, (HPSTR)ptr_, buffer_len_);
  if (read_len_ <= 0) {
    MY_LOG(L"�f�[�^������܂���B");
    return false;
  }

  //�����ǂݍ���
  memset(&xaudio2_buffer_, 0x00, sizeof(xaudio2_buffer_));
  xaudio2_buffer_.Flags =
      ((i32)read_len_ >= buffer_len_) ? 0 : XAUDIO2_END_OF_STREAM;
  xaudio2_buffer_.AudioBytes = read_len_;
  xaudio2_buffer_.pAudioData = ptr_;
  xaudio2_buffer_.PlayBegin = 0;
  xaudio2_buffer_.PlayLength = read_len_ / wav_format_.nBlockAlign;

  return true;
}

}  // namespace audio
}  // namespace legend
