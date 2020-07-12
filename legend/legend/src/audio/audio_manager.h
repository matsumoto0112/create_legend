#ifndef LEGEND_AUDIO_AUDIO_MANAGER_H_
#define LEGEND_AUDIO_AUDIO_MANAGER_H_

/**
 * @file audio_manager.h
 * @brief 音響管理クラス
 */

#include <xaudio2.h>

#include <vector>

#include "src/audio/audio_source.h"
#include "src/util/path.h"

#pragma comment(lib, "dsound.lib")

namespace legend {
namespace audio {

class AudioManager {
 public:
  /**
   * @brief コンストラクタ
   */
  AudioManager();
  /**
   * @brief デストラクタ
   */
  ~AudioManager();
  /**
   * @brief 初期化処理
   * @return 処理が正しく終了したらtrueを返す
   */
  bool Init(/*HWND* window*/);
  /**
   * @brief wav読み込み
   * @return 処理が正しく終了したらtrueを返す
   */
  bool LoadWav(std::wstring filename, AudioType audio_type);
  /**
   * @brief 再生処理
   * @return 処理が正しく終了したらkeyを返す
   * @return 処理に失敗したら-1を返す
   */
  i32 Start(std::wstring filename, float volume, bool loop = false);
  /**
   * @brief 再生処理
   */
  void Play(i32 key);
  /**
   * @brief ポーズさせる処理
   */
  void Pause(i32 key);
  /**
   * @brief 停止させる処理
   */
  void Stop(i32 key);
  /**
   * @brief マスター音量の設定
   */
  void SetMasterVolume(float volume);
  /**
   * @brief マスター音量の取得
   */
  float GetMasterVolume();
  /**
   * @brief 音量の設定
   */
  void SetVolume(i32 key, float volume);
  /**
   * @brief 音量の設定
   */
  void SetLoopFlag(i32 key, bool loop);
  /**
   * @brief ピッチの指定
   */
  void SetPitch(i32 key, float pitch);
  /**
   * @brief 更新処理
   */
  void Update();

private :
    void LoopStart(i32 key, std::wstring filename, float volume, float pitch);

    bool FindDirectory(std::wstring *filepath, std::wstring filename);

 public:
  //! AudioSource配列
  std::unordered_map<i32, std::unique_ptr<AudioSource>> audiosources_;
  //! 読み込んだAudioSourceを保存
  std::unordered_map<std::wstring, std::unique_ptr<AudioSource>>
      base_audiosources_;

 private:
  //! サウンドデバイス
  IXAudio2* p_xaudio2_;
  //! 最終到達地点
  IXAudio2MasteringVoice* p_xaudio2_mastering_voice_;
  //! 再生したサウンド数
  i32 play_count_;

  //! ファイルパス
  std::filesystem::path path_;

  //! 全体の音量
  float master_volume_;
};

}  // namespace audio
}  // namespace legend

#endif  //! LEGEND_AUDIO_AUDIO_MANAGER_H_
