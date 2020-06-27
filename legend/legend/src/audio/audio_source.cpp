#include "src/audio/audio_source.h"

//�R���X�g���N�^
legend::audio::AudioSource::AudioSource()
    : directsound_(nullptr),
      primary_(NULL),
      secondary_(NULL),
      is_playing_(false),
      // is_loop_(false),
      // mute_(false),
      // volume_(1.0f),
      file_path_(NULL) {}

//�f�X�g���N�^
legend::audio::AudioSource::~AudioSource() {
  if (secondary_) {
    secondary_->Release();
    secondary_ = NULL;
  }

  if (primary_) {
    primary_->Release();
    primary_ = NULL;
  }

  if (directsound_) {
    directsound_->Release();
    directsound_ = NULL;
  }

  // COM�̏I��
  CoUninitialize();
}

bool legend::audio::AudioSource::Init(HWND* window, const wchar_t* file) {
  // COM�̏�����
  CoInitialize(NULL);

  //�T�E���h�f�o�C�X�쐬
  if (DirectSoundCreate8(NULL, &directsound_, NULL) != S_OK) {
    MY_LOG(L"�T�E���h�f�o�C�X�̍쐬�Ɏ��s���܂���\n");
    return false;
  }

  //�������x����ݒ�
  if (directsound_->SetCooperativeLevel(*window, DSSCL_NORMAL) != DS_OK) {
    MY_LOG(L"�������x���̐ݒ�Ɏ��s���܂���\n");
    return false;
  }

  //�v���C�}���o�b�t�@�̍쐬
  if (CreatePrimaryBuffer()) {
    MY_LOG(L"�v���C�}���o�b�t�@�̍쐬�Ɏ��s���܂���\n");
    return false;
  }

  //�T�E���h�o�b�t�@�̍쐬
  if (CreateSoundBuffer(file)) {
    MY_LOG(L"�T�E���h�o�b�t�@�̍쐬�Ɏ��s���܂���\n");
    return false;
  }

  return true;
}

bool legend::audio::AudioSource::Play() {
  if (file_path_ == NULL) {
    MY_LOG(L"�t�@�C����ǂݍ��ޑO�ɍĐ����悤�Ƃ��܂���\n");
    return false;
  }

  secondary_->SetCurrentPosition(0);

  secondary_->Play(0, 0, 0);
  is_playing_ = true;

  return true;
}

// void legend::audio::AudioSource::Pause() {}

void legend::audio::AudioSource::Stop() {
  secondary_->Stop();
  is_playing_ = true;
}

//�v���C�}���o�b�t�@�̍쐬
bool legend::audio::AudioSource::CreatePrimaryBuffer() {
  WAVEFORMATEX tag_wave_format_;

  //�v���C�}���T�E���h�o�b�t�@�̍쐬
  DSBUFFERDESC directsound_desc_;
  ZeroMemory(&directsound_desc_, sizeof(DSBUFFERDESC));
  directsound_desc_.dwSize = sizeof(DSBUFFERDESC);
  directsound_desc_.dwFlags = DSBCAPS_PRIMARYBUFFER;
  directsound_desc_.dwBufferBytes = 0;
  directsound_desc_.lpwfxFormat = NULL;
  if (directsound_->CreateSoundBuffer(&directsound_desc_, &primary_, NULL) !=
      DS_OK) {
    MY_LOG(L"�v���C�}���T�E���h�o�b�t�@�̍쐬�Ɏ��s\n");
    return false;
  }

  //�v���C�}���o�b�t�@�̃X�e�[�^�X��ݒ�
  tag_wave_format_.cbSize = sizeof(WAVEFORMATEX);
  tag_wave_format_.nChannels = 2;
  tag_wave_format_.nSamplesPerSec = 44100;
  tag_wave_format_.nSamplesPerSec = 44100;
  tag_wave_format_.wBitsPerSample = 16;
  tag_wave_format_.nBlockAlign =
      tag_wave_format_.nChannels * tag_wave_format_.wBitsPerSample / 8;
  tag_wave_format_.nAvgBytesPerSec =
      tag_wave_format_.nSamplesPerSec * tag_wave_format_.nBlockAlign;
  if (primary_->SetFormat(&tag_wave_format_) != S_OK) {
    MY_LOG(L"�v���C�}���o�b�t�@�̃X�e�[�^�X�̐ݒ�Ɏ��s\n");
    return false;
  }

  return true;
}

