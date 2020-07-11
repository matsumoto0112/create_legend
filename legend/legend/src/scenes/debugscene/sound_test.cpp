#include "src/scenes/debugscene/sound_test.h"

#include "src/audio/audio_manager.h"
#include "src/game/game_device.h"
#include "src/util/path.h"
#include "src/util/string_util.h"

namespace legend {
namespace scenes {
namespace debugscene {
SoundTest::SoundTest(ISceneChange* scene_change) : Scene(scene_change) {}
SoundTest::~SoundTest() {}
void SoundTest::Initialize() {
  audio::AudioManager& audio_manager =
      game::GameDevice::GetInstance()->GetAudioManager();

  std::filesystem::path path =
      util::Path::GetInstance()->exe() / L"assets" / L"audios" / L"free_2.wav";
  audio_manager.LoadWav(path, AudioType::SE);
  path =
      util::Path::GetInstance()->exe() / L"assets" / L"audios" / L"free_3.wav";
  audio_manager.LoadWav(path, AudioType::BGM);
}
void SoundTest::Update() {
  audio::AudioManager& audio_manager =
      game::GameDevice::GetInstance()->GetAudioManager();

  if (ImGui::Begin("SoundTest")) {
    float master_volume_ = audio_manager.GetMasterVolume();
    if (ImGui::SliderFloat("MasterVolume", &master_volume_, 0.0f, 10.0f)) {
      audio_manager.SetMasterVolume(master_volume_);
    }

    if (ImGui::Button("SE")) {
      std::filesystem::path path = 
          util::Path::GetInstance()->exe() / L"assets" / L"audios" / L"free_2.wav";
      audio_manager.Start(path, 1.0f);
    }

    if (ImGui::Button("BGM")) {
      std::filesystem::path path =
          util::Path::GetInstance()->exe() / L"assets" / L"audios" / L"free_3.wav";
      audio_manager.Start(path, 1.0f);
    }
  }

  ImGui::End();

  if (ImGui::Begin("PlayingSoundList")) {
    for (auto&& audio : audio_manager.audiosources_) {
      ImGui::SetNextTreeNodeOpen(true, ImGuiCond_::ImGuiCond_Once);
      if (ImGui::TreeNode(std::to_string(audio.first).c_str())) {
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
}
void SoundTest::Draw() {}
void SoundTest::Finalize() {}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
