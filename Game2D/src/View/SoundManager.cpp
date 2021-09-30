#include "SoundManager.h"

#ifdef _XBOX// Big-Endian
    #define fourccRIFF 'RIFF'
    #define fourccDATA 'data'
    #define fourccFMT 'fmt '
    #define fourccWAVE 'WAVE'
    #define fourccXWMA 'XWMA'
    #define fourccDPDS 'dpds'
#endif

#ifndef _XBOX// Little-Endian
    #define fourccRIFF 'FFIR'
    #define fourccDATA 'atad'
    #define fourccFMT ' tmf'
    #define fourccWAVE 'EVAW'
    #define fourccXWMA 'AMWX'
    #define fourccDPDS 'sdpd'
#endif

/*SoundManager* SoundManager::soundManager = nullptr;

SoundManager::SoundManager()
{
    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr))
        return;

    if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
        return;

    if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
        return;

    //std::string initSourceVoiceAudioName = "destroyBlock";
    //loadAudioData(_TEXT("res\\sounds\\PushIceBlock.wav"), initSourceVoiceAudioName);
    //createSourceVoice(audios[initSourceVoiceAudioName].wfx);
}

HRESULT SoundManager::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());

    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD bytesRead = 0;
    DWORD dwOffset = 0;

    while (hr == S_OK)
    {
        DWORD dwRead;
        if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());

        if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());

        switch (dwChunkType)
        {
        case fourccRIFF:
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
                hr = HRESULT_FROM_WIN32(GetLastError());
            break;

        default:
            if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
                return HRESULT_FROM_WIN32(GetLastError());
        }

        dwOffset += sizeof(DWORD) * 2;

        if (dwChunkType == fourcc)
        {
            dwChunkSize = dwChunkDataSize;
            dwChunkDataPosition = dwOffset;
            return S_OK;
        }

        dwOffset += dwChunkDataSize;

        if (bytesRead >= dwRIFFDataSize) return S_FALSE;

    }

    return S_OK;
}

HRESULT SoundManager::ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());
    DWORD dwRead;
    if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
        hr = HRESULT_FROM_WIN32(GetLastError());
    return hr;
}

SoundManager* SoundManager::getInstance()
{
    if (soundManager == nullptr) {
        soundManager = new SoundManager();
    }

    return soundManager;
}

HRESULT SoundManager::loadAudioData(const TCHAR* filePath, std::string name, bool loop)
{
    audios[name] = AudioData();

#ifdef _XBOX
    //char* strFileName = "game:\\media\\MusicMono.wav";
#else
    const TCHAR* strFileName = filePath; //_TEXT("media\\MusicMono.wav");
#endif
    // Open the file
    HANDLE hFile = CreateFile(
        strFileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (INVALID_HANDLE_VALUE == hFile)
        return HRESULT_FROM_WIN32(GetLastError());

    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());

    // Locate 'RIFF' chunk
    DWORD dwChunkSize;
    DWORD dwChunkPosition;
    //check the file type, should be fourccWAVE or 'XWMA'
    FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
    DWORD filetype;
    ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
    if (filetype != fourccWAVE)
        return S_FALSE;

    // Locate the 'fmt ' chunk, and copy its contents into a WAVEFORMATEXTENSIBLE structure
    FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
    ReadChunkData(hFile, &audios[name].wfx, dwChunkSize, dwChunkPosition);

    //fill out the audio data buffer with the contents of the fourccDATA chunk
    FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
    BYTE* pDataBuffer = new BYTE[dwChunkSize];
    ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

    audios[name].buffer.AudioBytes = dwChunkSize;  //size of the audio buffer in bytes
    audios[name].buffer.pAudioData = pDataBuffer;  //buffer containing audio data
    if (loop) {
        audios[name].buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    }
    audios[name].buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

    IXAudio2SourceVoice* pSourceVoice;
    if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&audios[name].wfx)))
        return hr;

    soundeffect[name] = pSourceVoice;

    return 0;
}


HRESULT SoundManager::play(std::string name)
{
    //if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&audios.at(name).buffer)))
    //    return hr;
    if (FAILED(hr = soundeffect[name]->SubmitSourceBuffer(&audios.at(name).buffer)))
        return hr;

    if (FAILED(hr = soundeffect[name]->Start(0)))
        return hr;

    return 0;
}

HRESULT SoundManager::stop(std::string name)
{
    if (FAILED(hr = soundeffect[name]->Stop(0)))
        return hr;

    if (FAILED(hr = soundeffect[name]->FlushSourceBuffers()))
        return hr;

    return 0;
}*/