//�T�E���h�o�b�t�@�̍쐬
bool legend::audio::AudioSource::CreateSoundBuffer(const wchar_t* file) {
  HRESULT tag_result_;
  MMCKINFO tag_wave_file_;
  MMCKINFO tag_wave_fmt_;
  MMCKINFO tag_wave_data_;
  LPWAVEFORMATEX tag_wave_format_;

  // WAV�t�@�C�������[�h
  HMMIO h_src;
  h_src = mmioOpenA(LPSTR(file), NULL, MMIO_ALLOCBUF | MMIO_READ | MMIO_COMPAT);
  if (!h_src) {
    MY_LOG(L"WAV�t�@�C���̃��[�h�G���[\n");
    return false;
  }

  // WAVE�`�����N�`�F�b�N
  ZeroMemory(&tag_wave_file_, sizeof(tag_wave_file_));
  tag_result_ =
      mmioDescend(h_src, &tag_wave_fmt_, &tag_wave_file_, MMIO_FINDCHUNK);
  if (tag_wave_fmt_.ckid != mmioFOURCC('W', 'A', 'V', 'E')) {
    MY_LOG(L"WAVE �`�����N�`�F�b�N�G���[\n");
    mmioClose(h_src, 0);
    return false;
  }

  // fmt �`�����N�`�F�b�N
  ZeroMemory(&tag_wave_fmt_, sizeof(tag_wave_file_));
  tag_result_ =
      mmioDescend(h_src, &tag_wave_fmt_, &tag_wave_file_, MMIO_FINDCHUNK);
  if (tag_wave_fmt_.ckid != mmioFOURCC('f', 'm', 't', ' ')) {
    MY_LOG(L"fmt �`�����N�`�F�b�N�G���[\n");
    mmioClose(h_src, 0);
    return false;
  }

  //�w�b�_�������̌v�Z
  i64 header_size_ = tag_wave_fmt_.cksize;
  if (header_size_ < sizeof(WAVEFORMATEX)) header_size_ = sizeof(WAVEFORMATEX);

  //�w�b�_�������m��
  tag_wave_format_ = (LPWAVEFORMATEX)malloc(header_size_);
  if (!tag_wave_format_) {
    MY_LOG(L"�������m�ۃG���[\n");
    mmioClose(h_src, 0);
    return false;
  }
  ZeroMemory(tag_wave_format_, header_size_);

  // WAVE �t�H�[�}�b�g�̃��[�h
  tag_result_ = mmioRead(h_src, (char*)tag_wave_format_, tag_wave_fmt_.cksize);
  if (FAILED(tag_result_)) {
    MY_LOG(L"WAVE �t�H�[�}�b�g���[�h�G���[\n");
    free(tag_wave_format_);
    mmioClose(h_src, 0);
    return false;
  }

  // fmt�`�����N�ɖ߂�
  mmioAscend(h_src, &tag_wave_fmt_, 0);

  // data�`�����N��T��
  while (true) {
    tag_result_ = mmioDescend(h_src, &tag_wave_data_, &tag_wave_file_, 0);
    if (FAILED(tag_result_)) {
      MY_LOG(L"data�`�����N��������܂���ł���\n");
      free(tag_wave_format_);
      mmioClose(h_src, 0);
      return false;
    }
    if (tag_wave_data_.ckid == mmioStringToFOURCCA("data", 0)) break;
    //���̃`�����N��
    tag_result_ = mmioAscend(h_src, &tag_wave_data_, 0);
  }

  //�T�E���h�o�b�t�@�̍쐬
  DSBUFFERDESC directsound_desc_;
  ZeroMemory(&directsound_desc_, sizeof(DSBUFFERDESC));
  directsound_desc_.dwSize = sizeof(DSBUFFERDESC);
  directsound_desc_.dwFlags =
      DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STATIC | DSBCAPS_LOCDEFER;
  directsound_desc_.dwBufferBytes = tag_wave_data_.cksize;
  directsound_desc_.lpwfxFormat = tag_wave_format_;
  directsound_desc_.guid3DAlgorithm = DS3DALG_DEFAULT;
  tag_result_ =
      directsound_->CreateSoundBuffer(&directsound_desc_, &secondary_, NULL);
  if (FAILED(tag_result_)) {
    MY_LOG(L"�T�E���h�o�b�t�@�̍쐬�G���[\n");
    free(tag_wave_format_);
    mmioClose(h_src, 0);
    return false;
  }

  //���b�N�J�n
  LPVOID mem1_, mem2_;
  DWORD size1_, size2_;
  tag_result_ =
      (secondary_)
          ->Lock(0, tag_wave_data_.cksize, &mem1_, &size1_, &mem2_, &size2_, 0);
  if (FAILED(tag_result_)) {
    MY_LOG(L"���b�N���s\n");
    free(tag_wave_format_);
    mmioClose(h_src, 0);
    return false;
  }

  //�f�[�^��������
  mmioRead(h_src, (char*)mem1_, size1_);
  mmioRead(h_src, (char*)mem2_, size2_);

  //���b�N����
  (secondary_)->Unlock(mem1_, size1_, mem2_, size2_);

  //�w�b�_�p�����������
  free(tag_wave_format_);

  // WAV�����
  mmioClose(h_src, 0);

  //�ǂݍ��񂾃t�@�C���p�X���L�^
  file_path_ = file;

  return true;
}
