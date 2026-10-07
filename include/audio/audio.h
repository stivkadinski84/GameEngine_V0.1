#ifndef AUDIO_H
#define AUDIO_H

#include <windows.h>
#include <mmsystem.h>
#include <stdbool.h>

// Link the Windows Multimedia library automatically
// #pragma comment(lib, "winmm.lib")

// Raw floating-point audio asset buffer
typedef struct
{
    WAVEFORMATEX wavHeader;
    float *fSample;
    long nSamples;
    int nChannels;
    bool bSampleValid;
} AudioSample;

// Instance tracking for sounds actively moving through the mixer
typedef struct
{
    int nAudioSampleID; // 1-based index into our sample array (0 means empty/inactive)
    long nSamplePosition;
    bool bFinished;
    bool bLoop;
} CurrentlyPlayingSample;

// Main Audio Subsystem State
typedef struct
{
    unsigned int nSampleRate;
    unsigned int nChannels;
    unsigned int nBlockCount;
    unsigned int nBlockSamples;
    unsigned int nBlockCurrent;

    short *pBlockMemory;
    WAVEHDR *pWaveHeaders;
    HWAVEOUT hwDevice;

    HANDLE hAudioThread;
    unsigned int nBlockFree;
    HANDLE hEventBlockNotZero;
    CRITICAL_SECTION csMixer; // Protects active list modifications across threads

    float fGlobalTime;
    bool bAudioThreadActive;
    bool bEnableSound;

    // Track dynamic allocations for our loaded assets and active mixing channels
    AudioSample *vecAudioSamples;
    int nAudioSamplesCount;
    int nAudioSamplesCapacity;

    CurrentlyPlayingSample *listActiveSamples;
    int nActiveSamplesCount;
    int nActiveSamplesCapacity;
} AudioEngine;

// Engine Management
bool Create_Audio(AudioEngine *audio, unsigned int nSampleRate, unsigned int nChannels, unsigned int nBlocks, unsigned int nBlockSamples);
bool Destroy_Audio(AudioEngine *audio);

// Playback Controls
unsigned int Load_AudioSample(AudioEngine *audio, const wchar_t *sWavFile);
void Play_Sample(AudioEngine *audio, int id, bool bLoop);
void Stop_Sample(AudioEngine *audio, int id);

// Internal Mixer Logic (can be called by user code or overrides)
float Get_MixerOutput(AudioEngine *audio, int nChannel, float fGlobalTime, float fTimeStep);

#endif