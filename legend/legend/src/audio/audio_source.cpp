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

bool AudioSource::LoadWav(IXAudio2* p_xaudio2, std::wstring filepath,
                          std::wstring filename, AudioType audio_type,
                          AudioSplitType audio_split_type) {
  audio_type_ = audio_type;
  audio_split_type_ = audio_split_type;

  switch (audio_split_type) {
    case AudioSplitType::SPLIT:
      if (!LoadNoSplit(p_xaudio2, filepath)) {
        MY_LOG(L"BGM�̓ǂݍ��݂Ɏ��s���܂����B\n");
        return false;
      }
      break;
    case AudioSplitType::NO_SPLIT:
      if (!LoadSplit(p_xaudio2, filepath)) {
        MY_LOG(L"SE�̓ǂݍ��݂Ɏ��s���܂����B\n");
        return false;
      }
      break;
    default:
      MY_LOG(L"AudioSplitType���w�肳��Ă��܂���B\n");
      return false;
      break;
  }

  file_name_ = filename;

  return true;
}

//�Đ�����
bool AudioSource::Play() {
  if (file_path_.empty()) {
    MY_LOG(L"�t�@�C�����ǂݍ��܂�Ă��܂���B\n");
    return false;
  }

  if (is_playing_) return false;

  //�Đ����J�n
  p_source_voice->Start();

  //�e�t���O��ύX
  is_playing_ = true;
  is_pause_ = false;
  return true;
}

//�ꎞ��~
void legend::audio::AudioSource::Pause() {
  //��~������
  p_source_voice->Stop();

  //�e�t���O��ύX
  is_playing_ = false;
  is_pause_ = true;
}

//��~����
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

  while (state_.BuffersQueued < 4 && mmio_ != NULL &&
         audio_split_type_ != AudioSplitType::SPLIT) {
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

  // �Đ����I�����Ă��邩�̔���
  if (state_.BuffersQueued == 0) {
    Stop();
    return;
  }
}

//�I������
bool AudioSource::IsEnd() const { return (!is_playing_ && !is_pause_); }

//���ʐݒ�
void AudioSource::SetVolume(float volume, float master_volume) {
  //���ʂ�ݒ�
  p_source_voice->SetVolume(volume * master_volume);
  //���ʂ��L�^
  volume_ = volume;
}

//���ʎ擾
float AudioSource::GetVolume() { return volume_; }

//���[�v�ݒ�
void AudioSource::SetLoopFlag(bool loop) { is_loop_ = loop; }

//�s�b�`�ݒ�
void AudioSource::SetPitch(float pitch) {
  if (pitch < XAUDIO2_MIN_FREQ_RATIO) {
    pitch = XAUDIO2_MIN_FREQ_RATIO;
  } else if (pitch > XAUDIO2_MAX_FREQ_RATIO) {
    pitch = XAUDIO2_MAX_FREQ_RATIO;
  }

  //�s�b�`��ݒ�
  p_source_voice->SetFrequencyRatio(pitch);
  //�s�b�`���L�^
  pitch_ = pitch;
}

//�s�b�`���擾
float AudioSource::GetPitch() { return pitch_; }

// �R�s�[
bool AudioSource::Copy(IXAudio2* p_xaudio2, const AudioSource& other) {
  switch (other.audio_split_type_) {
    case AudioSplitType::NO_SPLIT:
      if (!CopyNoSplit(p_xaudio2, other)) {
        MY_LOG(L"��������Ă��Ȃ������f�[�^�̃R�s�[�Ɏ��s���܂����B\n");
        return false;
      }
      break;
    case AudioSplitType::SPLIT:
      if (!CopySplit(p_xaudio2, other)) {
        MY_LOG(L"�������ꂽ�����f�[�^�̃R�s�[�Ɏ��s���܂����B\n");
        return false;
      }
      break;
    default:
      MY_LOG(L"AudioType���w�肳��Ă��܂���B\n");
      return false;
      break;
  }
  return true;
}

std::wstring AudioSource::GetFilePath() { return file_path_; }

std::wstring AudioSource::GetFileName() { return file_name_; }

AudioType AudioSource::GetAudioType()
{
    return audio_type_;
}

bool AudioSource::LoadSplit(IXAudio2* p_xaudio2, std::wstring filepath) {
  buffer_ = NULL;
  buffer_count_ = 0;

  mmio_ = NULL;
  MMIOINFO info_ = {0};
  mmio_ = mmioOpen((LPWSTR)filepath.c_str(), &info_, MMIO_READ);

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

  buffer_len_ = static_cast<i32>(std::filesystem::file_size(filepath) /
                                 (wav_format_.nAvgBytesPerSec / 4) *
                                 (wav_format_.nAvgBytesPerSec / 4));
  buffer_ = new unsigned char[buffer_len_ * 5];
  ptr_ = buffer_;
  buffer_count_ = (buffer_count_ + 1) % 5;
  read_len_ = mmioRead(mmio_, (HPSTR)ptr_, buffer_len_);
  if (read_len_ <= 0) {
    MY_LOG(L"�o�b�t�@�[�̓ǂݍ��݂Ɏ��s���܂����B\n");
    return false;
  }

  //�����ǂݍ���
  memset(&xaudio2_buffer_, 0x00, sizeof(xaudio2_buffer_));
  xaudio2_buffer_.Flags = XAUDIO2_END_OF_STREAM;
  xaudio2_buffer_.AudioBytes = buffer_len_;
  xaudio2_buffer_.pAudioData = buffer_;
  xaudio2_buffer_.PlayBegin = 0;

  file_path_ = filepath;

  return true;
}

