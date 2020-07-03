#ifndef LEGEND_AUDIO_AUDIO_SOURCE_H_
#define LEGEND_AUDIO_AUDIO_SOURCE_H_

//#include <iostream>
//#include <dsound.h>
#include <xaudio2.h>
#include "src/audio/xaudio2_callback.h"

//#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "xaudio2.lib")
#pragma comment(lib, "winmm.lib")

namespace legend {
namespace audio {
/**
 * @file audio_source.h
 * @brief オーディオ再生クラス
 */
class AudioSource {
 public:
  /**
   * @brief コンストラクタ
   */
  AudioSource();
  ///**
  // * @brief コンストラクタ
  // */
  //AudioSource(const AudioSource &other);
  /**
   * @brief デストラクタ
   */
  ~AudioSource();
  /**
   * @brief 初期化処理
   * @return 処理が正しく終了したらtrueを返す
   */
  // bool Init(IDirectSound8* directsound, const wchar_t* file);
  bool Init(IXAudio2* p_xaudio2, std::wstring filename);
  /**
   * @brief 再生
   * @return 再生に成功したらtrueを返す
   */
  bool Play();
  /**
   * @brief 一時停止
   */
  // void Pause();
  /**
   * @brief 停止
   */
  void Stop();
  /**
   * @brief 更新処理
   */
  void Update();
  /**
   * @brief コピー
   */
  bool Copy(const AudioSource& other_audiosource);

 public:
  //! ループ再生するかどうか
  // bool is_loop_;
  //! ミュートかどうか
  // bool mute_;
  // !音量
  //float volume_;

 private:
  /**
   * @brief プライマリバッファの作成
   * @return 処理が正しく終了したらtrueを返す
   */
  // bool CreatePrimaryBuffer(IDirectSound8* directsound);
  /**
   * @brief サウンドバッファの作成の作成
   * @return 処理が正しく終了したらtrueを返す
   */
  // bool CreateSoundBuffer(IDirectSound8* directsound, const wchar_t* file);

 private:
  //! プライマリサウンドバッファ(DirectSound)
  // LPDIRECTSOUNDBUFFER primary_;
  //! セカンダリサウンドバッファ(DirectSound)
  // LPDIRECTSOUNDBUFFER secondary_;

  //! 読み込んだ音源(XAudio2)
  IXAudio2SourceVoice* p_source_voice;
  //! サウンドバッファ(XAudio2)
  XAUDIO2_BUFFER xaudio2_buffer_;
  //! コールバッククラス(XAudio2)
  XAudio2CallBack callback_;

  //! MMIO
  HMMIO mmio_;
  //! 波形データフォーマット
  WAVEFORMATEX wav_format_;
  //! 波形データバッファー
  BYTE** buffer_;
  //! 波形サイズの長さ?
  i64 buffer_len_;
  //! 読み込みカウント
  i64 buffer_count_;

  //! 再生中かどうか
  bool is_playing_;
  //! 再生時間

  //! 読み込んだファイルパス
  std::wstring file_path_;
};

}  // namespace audio
}  // namespace legend

#endif  //! LEGEND_AUDIO_AUDIO_SOURCE_H_