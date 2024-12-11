#include "Audio.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <iostream>

Audio::Audio()
{
    // XAudio2 を初期化
    if (FAILED(XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR)))
    {
        throw std::runtime_error("Failed to initialize XAudio2");
    }

    // マスターボイスを作成
    if (FAILED(xAudio2_->CreateMasteringVoice(&masteringVoice_))) 
    {
        throw std::runtime_error("Failed to create mastering voice");
    }
}

Audio::~Audio()
{
    StopAll();
    Release();
    
    if (masteringVoice_)
    {
        masteringVoice_->DestroyVoice();
    }
 
    if (xAudio2_)
    {
        xAudio2_->Release();
    }
}

bool Audio::LoadSound(const std::string& soundName, const std::wstring& filePath)
{
    // ファイルパスからディレクトリとファイル名を抽出
    //std::filesystem::path path(filePath);
    //std::string directory = "resources/audio" + path.parent_path().string();   
    //std::wstring wDirectory = std::wstring(directory.begin(), directory.end()); // directory を std::wstring に変換
    //std::wstring fileName = path.filename().wstring(); // ファイル名だけを抽出

    //std::wstring fullPath = path.wstring();

    SoundData soundData;
    if (!LoadWavFile(/*wDirectory,fileName*/filePath, soundData))
    {
        //std::wcerr << L"Failed to load sound: " << fullPath << std::endl;
        return false;
    }
    soundDataMap_[soundName] = std::move(soundData);
    return true;
}

void Audio::Play(const std::string& soundName, bool loop)
{
    auto it = soundDataMap_.find(soundName);
    if (it == soundDataMap_.end())
    {
        std::cerr << "Sound not found: " << soundName << std::endl;
        return; // サウンドが存在しない
    }

    SoundData& soundData = it->second;

    // ソースボイスを作成
    IXAudio2SourceVoice* sourceVoice = nullptr;
    HRESULT hr = xAudio2_->CreateSourceVoice(&sourceVoice, &soundData.format);
    if (FAILED(hr))
    {
        std::cerr << "Failed to create source voice. Error: " << std::hex << hr << std::endl;
        return;
    }

    // 再生するバッファをキューに追加
    XAUDIO2_BUFFER buffer = {};
    buffer.AudioBytes = static_cast<UINT32>(soundData.buffer.size());
    buffer.pAudioData = soundData.buffer.data();
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;

    hr = sourceVoice->SubmitSourceBuffer(&buffer);
    if (FAILED(hr))
    {
        std::cerr << "Failed to submit source buffer. Error: " << std::hex << hr << std::endl;
        return;
    }

    // 再生開始
    hr = sourceVoice->Start(0);
    if (FAILED(hr))
    {
        std::cerr << "Failed to start playback. Error: " << std::hex << hr << std::endl;
        sourceVoice->DestroyVoice();
        return;
    }

    sourceVoices_[soundName].push_back(sourceVoice);
}

void Audio::Stop(const std::string& soundName)
{
    auto it = sourceVoices_.find(soundName);
    if (it != sourceVoices_.end()) 
    {
        for (auto* sourceVoice : it->second)
        {
            sourceVoice->Stop(0);
            sourceVoice->DestroyVoice();
        }
        it->second.clear();
        sourceVoices_.erase(it);
    }
}

void Audio::StopAll()
{
    for (auto& [soundName, voices] : sourceVoices_)
    {
        for (auto* sourceVoice : voices) 
        {
            sourceVoice->Stop(0);
            sourceVoice->DestroyVoice();
        }
    }
    sourceVoices_.clear();
}

void Audio::SetVolume(const std::string& soundName, float volume)
{
    auto it = sourceVoices_.find(soundName);
    if (it != sourceVoices_.end())
    {
        for (auto* sourceVoice : it->second)
        {
            // 音量を設定（0.0f から 1.0f の範囲）
            sourceVoice->SetVolume(volume);
        }
    }
}

void Audio::SetMasterVolume(float volume)
{
    // マスターボイスの音量設定（0.0f から 1.0f の範囲）
    masteringVoice_->SetVolume(volume);
}

bool Audio::LoadWavFile(/*const std::wstring& directoryPath,*/ const std::wstring& filePath, SoundData& soundData)
{

    /*std::wstring fullPath = directoryPath + L"/" + filePath;
    std::wcout << L"Loading WAV file from path: " << fullPath << std::endl;*/

    std::ifstream file(/*directoryPath + L"/" +*/ filePath, std::ios::binary);
    if (!file)
    {
       // std::wcerr << L"File not found: " << fullPath << std::endl;
        return false; // ファイルが見つからない
    }

    // RIFF チャンクを確認
    char riff[4];
    file.read(riff, 4);
    if (std::memcmp(riff, "RIFF", 4) != 0)
    {
        //std::wcerr << L"Invalid RIFF header in file: " << fullPath << std::endl;
        return false;
    }

    file.seekg(22); // WAVEFORMATEX 構造体が始まるオフセット
    file.read(reinterpret_cast<char*>(&soundData.format), sizeof(WAVEFORMATEX));

    file.seekg(44); // データチャンクが始まるオフセット
    soundData.buffer.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

    return true;
}

void Audio::Release()
{
    soundDataMap_.clear();
    StopAll();
}
