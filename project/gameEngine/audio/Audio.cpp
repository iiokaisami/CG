#include "Audio.h"

#include <fstream>
#include <stdexcept>

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
    SoundData soundData;
    if (!LoadWavFile(filePath, soundData))
    {
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
        return; // サウンドが存在しない
    }

    SoundData& soundData = it->second;

    // ソースボイスを作成
    IXAudio2SourceVoice* sourceVoice = nullptr;
    if (FAILED(xAudio2_->CreateSourceVoice(&sourceVoice, &soundData.format))) 
    {
        return;
    }

    // 再生するバッファをキューに追加
    XAUDIO2_BUFFER buffer = {};
    buffer.AudioBytes = static_cast<UINT32>(soundData.buffer.size());
    buffer.pAudioData = soundData.buffer.data();
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;

    if (FAILED(sourceVoice->SubmitSourceBuffer(&buffer))) 
    {
        sourceVoice->DestroyVoice();
        return;
    }

    // 再生開始
    if (FAILED(sourceVoice->Start(0))) 
    {
        sourceVoice->DestroyVoice();
        return;
    }

    sourceVoices_[soundName].push_back(sourceVoice);
}

void Audio::Stop(const std::string& soundName)
{
    auto it = sourceVoices_.find(soundName);
    if (it != sourceVoices_.end()) {
        for (auto* sourceVoice : it->second) {
            sourceVoice->Stop(0);
            sourceVoice->DestroyVoice();
        }
        it->second.clear();
        sourceVoices_.erase(it);
    }
}

void Audio::StopAll()
{
    for (auto& [soundName, voices] : sourceVoices_) {
        for (auto* sourceVoice : voices) {
            sourceVoice->Stop(0);
            sourceVoice->DestroyVoice();
        }
    }
    sourceVoices_.clear();
}

bool Audio::LoadWavFile(const std::wstring& filePath, SoundData& soundData)
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        return false; // ファイルが見つからない
    }

    // RIFF チャンクを確認
    char riff[4];
    file.read(riff, 4);
    if (std::memcmp(riff, "RIFF", 4) != 0) {
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
