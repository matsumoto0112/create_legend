#ifndef LEGEND_AUDIO_AUDIO_SOURCE_H_
#define LEGEND_AUDIO_AUDIO_SOURCE_H_

#include <xaudio2.h>

#include "src/audio/audio_type.h"
#include "src/audio/xaudio2_callback.h"

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
  /**
   * @brief デストラクタ
   */
  ~AudioSource();
  /**
   * @brief 初期化処理
   * @return 処理が正しく終了したらtrueを返す
   */
  bool LoadWav(IXAudio2* p_xaudio2, std::wstring filepath,
               std::wstring filename, AudioType audio_type);
  /**
   * @brief 再生
   * @return 再生に成功したらtrueを返す
   */
  bool Play();
  /**
   * @brief 一時停止
   */
  void Pause();
  /**
   * @brief 停止
   */
  void Stop();
  /**
   * @brief 更新処理
   */
  void Update();
  /**
   * @brief 再生中かどうか
   */
  bool IsEnd() const;
  /**
   * @brief 音量の設定
   */
  void SetVolume(float volume, float master_volume);
  /**
   * @brief 音量の取得
   */
  float GetVolume();
  /**
   * @brief ロープフラグの設定
   */
  void SetLoopFlag(bool loop);
  /**
   * @brief ピッチを設定
   */
  void SetPitch(float pitch);
  /**
   * @brief ピッチを取得
   */
  float GetPitch();
  /**
   * @brief コピー
   */
  bool Copy(IXAudio2* p_xaudio2, const AudioSource& other_audiosource);
  /**
   * @brief ファイルパスを取得
   */
  std::wstring GetFilePath();
  /**
   * @brief ファイルネームを取得
   */
  std::wstring GetFileName();

 public:
  //! ループ再生するかどうか
  bool is_loop_;
  //! ミュートかどうか
  // bool mute_;

 private:
  //! 読み込んだ音源(XAudio2)
  IXAudio2SourceVoice* p_source_voice;
  //! サウンドバッファ(XAudio2)
  XAUDIO2_BUFFER xaudio2_buffer_;
  //! コールバッククラス(XAudio2)
  XAudio2CallBack callback_;
  //! オーディオデバイスの状態(Xaudio2)
  XAUDIO2_VOICE_STATE state_;

  //! MMIO
  HMMIO mmio_;
  //! 波形データフォーマット
  WAVEFORMATEX wav_format_;
  //! 波形データバッファー
  unsigned char* buffer_;
  unsigned char* ptr_;
  LONG read_len_;
  //! 波形データの長さ?
  i32 buffer_len_;
  //! 読み込みカウンタ―
  i32 buffer_count_;

  //! 再生中かどうか
  bool is_playing_;
  //! 一時停止中かどうか
  bool is_pause_;
  //! 音量
  float volume_;
  //! ピッチ
  float pitch_;

  //! 音の種類
  AudioType audio_type_;

  //! 読み込んだファイルパス
  std::wstring file_path_;
  //! 読み込んだファイルネーム
  std::wstring file_name_;
};

}  // namespace audio
}  // namespace legend

#endif  //! LEGEND_AUDIO_AUDIO_SOURCE_H_