bool AudioSource::LoadNoSplit(IXAudio2* p_xaudio2, std::wstring filepath) {
  buffer_ = NULL;
  buffer_count_ = 0;

  mmio_ = NULL;
  MMIOINFO info_ = {0};
  mmio_ = mmioOpen((LPWSTR)filepath.c_str(), &info_, MMIO_READ);

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
    MY_LOG(L"�o�b�t�@�[�̓ǂݍ��݂Ɏ��s���܂����B\n");
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

  file_path_ = filepath;

  return true;
}

bool AudioSource::CopyNoSplit(IXAudio2* p_xaudio2, const AudioSource& other) {
  buffer_ = NULL;
  buffer_count_ = 0;
  is_playing_ = false;

  audio_type_ = other.audio_type_;

  xaudio2_buffer_;

  is_loop_ = other.is_loop_;
  file_path_ = other.file_path_;
  file_name_ = other.file_name_;
  wav_format_ = other.wav_format_;
  buffer_len_ = other.buffer_len_;

  mmio_ = NULL;
  MMIOINFO info_ = {0};
  mmio_ = mmioOpen((LPWSTR)file_path_.c_str(), &info_, MMIO_READ);

  MMRESULT mmresult_;
  // �g�`�t�H�[�}�b�g������SourceVoice�̐���
  mmresult_ = p_xaudio2->CreateSourceVoice(
      &p_source_voice, &wav_format_, XAUDIO2_VOICE_USEFILTER,
      XAUDIO2_DEFAULT_FREQ_RATIO, &callback_);

  if (FAILED(mmresult_)) {
    MY_LOG(L"SourceVoice�̐����Ɏ��s���܂����B\n");
    return false;
  }

  buffer_ = new unsigned char[buffer_len_ * 5];
  ptr_ = buffer_;
  buffer_count_ = (buffer_count_ + 1) % 5;
  read_len_ = mmioRead(mmio_, (HPSTR)ptr_, buffer_len_);
  if (read_len_ <= 0) {
    MY_LOG(L"�o�b�t�@�[�̓ǂݍ��݂Ɏ��s���܂����B\n");
    return false;
  }

  //�����ǂݍ���
  memset(&xaudio2_buffer_, 0x00, sizeof(xaudio2_buffer_));
  xaudio2_buffer_.Flags =
      ((i32)read_len_ >= buffer_len_) ? 0 : XAUDIO2_END_OF_STREAM;
  xaudio2_buffer_.AudioBytes = read_len_;
  xaudio2_buffer_.pAudioData = ptr_;
  xaudio2_buffer_.PlayBegin = 16;

  p_source_voice->FlushSourceBuffers();
  p_source_voice->SubmitSourceBuffer(&xaudio2_buffer_);

  return true;
}

bool AudioSource::CopySplit(IXAudio2* p_xaudio2, const AudioSource& other) {
  buffer_ = NULL;
  buffer_count_ = 0;
  is_playing_ = false;

  audio_type_ = other.audio_type_;

  xaudio2_buffer_;

  is_loop_ = other.is_loop_;
  file_path_ = other.file_path_;
  file_name_ = other.file_name_;
  wav_format_ = other.wav_format_;
  buffer_len_ = other.buffer_len_;

  mmio_ = NULL;
  MMIOINFO info_ = {0};
  mmio_ = mmioOpen((LPWSTR)file_path_.c_str(), &info_, MMIO_READ);

  MMRESULT mmresult_;
  // �g�`�t�H�[�}�b�g������SourceVoice�̐���
  mmresult_ = p_xaudio2->CreateSourceVoice(
      &p_source_voice, &wav_format_, XAUDIO2_VOICE_USEFILTER,
      XAUDIO2_DEFAULT_FREQ_RATIO, &callback_);

  if (FAILED(mmresult_)) {
    MY_LOG(L"SourceVoice�̐����Ɏ��s���܂����B\n");
    return false;
  }

  buffer_ = new unsigned char[buffer_len_ * 5];
  ptr_ = buffer_;
  buffer_count_ = (buffer_count_ + 1) % 5;
  read_len_ = mmioRead(mmio_, (HPSTR)ptr_, buffer_len_);
  if (read_len_ <= 0) {
    MY_LOG(L"�o�b�t�@�[�̓ǂݍ��݂Ɏ��s���܂����B\n");
    return false;
  }

  //�����ǂݍ���
  memset(&xaudio2_buffer_, 0x00, sizeof(xaudio2_buffer_));
  xaudio2_buffer_.Flags = XAUDIO2_END_OF_STREAM;
  xaudio2_buffer_.AudioBytes = read_len_;
  xaudio2_buffer_.pAudioData = ptr_;
  xaudio2_buffer_.PlayBegin = 16;

  p_source_voice->FlushSourceBuffers();
  p_source_voice->SubmitSourceBuffer(&xaudio2_buffer_);

  return true;
}

}  // namespace audio
}  // namespace legend
