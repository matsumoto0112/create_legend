#include "src/audio/audio_source.h"

//コンストラクタ
legend::audio::AudioSource::AudioSource()
    : directsound_(nullptr),
      primary_(NULL),
      secondary_(NULL),
      is_playing_(false),
      // is_loop_(false),
      // mute_(false),
      // volume_(1.0f),
      file_path_(NULL) {}

//デストラクタ
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

  // COMの終了
  CoUninitialize();
}

bool legend::audio::AudioSource::Init(HWND* window, const wchar_t* file) {
  // COMの初期化
  CoInitialize(NULL);

  //サウンドデバイス作成
  if (DirectSoundCreate8(NULL, &directsound_, NULL) != S_OK) {
    MY_LOG(L"サウンドデバイスの作成に失敗しました\n");
    return false;
  }

  //強調レベルを設定
  if (directsound_->SetCooperativeLevel(*window, DSSCL_NORMAL) != DS_OK) {
    MY_LOG(L"強調レベルの設定に失敗しました\n");
    return false;
  }

  //プライマリバッファの作成
  if (CreatePrimaryBuffer()) {
    MY_LOG(L"プライマリバッファの作成に失敗しました\n");
    return false;
  }

  //サウンドバッファの作成
  if (CreateSoundBuffer(file)) {
    MY_LOG(L"サウンドバッファの作成に失敗しました\n");
    return false;
  }

  return true;
}

bool legend::audio::AudioSource::Play() {
  if (file_path_ == NULL) {
    MY_LOG(L"ファイルを読み込む前に再生しようとしました\n");
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

//プライマリバッファの作成
bool legend::audio::AudioSource::CreatePrimaryBuffer() {
  WAVEFORMATEX tag_wave_format_;

  //プライマリサウンドバッファの作成
  DSBUFFERDESC directsound_desc_;
  ZeroMemory(&directsound_desc_, sizeof(DSBUFFERDESC));
  directsound_desc_.dwSize = sizeof(DSBUFFERDESC);
  directsound_desc_.dwFlags = DSBCAPS_PRIMARYBUFFER;
  directsound_desc_.dwBufferBytes = 0;
  directsound_desc_.lpwfxFormat = NULL;
  if (directsound_->CreateSoundBuffer(&directsound_desc_, &primary_, NULL) !=
      DS_OK) {
    MY_LOG(L"プライマリサウンドバッファの作成に失敗\n");
    return false;
  }

  //プライマリバッファのステータスを設定
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
    MY_LOG(L"プライマリバッファのステータスの設定に失敗\n");
    return false;
  }

  return true;
}

//サウンドバッファの作成
bool legend::audio::AudioSource::CreateSoundBuffer(const wchar_t* file) {
  HRESULT tag_result_;
  MMCKINFO tag_wave_file_;
  MMCKINFO tag_wave_fmt_;
  MMCKINFO tag_wave_data_;
  LPWAVEFORMATEX tag_wave_format_;

  // WAVファイルをロード
  HMMIO h_src;
  h_src = mmioOpenA(LPSTR(file), NULL, MMIO_ALLOCBUF | MMIO_READ | MMIO_COMPAT);
  if (!h_src) {
    MY_LOG(L"WAVファイルのロードエラー\n");
    return false;
  }

  // WAVEチャンクチェック
  ZeroMemory(&tag_wave_file_, sizeof(tag_wave_file_));
  tag_result_ =
      mmioDescend(h_src, &tag_wave_fmt_, &tag_wave_file_, MMIO_FINDCHUNK);
  if (tag_wave_fmt_.ckid != mmioFOURCC('W', 'A', 'V', 'E')) {
    MY_LOG(L"WAVE チャンクチェックエラー\n");
    mmioClose(h_src, 0);
    return false;
  }

  // fmt チャンクチェック
  ZeroMemory(&tag_wave_fmt_, sizeof(tag_wave_file_));
  tag_result_ =
      mmioDescend(h_src, &tag_wave_fmt_, &tag_wave_file_, MMIO_FINDCHUNK);
  if (tag_wave_fmt_.ckid != mmioFOURCC('f', 'm', 't', ' ')) {
    MY_LOG(L"fmt チャンクチェックエラー\n");
    mmioClose(h_src, 0);
    return false;
  }

  //ヘッダメモリの計算
  i64 header_size_ = tag_wave_fmt_.cksize;
  if (header_size_ < sizeof(WAVEFORMATEX)) header_size_ = sizeof(WAVEFORMATEX);

  //ヘッダメモリ確保
  tag_wave_format_ = (LPWAVEFORMATEX)malloc(header_size_);
  if (!tag_wave_format_) {
    MY_LOG(L"メモリ確保エラー\n");
    mmioClose(h_src, 0);
    return false;
  }
  ZeroMemory(tag_wave_format_, header_size_);

  // WAVE フォーマットのロード
  tag_result_ = mmioRead(h_src, (char*)tag_wave_format_, tag_wave_fmt_.cksize);
  if (FAILED(tag_result_)) {
    MY_LOG(L"WAVE フォーマットロードエラー\n");
    free(tag_wave_format_);
    mmioClose(h_src, 0);
    return false;
  }

  // fmtチャンクに戻る
  mmioAscend(h_src, &tag_wave_fmt_, 0);

  // dataチャンクを探す
  while (true) {
    tag_result_ = mmioDescend(h_src, &tag_wave_data_, &tag_wave_file_, 0);
    if (FAILED(tag_result_)) {
      MY_LOG(L"dataチャンクが見つかりませんでした\n");
      free(tag_wave_format_);
      mmioClose(h_src, 0);
      return false;
    }
    if (tag_wave_data_.ckid == mmioStringToFOURCCA("data", 0)) break;
    //次のチャンクへ
    tag_result_ = mmioAscend(h_src, &tag_wave_data_, 0);
  }

  //サウンドバッファの作成
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
    MY_LOG(L"サウンドバッファの作成エラー\n");
    free(tag_wave_format_);
    mmioClose(h_src, 0);
    return false;
  }

  //ロック開始
  LPVOID mem1_, mem2_;
  DWORD size1_, size2_;
  tag_result_ =
      (secondary_)
          ->Lock(0, tag_wave_data_.cksize, &mem1_, &size1_, &mem2_, &size2_, 0);
  if (FAILED(tag_result_)) {
    MY_LOG(L"ロック失敗\n");
    free(tag_wave_format_);
    mmioClose(h_src, 0);
    return false;
  }

  //データ書き込み
  mmioRead(h_src, (char*)mem1_, size1_);
  mmioRead(h_src, (char*)mem2_, size2_);

  //ロック解除
  (secondary_)->Unlock(mem1_, size1_, mem2_, size2_);

  //ヘッダ用メモリを解放
  free(tag_wave_format_);

  // WAVを閉じる
  mmioClose(h_src, 0);

  //読み込んだファイルパスを記録
  file_path_ = file;

  return true;
}
