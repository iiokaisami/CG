#pragma once

#include <xaudio2.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#pragma comment(lib,"xaudio2.lib")

class Audio
{
public:

    Audio();
    ~Audio();

    // サウンドファイルをロード
    bool LoadSound(const std::string& soundName, const std::wstring& filePath);

    // サウンドの再生
    void Play(const std::string& soundName, bool loop = false);

    // サウンドを停止（特定の音）
    void Stop(const std::string& soundName);

    // 全サウンド停止
    void StopAll();

    // 音量の設定
    void SetVolume(const std::string& soundName, float volume); // サウンドごとの音量
    void SetMasterVolume(float volume); // マスターボイスの音量

private: // 構造体

    // チャンクヘッダ
    struct ChunkHeader {
        char id[4];		 // チャンク毎のID
        int32_t size;	 // チャンクサイズ
    };

    // RIFFヘッダチャンク
    struct RiffHeader {
        ChunkHeader chunk;	// "RIFF"
        char type[4];		// "WAVE"
    };

    // FMTチャンク
    struct FormatChunk {
        ChunkHeader chunk;	// "fmt"
        WAVEFORMATEX fmt;	// 波形フォーマット
    };

    // サウンドデータ
    struct SoundData {
        std::vector<BYTE> buffer;
        WAVEFORMATEX format;
    };

private:

    // WAV ファイル読み込み関数
    bool LoadWavFile(/*const std::wstring& directoryPath,*/ const std::wstring& filePath, SoundData& soundData);

    // 解放用関数
    void Release();

private:
    // XAudio2 関連のリソース
    IXAudio2* xAudio2_ = nullptr;
    IXAudio2MasteringVoice* masteringVoice_ = nullptr;

    // サウンドデータのマッピング
    std::unordered_map<std::string, SoundData> soundDataMap_;

    // ソースボイスのリスト
    std::unordered_map<std::string, std::vector<IXAudio2SourceVoice*>> sourceVoices_;

};

