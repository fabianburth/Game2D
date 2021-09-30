#pragma once
#pragma once
#include <map>
#include <string>
/*#include <xaudio2.h>
#include "AudioData.h"

#include <tchar.h>


class SoundManager
{
private:
    /**
    * Storing the instance
    */
// static SoundManager* soundManager;

/**
 * Private Constructor to prevent instancing
 */
/*SoundManager();

HRESULT hr = 0;

/** XAudio2 Engine */
/*IXAudio2* pXAudio2 = nullptr;
/** Mastering Voice */
/*IXAudio2MasteringVoice* pMasterVoice = nullptr;

std::map<std::string, AudioData> audios;
std::map<std::string, IXAudio2SourceVoice*> soundeffect;

HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);


public:
/** Returns a pointer to the singleton instance */
/*static SoundManager* getInstance();

HRESULT loadAudioData(const TCHAR* filePath, std::string name, bool loop);
HRESULT play(std::string name);
HRESULT stop(std::string name);
};*/