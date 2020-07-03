#include "src/audio/audio_source.h"

namespace legend {
namespace audio {

//�R���X�g���N�^
AudioSource::AudioSource()
    : /*primary_(NULL),*/
      // secondary_(NULL),
      is_playing_(false) /*,
       // is_loop_(false),
       // mute_(false),
       // volume_(1.0f),
       file_path_(),
       p_source_voice(NULL),
       buffer_()*/
{}

// AudioSource::AudioSource(const AudioSource& other)
//    : is_playing_(other.is_playing_),
//      // is_loop_(other_audiosource.is_loop_),
//      // mute_(other_audiosource.mute_),
//      // volume_(other_audiosource.volume_),
//      file_path_(other.file_path_),
//      p_source_voice(other.p_source_voice),
//      buffer_(other.buffer_) {}

//�f�X�g���N�^
AudioSource::~AudioSource() {
  // if (secondary_) {
  //  secondary_->Release();
  //  secondary_ = NULL;
  //}

  // if (primary_) {
  //  primary_->Release();
  //  primary_ = NULL;
  //}

  // COM�̏I��
  //CoUninitialize();

    if (buffer_ != NULL)delete[] buffer_;
    if (p_source_voice != NULL) p_source_voice->DestroyVoice();
    if (mmio_ != NULL) mmioClose(mmio_, 0);
    //if (callback_ != NULL) 
}

// bool AudioSource::Init(IDirectSound8* directsound, const wchar_t* file) {
//  // COM�̏�����
//  CoInitialize(NULL);
//
//  //�v���C�}���o�b�t�@�̍쐬
//  if (CreatePrimaryBuffer(directsound)) {
//    MY_LOG(L"�v���C�}���o�b�t�@�̍쐬�Ɏ��s���܂���\n");
//    return false;
//  }
//
//  //�T�E���h�o�b�t�@�̍쐬
//  if (CreateSoundBuffer(directsound, file)) {
//    MY_LOG(L"�T�E���h�o�b�t�@�̍쐬�Ɏ��s���܂���\n");
//    return false;
//  }
//
//  return true;
//}

bool AudioSource::Init(IXAudio2* p_xaudio2, std::wstring filename) {
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
  wav_format_ = { 0 };
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

  xaudio2_buffer_ = { 0 };
  buffer_len_ = 2;
  buffer_ = new BYTE * [buffer_len_];
  const int len = wav_format_.nAvgBytesPerSec;
  for (int i = 0; i < buffer_len_; i++) {
      buffer_[i] = new BYTE[len];
  }
  buffer_count_ = 0;
  int i_size_ = mmioRead(mmio_, (HPSTR)buffer_[buffer_count_], len);
  xaudio2_buffer_.AudioBytes = i_size_;
  xaudio2_buffer_.pAudioData = buffer_[buffer_count_];

  // �g�`�t�H�[�}�b�g������SourceVoice�̐���
  mmresult_ = p_xaudio2->CreateSourceVoice(&p_source_voice, &wav_format_, 0, XAUDIO2_DEFAULT_FREQ_RATIO, &callback_);

  if (FAILED(mmresult_)){
      MY_LOG(L"SourceVoice�̐����Ɏ��s���܂����B\n");
    return false;
  }

  //// �Đ�����g�`�f�[�^�̐ݒ�
  //buffer_.pAudioData = (BYTE*)p_buffer_;
  //buffer_.Flags = XAUDIO2_END_OF_STREAM;
  //buffer_.AudioBytes = data_.size;

  p_source_voice->SubmitSourceBuffer(&xaudio2_buffer_);

  file_path_ = filename;

  return true;
}

//�Đ�����
bool AudioSource::Play() {
  if (file_path_.empty()) {
    MY_LOG(L"�t�@�C����ǂݍ��ޑO�ɍĐ����悤�Ƃ��܂���\n");
    return false;
  }
  // �Đ��ʒu��ݒ�
  // secondary_->SetCurrentPosition(0);
  xaudio2_buffer_.PlayBegin = (0);
  p_source_voice->SubmitSourceBuffer(&xaudio2_buffer_);

  //�Đ����J�n
  // secondary_->Play(0, 0, 0);
  p_source_voice->Start();

  is_playing_ = true;
  return true;
}

// void legend::audio::AudioSource::Pause() {}

// ��~����
void AudioSource::Stop() {
  // secondary_->Stop();
  p_source_voice->Stop();
  is_playing_ = false;
}

//�X�V����
void AudioSource::Update()
{
    //�Đ����łȂ��Ȃ牽���s��Ȃ�
    if (!is_playing_) return;

    do {
        int i_size_ = mmioRead(mmio_, (HPSTR)buffer_[buffer_len_], wav_format_.nAvgBytesPerSec);
        if (i_size_ <= 0) {
            is_playing_ = false;
            break;
        }
        xaudio2_buffer_.AudioBytes = i_size_;
        xaudio2_buffer_.pAudioData = buffer_[buffer_count_];
        if (FAILED(p_source_voice->SubmitSourceBuffer(&xaudio2_buffer_))) {
            MY_LOG(L"�Đ����Ƀo�b�t�@�̓ǂݍ��݂Ɏ��s���܂����B\n");
            return;
        }
        if (buffer_len_ <= ++buffer_count_) buffer_count_ = 0;
    } while (WaitForSingleObject(callback_.event, INFINITE) == WAIT_OBJECT_0);
}

bool AudioSource::Copy(const AudioSource& other) {
  is_playing_ = false;
  file_path_ = other.file_path_;
  buffer_count_ = 0;
  wav_format_ = other.wav_format_;
  buffer_len_ = other.buffer_len_;
  xaudio2_buffer_ = other.xaudio2_buffer_;
  buffer_ = other.buffer_;

  return true;
}

//�v���C�}���o�b�t�@�̍쐬
// bool AudioSource::CreatePrimaryBuffer(IDirectSound8* directsound) {
//  WAVEFORMATEX tag_wave_format_;
//
//  //�v���C�}���T�E���h�o�b�t�@�̍쐬
//  DSBUFFERDESC directsound_desc_;
//  ZeroMemory(&directsound_desc_, sizeof(DSBUFFERDESC));
//  directsound_desc_.dwSize = sizeof(DSBUFFERDESC);
//  directsound_desc_.dwFlags = DSBCAPS_PRIMARYBUFFER;
//  directsound_desc_.dwBufferBytes = 0;
//  directsound_desc_.lpwfxFormat = NULL;
//  if (directsound->CreateSoundBuffer(&directsound_desc_, &primary_, NULL) !=
//      DS_OK) {
//    MY_LOG(L"�v���C�}���T�E���h�o�b�t�@�̍쐬�Ɏ��s\n");
//    return false;
//  }
//
//  //�v���C�}���o�b�t�@�̃X�e�[�^�X��ݒ�
//  tag_wave_format_.cbSize = sizeof(WAVEFORMATEX);
//  tag_wave_format_.nChannels = 2;
//  tag_wave_format_.nSamplesPerSec = 44100;
//  tag_wave_format_.nSamplesPerSec = 44100;
//  tag_wave_format_.wBitsPerSample = 16;
//  tag_wave_format_.nBlockAlign =
//      tag_wave_format_.nChannels * tag_wave_format_.wBitsPerSample / 8;
//  tag_wave_format_.nAvgBytesPerSec =
//      tag_wave_format_.nSamplesPerSec * tag_wave_format_.nBlockAlign;
//  if (primary_->SetFormat(&tag_wave_format_) != S_OK) {
//    MY_LOG(L"�v���C�}���o�b�t�@�̃X�e�[�^�X�̐ݒ�Ɏ��s\n");
//    return false;
//  }
//
//  return true;
//}

//�T�E���h�o�b�t�@�̍쐬
// bool AudioSource::CreateSoundBuffer(IDirectSound8* directsound,
//                                    const wchar_t* file) {
//  HRESULT tag_result_;
//  MMCKINFO tag_wave_file_;
//  MMCKINFO tag_wave_fmt_;
//  MMCKINFO tag_wave_data_;
//  LPWAVEFORMATEX tag_wave_format_;
//
//  // WAV�t�@�C�������[�h
//  HMMIO h_src;
//  h_src = mmioOpenA(LPSTR(file), NULL, MMIO_ALLOCBUF | MMIO_READ |
//  MMIO_COMPAT); if (!h_src) {
//    MY_LOG(L"WAV�t�@�C���̃��[�h�G���[\n");
//    return false;
//  }
//
//  // WAVE�`�����N�`�F�b�N
//  ZeroMemory(&tag_wave_file_, sizeof(tag_wave_file_));
//  tag_result_ =
//      mmioDescend(h_src, &tag_wave_fmt_, &tag_wave_file_, MMIO_FINDCHUNK);
//  if (tag_wave_fmt_.ckid != mmioFOURCC('W', 'A', 'V', 'E')) {
//    MY_LOG(L"WAVE �`�����N�`�F�b�N�G���[\n");
//    mmioClose(h_src, 0);
//    return false;
//  }
//
//  // fmt �`�����N�`�F�b�N
//  ZeroMemory(&tag_wave_fmt_, sizeof(tag_wave_file_));
//  tag_result_ =
//      mmioDescend(h_src, &tag_wave_fmt_, &tag_wave_file_, MMIO_FINDCHUNK);
//  if (tag_wave_fmt_.ckid != mmioFOURCC('f', 'm', 't', ' ')) {
//    MY_LOG(L"fmt �`�����N�`�F�b�N�G���[\n");
//    mmioClose(h_src, 0);
//    return false;
//  }
//
//  //�w�b�_�������̌v�Z
//  i64 header_size_ = tag_wave_fmt_.cksize;
//  if (header_size_ < sizeof(WAVEFORMATEX)) header_size_ =
//  sizeof(WAVEFORMATEX);
//
//  //�w�b�_�������m��
//  tag_wave_format_ = (LPWAVEFORMATEX)malloc(header_size_);
//  if (!tag_wave_format_) {
//    MY_LOG(L"�������m�ۃG���[\n");
//    mmioClose(h_src, 0);
//    return false;
//  }
//  ZeroMemory(tag_wave_format_, header_size_);
//
//  // WAVE �t�H�[�}�b�g�̃��[�h
//  tag_result_ = mmioRead(h_src, (char*)tag_wave_format_,
//  tag_wave_fmt_.cksize); if (FAILED(tag_result_)) {
//    MY_LOG(L"WAVE �t�H�[�}�b�g���[�h�G���[\n");
//    free(tag_wave_format_);
//    mmioClose(h_src, 0);
//    return false;
//  }
//
//  // fmt�`�����N�ɖ߂�
//  mmioAscend(h_src, &tag_wave_fmt_, 0);
//
//  // data�`�����N��T��
//  while (true) {
//    tag_result_ = mmioDescend(h_src, &tag_wave_data_, &tag_wave_file_, 0);
//    if (FAILED(tag_result_)) {
//      MY_LOG(L"data�`�����N��������܂���ł���\n");
//      free(tag_wave_format_);
//      mmioClose(h_src, 0);
//      return false;
//    }
//    if (tag_wave_data_.ckid == mmioStringToFOURCCA("data", 0)) break;
//    //���̃`�����N��
//    tag_result_ = mmioAscend(h_src, &tag_wave_data_, 0);
//  }
//
//  //�T�E���h�o�b�t�@�̍쐬
//  DSBUFFERDESC directsound_desc_;
//  ZeroMemory(&directsound_desc_, sizeof(DSBUFFERDESC));
//  directsound_desc_.dwSize = sizeof(DSBUFFERDESC);
//  directsound_desc_.dwFlags =
//      DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STATIC | DSBCAPS_LOCDEFER;
//  directsound_desc_.dwBufferBytes = tag_wave_data_.cksize;
//  directsound_desc_.lpwfxFormat = tag_wave_format_;
//  directsound_desc_.guid3DAlgorithm = DS3DALG_DEFAULT;
//  tag_result_ =
//      directsound->CreateSoundBuffer(&directsound_desc_, &secondary_, NULL);
//  if (FAILED(tag_result_)) {
//    MY_LOG(L"�T�E���h�o�b�t�@�̍쐬�G���[\n");
//    free(tag_wave_format_);
//    mmioClose(h_src, 0);
//    return false;
//  }
//
//  //���b�N�J�n
//  LPVOID mem1_, mem2_;
//  DWORD size1_, size2_;
//  tag_result_ =
//      (secondary_)
//          ->Lock(0, tag_wave_data_.cksize, &mem1_, &size1_, &mem2_, &size2_,
//          0);
//  if (FAILED(tag_result_)) {
//    MY_LOG(L"���b�N���s\n");
//    free(tag_wave_format_);
//    mmioClose(h_src, 0);
//    return false;
//  }
//
//  //�f�[�^��������
//  mmioRead(h_src, (char*)mem1_, size1_);
//  mmioRead(h_src, (char*)mem2_, size2_);
//
//  //���b�N����
//  (secondary_)->Unlock(mem1_, size1_, mem2_, size2_);
//
//  //�w�b�_�p�����������
//  free(tag_wave_format_);
//
//  // WAV�����
//  mmioClose(h_src, 0);
//
//  //�ǂݍ��񂾃t�@�C���p�X���L�^
//  file_path_ = file;
//
//  return true;
//}

}  // namespace audio
}  // namespace legend
