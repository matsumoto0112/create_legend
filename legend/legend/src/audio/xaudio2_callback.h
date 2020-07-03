#ifndef LEGEND_AUDIO_XAUDIO2_CALLBACK_H_
#define LEGEND_AUDIO_XAUDIO2_CALLBACK_H_

#include <xaudio2.h>
//#include

#pragma comment(lib, "xaudio2.lib")

namespace legend {
namespace audio {
/**
 * @file xaudio2_callback.h
 * @brief XAudio2コールバッククラス
 */
class XAudio2CallBack : public IXAudio2VoiceCallback {
 public:
  HANDLE event;
  XAudio2CallBack() : event(CreateEvent(NULL, FALSE, FALSE, NULL)) {}
  ~XAudio2CallBack() { CloseHandle(event); }
  void STDMETHODCALLTYPE OnStreamEnd() { printf("%s\n", __func__); }
  void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() {}
  void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 SamplesRequired) {}
  void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext) { SetEvent(event); }
  void STDMETHODCALLTYPE OnBufferStart(void* pBufferContext) {}
  void STDMETHODCALLTYPE OnLoopEnd(void* pBufferContext) {
    printf("%s\n", __func__);
  }
  void STDMETHODCALLTYPE OnVoiceError(void* pBufferContext, HRESULT Error) {
    printf("%s\n", __func__);
  }
};

}  // namespace audio
}  // namespace legend
#endif  //! LEGEND_AUDIO_XAUDIO2_CALLBACK_H_