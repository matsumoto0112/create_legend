#ifndef LEGEND_AUDIO_AUDIO_SOURCE_H_
#define LEGEND_AUDIO_AUDIO_SOURCE_H_

#include <dsound.h>
#include <src/util/debug.h>

#pragma comment(lib, "dsound.lib")
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
  bool Init(HWND* window, const wchar_t* file);
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

 public:
  //! ループ再生するかどうか
  // bool is_loop_;
  //! ミュートかどうか
  // bool mute_;
  // !音量
  // float volume_;

 private:
  /**
   * @brief プライマリバッファの作成
   * @return 処理が正しく終了したらtrueを返す
   */
  bool CreatePrimaryBuffer();
  /**
   * @brief サウンドバッファの作成の作成
   * @return 処理が正しく終了したらtrueを返す
   */
  bool CreateSoundBuffer(const wchar_t* file);

 private:
  //! サウンドデバイス
  IDirectSound8* directsound_;
  //! プライマリサウンドバッファ
  LPDIRECTSOUNDBUFFER primary_;
  //! セカンダリサウンドバッファ
  LPDIRECTSOUNDBUFFER secondary_;
  //! 再生中かどうか
  bool is_playing_;
  //! 再生時間

  //! 読み込んだファイルパス
  const wchar_t* file_path_;
};

}  // namespace audio
}  // namespace legend

#endif  //! LEGEND_AUDIO_AUDIO_SOURCE_H_