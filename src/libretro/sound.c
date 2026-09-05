#include "driver.h"
#include "minimal.h"
#include "libretro.h"
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

int samples_per_frame = 0;
short *samples_buffer = NULL;
int usestereo = 1;

void hook_audio_done(void);

int osd_start_audio_stream(int stereo)
{
    usestereo = stereo ? 1 : 0;

    if (Machine == NULL || Machine->sample_rate == 0 || Machine->drv == NULL || Machine->drv->frames_per_second == 0)
        return 0;

    samples_per_frame = Machine->sample_rate / Machine->drv->frames_per_second;

    // Prevent leaks if audio stream restarts
    if (samples_buffer) {
        free(samples_buffer);
        samples_buffer = NULL;
    }

    // PS2 R5900 optimization: Align buffer to 64-byte cache line boundaries for optimal cache performance
    size_t buffer_size = samples_per_frame * 2 * sizeof(short);
    samples_buffer = (short *)memalign(64, buffer_size);
    if (!samples_buffer)
        return 0;

    memset(samples_buffer, 0, buffer_size);

    return samples_per_frame;
}

void osd_stop_audio_stream(void)
{
    samples_per_frame = 0;
    if (samples_buffer) {
        free(samples_buffer);
        samples_buffer = NULL;
    }
}

int osd_update_audio_stream(int16_t *buffer)
{
    (void)buffer;
    hook_audio_done();
    return samples_per_frame;
}

extern retro_audio_sample_batch_t audio_batch_cb;

void osd_update_silent_stream(void)
{
    if (Machine == NULL || Machine->sample_rate == 0 || samples_buffer == NULL || samples_per_frame <= 0)
        return;

    memset(samples_buffer, 0, samples_per_frame * 2 * sizeof(short));
    if (audio_batch_cb) 
        audio_batch_cb(samples_buffer, samples_per_frame);
}

void osd_set_mastervolume(int _attenuation)
{
    (void)_attenuation;
}

int osd_get_mastervolume(void)
{
    return 100;
}

void osd_sound_enable(int enable_it)
{
    (void)enable_it;
}

void osd_opl_control(int chip, int reg)
{
    (void)chip;
    (void)reg;
}

void osd_opl_write(int chip, int data)
{
    (void)chip;
    (void)data;
}