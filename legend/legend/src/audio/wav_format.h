#ifndef LEGEND_AUDIO_WAV_FORMAT_H_
#define LEGEND_AUDIO_WAV_FORMAT_H_

typedef struct {
    WORD formatTag;
    WORD channels;
    DWORD samples_per_sec;
    DWORD avg_bytes_per_sec;
    WORD block_align;
    WORD bits_pre_sample;
    WORD cd_size;
} wav_format;


#endif //! LEGEND_AUDIO_WAV_FORMAT_H_
