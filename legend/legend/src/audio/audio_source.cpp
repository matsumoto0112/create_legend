#include "src/audio/audio_source.h"

namespace legend {
namespace audio {

//コンストラクタ
AudioSource::AudioSource()
    : is_playing_(false) /*,
       // is_loop_(false),
       // mute_(false),
       // volume_(1.0f),
       file_path_(),
       p_source_voice(NULL)*/
{}

//デストラクタ
AudioSource::~AudioSource() {
  if (buffer_ != NULL) delete[] buffer_;
  if (p_source_voice != NULL) p_source_voice->DestroyVoice();
  if (mmio_ != NULL) mmioClose(mmio_, MMIO_FHOPEN);
}

bool AudioSource::Init(IXAudio2* p_xaudio2, std::wstring filename) {
  // CoInitializeEx(NULL, COINIT_MULTITHREADED);
  buffer_ = NULL;
  buffer_count_ = 0;

  mmio_ = NULL;
  MMIOINFO info_ = {0};
  mmio_ = mmioOpen((LPWSTR)filename.c_str(), &info_, MMIO_READ);

  if (!mmio_) {
    MY_LOG(L"ファイルの読み込みに失敗しました。\n");
    return false;
  }

  MMRESULT mmresult_;
  MMCKINFO riff_chunk_;
  riff_chunk_.fccType = mmioFOURCC('W', 'A', 'V', 'E');
  mmresult_ = mmioDescend(mmio_, &riff_chunk_, NULL, MMIO_FINDRIFF);
  if (mmresult_ != MMSYSERR_NOERROR) {
    MY_LOG(L"波形データの読み込みに失敗しました。");
    return false;
  }

  // fmtチャンクの読み込み
  MMCKINFO chunk_;
  chunk_.ckid = mmioFOURCC('f', 'm', 't', ' ');
  mmresult_ = mmioDescend(mmio_, &chunk_, &riff_chunk_, MMIO_FINDCHUNK);
  if (mmresult_ != MMSYSERR_NOERROR) {
    MY_LOG(L"fmtチャンクの読み込みに失敗しました。\n");
    return false;
  }

  // 波形フォーマットの設定
  wav_format_ = {0};
  DWORD size = mmioRead(mmio_, (HPSTR)&wav_format_, chunk_.cksize);
  if (size != chunk_.cksize) {
    MY_LOG(L"MMIOの読み込みに失敗しました。\n");
    return false;
  }

  //データチャンク
  chunk_.ckid = mmioFOURCC('d', 'a', 't', 'a');
  mmresult_ = mmioDescend(mmio_, &chunk_, &riff_chunk_, MMIO_FINDCHUNK);
  if (mmresult_ != MMSYSERR_NOERROR) {
    MY_LOG(L"データチャンクの読み込みに失敗しました。\n");
    return false;
  }

  // 波形フォーマットを元にSourceVoiceの生成
  mmresult_ = p_xaudio2->CreateSourceVoice(
      &p_source_voice, &wav_format_, XAUDIO2_VOICE_NOPITCH,
      XAUDIO2_DEFAULT_FREQ_RATIO, &callback_);

  if (FAILED(mmresult_)) {
    MY_LOG(L"SourceVoiceの生成に失敗しました。\n");
    return false;
  }

  buffer_len_ = wav_format_.nAvgBytesPerSec / 4;
  buffer_ = new unsigned char[buffer_len_ * 5];
  ptr_ = buffer_;
  buffer_count_ = (buffer_count_ + 1) % 5;
  read_len_ = mmioRead(mmio_, (HPSTR)ptr_, buffer_len_);
  if (read_len_ <= 0) {
    MY_LOG(L"データがありません。");
    return false;
  }

  //初期読み込み
  memset(&xaudio2_buffer_, 0x00, sizeof(xaudio2_buffer_));
  // xaudio2_buffer_.Flags =
  //    ((UINT32)read_len_ >= buffer_len_) ? 0 : XAUDIO2_END_OF_STREAM;
  xaudio2_buffer_.Flags = 0;
  xaudio2_buffer_.AudioBytes = read_len_;
  xaudio2_buffer_.pAudioData = ptr_;
  xaudio2_buffer_.PlayBegin = 0;
  xaudio2_buffer_.PlayLength = read_len_ / wav_format_.nBlockAlign;

  // if (FAILED(p_source_voice->SubmitSourceBuffer(&xaudio2_buffer_, NULL))) {
  //  return false;
  //}

  // if (FAILED(p_source_voice->Start(0, XAUDIO2_COMMIT_NOW))) {
  //  return false;
  //}

  file_path_ = filename;

  return true;
}

//再生処理
bool AudioSource::Play() {
  if (file_path_.empty()) {
    MY_LOG(L"ファイルを読み込む前に再生しようとしました\n");
    return false;
  }
  // 再生位置を設定
  p_source_voice->SubmitSourceBuffer(&xaudio2_buffer_);

  //再生を開始
  p_source_voice->Start();

  is_playing_ = true;
  return true;
}

// void legend::audio::AudioSource::Pause() {}

// 停止処理
void AudioSource::Stop() {
  p_source_voice->Stop();
  is_playing_ = false;
}

//更新処理
void AudioSource::Update() {
  //再生中でないなら何も行わない
  if (!is_playing_) return;

  // while (WaitForSingleObject(callback_.event, INFINITE) == WAIT_OBJECT_0) {
  p_source_voice->GetState(&state_);
  if (state_.BuffersQueued == 0) {
    is_playing_ = false;
    return;
  }
  while (state_.BuffersQueued < 4 && mmio_ != NULL) {
    ptr_ = buffer_ + buffer_len_ * buffer_count_;
    buffer_count_ = (buffer_count_ + 1) % 5;
    read_len_ = mmioRead(mmio_, (HPSTR)ptr_, buffer_len_);
    if (read_len_ <= 0) break;
    // xaudio2_buffer_.Flags =
    //    ((UINT32)read_len_ >= buffer_len_) ? 0 : XAUDIO2_END_OF_STREAM;
    xaudio2_buffer_.Flags = 0;
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
  //}
}

bool AudioSource::IsPlaying() { return is_playing_; }

// コピー
bool AudioSource::Copy(const AudioSource& other) {
  is_playing_ = false;
  mmio_ = other.mmio_;
  p_source_voice = other.p_source_voice;
  wav_format_ = other.wav_format_;
  xaudio2_buffer_ = other.xaudio2_buffer_;
  buffer_ = other.buffer_;
  ptr_ = other.ptr_;
  read_len_ = read_len_;
  buffer_len_ = other.buffer_len_;
  buffer_count_ = 0;
  file_path_ = other.file_path_;

  return true;
}

}  // namespace audio
}  // namespace legend
