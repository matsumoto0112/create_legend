#include "src/scenes/debugscene/sound_test.h"

#include "src/audio/audio_manager.h"
#include "src/game/game_device.h"
#include "src/util/path.h"

namespace legend {
namespace scenes {
namespace debugscene
{
SoundTest::SoundTest(ISceneChange* scene_change) : Scene(scene_change) {}
SoundTest::~SoundTest() {}
void SoundTest::Initialize() {
  audio::AudioManager& audio_manager =
      game::GameDevice::GetInstance()->GetAudioManager();

  std::filesystem::path path =
      util::Path::GetInstance()->exe() / L"assets" / L"audios" / L"free_3.wav";
  audio_manager.LoadWav(path);
}
void SoundTest::Update() {
  audio::AudioManager& audio_manager =
      game::GameDevice::GetInstance()->GetAudioManager();

  if (ImGui::Begin("SoundTest")) {
    if (ImGui::Button("SE")) {
      std::filesystem::path path = util::Path::GetInstance()->exe() /
                                   L"assets" / L"audios" / L"free_3.wav";
      audio_manager.Start(path, 1.0f);
    }
  }
  ImGui::End();
}
void SoundTest::Draw() {}
void SoundTest::Finalize() {}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
