#include "src/scenes/debugscene/sound_test.h"

#include "src/audio/audio_manager.h"
#include "src/game/game_device.h"
#include "src/util/path.h"
#include "src/util/string_util.h"

namespace legend {
namespace scenes {
namespace debugscene {
SoundTest::SoundTest(ISceneChange* scene_change) : Scene(scene_change) {}
SoundTest::~SoundTest() {
    game::GameDevice::GetInstance()->GetDevice().WaitForGPU();
}
bool SoundTest::Initialize() {
  audio::AudioManager& audio_manager =
      game::GameDevice::GetInstance()->GetAudioManager();

  audio_manager.LoadWav(L"free_2.wav", AudioType::SE);
  audio_manager.LoadWav(L"free_3.wav", AudioType::BGM);

  return true;
}
bool SoundTest::Update() {
  audio::AudioManager& audio_manager =
      game::GameDevice::GetInstance()->GetAudioManager();

  if (ImGui::Begin("SoundTest")) {
    float master_volume_ = audio_manager.GetMasterVolume();
    if (ImGui::SliderFloat("MasterVolume", &master_volume_, 0.0f, 10.0f)) {
      audio_manager.SetMasterVolume(master_volume_);
    }

    for (auto&& audio : audio_manager.base_audiosources_) {
        if (ImGui::Button((util::string_util::WString_2_String(audio.first)).c_str()))
        {
            audio_manager.Start(audio.first, 1.0f);
        }
    }
  }

  ImGui::End();

  if (ImGui::Begin("PlayingSoundList")) {
    for (auto&& audio : audio_manager.audiosources_) {
      ImGui::SetNextTreeNodeOpen(true, ImGuiCond_::ImGuiCond_Once);
      if (ImGui::TreeNode(
              (std::to_string(audio.first) + " " +
               util::string_util::WString_2_String(audio.second->GetFileName()))
                  .c_str())) {
        if (ImGui::Button(("Play" + std::to_string(audio.first)).c_str())) {
          audio_manager.Play(audio.first);
        }
        ImGui::SameLine();
        if (ImGui::Button(("Pause" + std::to_string(audio.first)).c_str())) {
          audio_manager.Pause(audio.first);
        }
        ImGui::SameLine();
        if (ImGui::Button(("Stop" + std::to_string(audio.first)).c_str())) {
          audio_manager.Stop(audio.first);
        }
        ImGui::SameLine();
        bool is_loop_ = audio.second->is_loop_;
        if (ImGui::Checkbox(("Loop" + std::to_string(audio.first)).c_str(),
                            &is_loop_)) {
          audio_manager.SetLoopFlag(audio.first, is_loop_);
        }
        float volume_ = audio.second->GetVolume();
        if (ImGui::SliderFloat(("Volume" + std::to_string(audio.first)).c_str(),
                               &volume_, 0.0f, 10.0f)) {
          audio_manager.SetVolume(audio.first, volume_);
        }
        ImGui::SameLine();
        if (ImGui::Button(
                ("VolumeReset" + std::to_string(audio.first)).c_str())) {
          audio_manager.SetVolume(audio.first, 1.0f);
        }
        float pitch_ = audio.second->GetPitch();
        if (ImGui::SliderFloat(("Pitch" + std::to_string(audio.first)).c_str(),
                               &pitch_, XAUDIO2_MIN_FREQ_RATIO, 2)) {
          audio_manager.SetPitch(audio.first, pitch_);
        }
        ImGui::SameLine();
        if (ImGui::Button(
                ("PitchReset" + std::to_string(audio.first)).c_str())) {
          audio_manager.SetPitch(audio.first, 1.0f);
        }

        ImGui::TreePop();
      }
    }
  }
  ImGui::End();

  return true;
}
void SoundTest::Draw() {}
void SoundTest::Finalize() {}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
