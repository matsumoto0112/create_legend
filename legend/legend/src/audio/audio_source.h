#ifndef LEGEND_AUDIO_AUDIO_SOURCE_H_
#define LEGEND_AUDIO_AUDIO_SOURCE_H_

#include <dsound.h>

#pragma comment(lib, "dsound.lib")

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
  AudioSource(HWND* window);
  /**
   * @brief デストラクタ
   */
  ~AudioSource();
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

  public:
  //! ループ再生するかどうか
  bool is_loop_;
  //! ミュートかどうか
  bool mute_;
  // !音量
  float volume_;

 private:
  //! サウンドデバイス
  IDirectSound8 *direct_sound_;
  //! 再生中かどうか
  bool is_playing_;
};

}  // namespace audio
}  // namespace legend

#endif  //! LEGEND_AUDIO_AUDIO_SOURCE_H_
