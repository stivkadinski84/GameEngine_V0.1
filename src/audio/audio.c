#include "audio/audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

// Forward declaration for the internal Win32 audio thread driver
static DWORD WINAPI AudioThreadProc(LPVOID lpParam);
static void CALLBACK waveOutProcWrap(HWAVEOUT hWaveOut, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

// Direct sample clipping inline utility
static inline float audio_clip(float fSample, float fMax)
{
    if (fSample >= 0.0f)
        return (fSample > fMax) ? fMax : fSample;
    else
        return (fSample < -fMax) ? -fMax : fSample;
}

// ----------------------------------------------------------------------------
// Asset Management & Parsing
// ----------------------------------------------------------------------------

unsigned int Load_AudioSample(AudioEngine *audio, const wchar_t *sWavFile)
{
    if (!audio || !audio->bEnableSound)
        return -1;

    FILE *f = NULL;
    _wfopen_s(&f, sWavFile, L"rb");
    if (f == NULL)
        return -1;

    char dump[4];
    fread(&dump, sizeof(char), 4, f);
    if (strncmp(dump, "RIFF", 4) != 0)
    {
        fclose(f);
        return -1;
    }
    fread(&dump, sizeof(char), 4, f);
    fread(&dump, sizeof(char), 4, f);
    if (strncmp(dump, "WAVE", 4) != 0)
    {
        fclose(f);
        return -1;
    }

    fread(&dump, sizeof(char), 4, f);
    fread(&dump, sizeof(char), 4, f);

    AudioSample sample = {0};
    fread(&sample.wavHeader, sizeof(WAVEFORMATEX) - 2, 1, f);

    if (sample.wavHeader.wBitsPerSample != 16 || sample.wavHeader.nSamplesPerSec != 44100)
    {
        fclose(f);
        return -1;
    }

    long nChunksize = 0;
    fread(&dump, sizeof(char), 4, f);
    fread(&nChunksize, sizeof(long), 1, f);
    while (strncmp(dump, "data", 4) != 0)
    {
        fseek(f, nChunksize, SEEK_CUR);
        fread(&dump, sizeof(char), 4, f);
        fread(&nChunksize, sizeof(long), 1, f);
    }

    sample.nSamples = nChunksize / (sample.wavHeader.nChannels * (sample.wavHeader.wBitsPerSample >> 3));
    sample.nChannels = sample.wavHeader.nChannels;
    sample.fSample = (float *)malloc(sizeof(float) * sample.nSamples * sample.nChannels);

    if (!sample.fSample)
    {
        fclose(f);
        return -1;
    }

    float *pSample = sample.fSample;
    for (long i = 0; i < sample.nSamples; i++)
    {
        for (int c = 0; c < sample.nChannels; c++)
        {
            short s = 0;
            fread(&s, sizeof(short), 1, f);
            *pSample = (float)s / 32767.0f;
            pSample++;
        }
    }

    fclose(f);
    sample.bSampleValid = true;

    EnterCriticalSection(&audio->csMixer);
    if (audio->nAudioSamplesCount >= audio->nAudioSamplesCapacity)
    {
        audio->nAudioSamplesCapacity = audio->nAudioSamplesCapacity == 0 ? 8 : audio->nAudioSamplesCapacity * 2;
        // FIX 1: Check realloc for NULL before using
        AudioSample *temp = (AudioSample *)realloc(audio->vecAudioSamples,
                                                   sizeof(AudioSample) * audio->nAudioSamplesCapacity);
        if (!temp)
        {
            LeaveCriticalSection(&audio->csMixer);
            return -1;
        }
        audio->vecAudioSamples = temp;
    }
    audio->vecAudioSamples[audio->nAudioSamplesCount] = sample;
    audio->nAudioSamplesCount++;
    unsigned int assignedID = audio->nAudioSamplesCount;
    LeaveCriticalSection(&audio->csMixer);

    return assignedID;
}

void Play_Sample(AudioEngine *audio, int id, bool bLoop)
{
    if (!audio || id <= 0 || id > audio->nAudioSamplesCount)
        return;

    EnterCriticalSection(&audio->csMixer);
    if (audio->nActiveSamplesCount >= audio->nActiveSamplesCapacity)
    {
        audio->nActiveSamplesCapacity = audio->nActiveSamplesCapacity == 0 ? 8 : audio->nActiveSamplesCapacity * 2;
        // FIX 2: Check realloc for NULL before using
        CurrentlyPlayingSample *temp = (CurrentlyPlayingSample *)realloc(
            audio->listActiveSamples,
            sizeof(CurrentlyPlayingSample) * audio->nActiveSamplesCapacity);
        if (!temp)
        {
            LeaveCriticalSection(&audio->csMixer);
            return;
        }
        audio->listActiveSamples = temp;
    }

    CurrentlyPlayingSample playing = {.nAudioSampleID = id, .nSamplePosition = 0, .bFinished = false, .bLoop = bLoop};
    audio->listActiveSamples[audio->nActiveSamplesCount] = playing;
    audio->nActiveSamplesCount++;
    LeaveCriticalSection(&audio->csMixer);
}

void Stop_Sample(AudioEngine *audio, int id)
{
    if (!audio)
        return;
    EnterCriticalSection(&audio->csMixer);
    for (int i = 0; i < audio->nActiveSamplesCount; i++)
    {
        if (audio->listActiveSamples[i].nAudioSampleID == id)
        {
            audio->listActiveSamples[i].bFinished = true;
        }
    }
    LeaveCriticalSection(&audio->csMixer);
}

// ----------------------------------------------------------------------------
// Hardware Control & Core Mixer Loop
// ----------------------------------------------------------------------------

bool Create_Audio(AudioEngine *audio, unsigned int nSampleRate, unsigned int nChannels, unsigned int nBlocks, unsigned int nBlockSamples)
{
    if (!audio)
        return false;

    audio->bAudioThreadActive = false;
    audio->nSampleRate = nSampleRate;
    audio->nChannels = nChannels;
    audio->nBlockCount = nBlocks;
    audio->nBlockSamples = nBlockSamples;
    audio->nBlockFree = nBlocks;
    audio->nBlockCurrent = 0;
    audio->pBlockMemory = NULL;
    audio->pWaveHeaders = NULL;
    audio->bEnableSound = true;
    audio->fGlobalTime = 0.0f;

    audio->vecAudioSamples = NULL;
    audio->nAudioSamplesCount = 0;
    audio->nAudioSamplesCapacity = 0;

    audio->listActiveSamples = NULL;
    audio->nActiveSamplesCount = 0;
    audio->nActiveSamplesCapacity = 0;

    InitializeCriticalSection(&audio->csMixer);
    audio->hEventBlockNotZero = CreateEvent(NULL, FALSE, FALSE, NULL);

    WAVEFORMATEX waveFormat = {
        .wFormatTag = WAVE_FORMAT_PCM,
        .nSamplesPerSec = audio->nSampleRate,
        .wBitsPerSample = sizeof(short) * 8,
        .nChannels = audio->nChannels,
        .cbSize = 0};
    waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;

    if (waveOutOpen(&audio->hwDevice, WAVE_MAPPER, &waveFormat, (DWORD_PTR)waveOutProcWrap, (DWORD_PTR)audio, CALLBACK_FUNCTION) != MMSYSERR_NOERROR)
    {
        Destroy_Audio(audio);
        return false;
    }

    audio->pBlockMemory = (short *)malloc(sizeof(short) * audio->nBlockCount * audio->nBlockSamples);
    if (!audio->pBlockMemory)
    {
        Destroy_Audio(audio);
        return false;
    }
    memset(audio->pBlockMemory, 0, sizeof(short) * audio->nBlockCount * audio->nBlockSamples);

    audio->pWaveHeaders = (WAVEHDR *)malloc(sizeof(WAVEHDR) * audio->nBlockCount);
    if (!audio->pWaveHeaders)
    {
        Destroy_Audio(audio);
        return false;
    }
    memset(audio->pWaveHeaders, 0, sizeof(WAVEHDR) * audio->nBlockCount);

    for (unsigned int n = 0; n < audio->nBlockCount; n++)
    {
        audio->pWaveHeaders[n].dwBufferLength = audio->nBlockSamples * sizeof(short);
        audio->pWaveHeaders[n].lpData = (LPSTR)(audio->pBlockMemory + (n * audio->nBlockSamples));
    }

    audio->bAudioThreadActive = true;
    audio->hAudioThread = CreateThread(NULL, 0, AudioThreadProc, audio, 0, NULL);
    if (!audio->hAudioThread)
    {
        Destroy_Audio(audio);
        return false;
    }

    SetEvent(audio->hEventBlockNotZero);
    return true;
}

bool Destroy_Audio(AudioEngine *audio)
{
    if (!audio)
        return false;

    audio->bAudioThreadActive = false;
    if (audio->hAudioThread)
    {
        SetEvent(audio->hEventBlockNotZero);
        WaitForSingleObject(audio->hAudioThread, INFINITE);
        CloseHandle(audio->hAudioThread);
        audio->hAudioThread = NULL;
    }

    if (audio->hwDevice)
    {
        waveOutReset(audio->hwDevice);
        if (audio->pWaveHeaders)
        {
            for (unsigned int n = 0; n < audio->nBlockCount; n++)
            {
                if (audio->pWaveHeaders[n].dwFlags & WHDR_PREPARED)
                {
                    waveOutUnprepareHeader(audio->hwDevice, &audio->pWaveHeaders[n], sizeof(WAVEHDR));
                }
            }
        }
        waveOutClose(audio->hwDevice);
        audio->hwDevice = NULL;
    }

    if (audio->pBlockMemory)
    {
        free(audio->pBlockMemory);
        audio->pBlockMemory = NULL;
    }
    if (audio->pWaveHeaders)
    {
        free(audio->pWaveHeaders);
        audio->pWaveHeaders = NULL;
    }
    if (audio->hEventBlockNotZero)
    {
        CloseHandle(audio->hEventBlockNotZero);
        audio->hEventBlockNotZero = NULL;
    }

    if (audio->vecAudioSamples)
    {
        for (int i = 0; i < audio->nAudioSamplesCount; i++)
        {
            if (audio->vecAudioSamples[i].fSample)
                free(audio->vecAudioSamples[i].fSample);
        }
        free(audio->vecAudioSamples);
        audio->vecAudioSamples = NULL;
    }

    if (audio->listActiveSamples)
    {
        free(audio->listActiveSamples);
        audio->listActiveSamples = NULL;
    }

    DeleteCriticalSection(&audio->csMixer);
    return false;
}

static void CALLBACK waveOutProcWrap(HWAVEOUT hWaveOut, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    if (uMsg != WOM_DONE)
        return;
    AudioEngine *audio = (AudioEngine *)dwInstance;
    // FIX 3: Use interlocked increment for thread safety (replaces std::atomic)
    InterlockedIncrement((LONG volatile *)&audio->nBlockFree);
    SetEvent(audio->hEventBlockNotZero);
}

float Get_MixerOutput(AudioEngine *audio, int nChannel, float fGlobalTime, float fTimeStep)
{
    float fMixerSample = 0.0f;

    EnterCriticalSection(&audio->csMixer);
    for (int i = 0; i < audio->nActiveSamplesCount; i++)
    {
        CurrentlyPlayingSample *s = &audio->listActiveSamples[i];
        AudioSample *sampleAsset = &audio->vecAudioSamples[s->nAudioSampleID - 1];

        s->nSamplePosition += (long)((float)sampleAsset->wavHeader.nSamplesPerSec * fTimeStep);

        if (s->nSamplePosition < sampleAsset->nSamples)
        {
            fMixerSample += sampleAsset->fSample[(s->nSamplePosition * sampleAsset->nChannels) + nChannel];
        }
        else
        {
            if (s->bLoop)
            {
                s->nSamplePosition = 0;
            }
            else
            {
                s->bFinished = true;
            }
        }
    }

    for (int i = 0; i < audio->nActiveSamplesCount;)
    {
        if (audio->listActiveSamples[i].bFinished)
        {
            for (int j = i; j < audio->nActiveSamplesCount - 1; j++)
            {
                audio->listActiveSamples[j] = audio->listActiveSamples[j + 1];
            }
            audio->nActiveSamplesCount--;
        }
        else
        {
            i++;
        }
    }
    LeaveCriticalSection(&audio->csMixer);

    return fMixerSample;
}

static DWORD WINAPI AudioThreadProc(LPVOID lpParam)
{
    AudioEngine *audio = (AudioEngine *)lpParam;
    audio->fGlobalTime = 0.0f;
    float fTimeStep = 1.0f / (float)audio->nSampleRate;

    short nMaxSample = (short)pow(2, (sizeof(short) * 8) - 1) - 1;
    float fMaxSample = (float)nMaxSample;

    while (audio->bAudioThreadActive)
    {
        if (audio->nBlockFree == 0)
        {
            while (audio->nBlockFree == 0 && audio->bAudioThreadActive)
            {
                WaitForSingleObject(audio->hEventBlockNotZero, 10);
            }
        }

        if (!audio->bAudioThreadActive)
            break;
        // FIX 3: Use interlocked decrement for thread safety (replaces std::atomic)
        InterlockedDecrement((LONG volatile *)&audio->nBlockFree);

        if (audio->pWaveHeaders[audio->nBlockCurrent].dwFlags & WHDR_PREPARED)
        {
            waveOutUnprepareHeader(audio->hwDevice, &audio->pWaveHeaders[audio->nBlockCurrent], sizeof(WAVEHDR));
        }

        int nCurrentBlock = audio->nBlockCurrent * audio->nBlockSamples;

        for (unsigned int n = 0; n < audio->nBlockSamples; n += audio->nChannels)
        {
            for (unsigned int c = 0; c < audio->nChannels; c++)
            {
                short nNewSample = (short)(audio_clip(Get_MixerOutput(audio, c, audio->fGlobalTime, fTimeStep), 1.0f) * fMaxSample);
                audio->pBlockMemory[nCurrentBlock + n + c] = nNewSample;
            }
            audio->fGlobalTime += fTimeStep;
        }

        waveOutPrepareHeader(audio->hwDevice, &audio->pWaveHeaders[audio->nBlockCurrent], sizeof(WAVEHDR));
        waveOutWrite(audio->hwDevice, &audio->pWaveHeaders[audio->nBlockCurrent], sizeof(WAVEHDR));
        audio->nBlockCurrent = (audio->nBlockCurrent + 1) % audio->nBlockCount;
    }

    return 0;
}