#pragma once

#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")
#include <fstream>
#include "wrl.h"

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
// 音声データ
struct SoundData {
	WAVEFORMATEX wfex;		// 波形フォーマット
	BYTE* pBuffer;			// バッファの先頭アドレス
	IXAudio2SourceVoice* sourceVoice; // ソースボイス
	unsigned int bufferSize;// バッファサイズ
};

class Audio
{
#pragma region シングルトンインスタンス
private:
	static Audio* instance;

	Audio() = default;
	~Audio() = default;
	Audio(Audio&) = delete;
	Audio& operator = (Audio&) = delete;

public:
	// シングルトンインスタンスの取得
	static Audio* GetInstance();
	// 終了
	void Finalize();
#pragma endregion シングルトンインスタンス

public:
	
	// 初期化
	void Initialize();
	
	// 音声データ読み込み
	SoundData LoadWav(const char* filename);

	// 音声データの解放
	void SoundUnload(Microsoft::WRL::ComPtr<IXAudio2> xAudio2, SoundData* soundData);

	// サウンド再生
	// 再生したいサウンドデータとループフラグ、ボリュームを引数に取る
	void PlayWave(const SoundData& soundData, bool loop, float volume);

	// サウンド停止
	// 停止したいサウンドデータを引数に取る
	void SoundStop(SoundData& soundData);

	// ボリュームを設定	
	// 設定したいサウンドデータとボリュームを引数に取る
	void SetVolume(SoundData& soundData, float volume);

public: // ゲッター

	// xAudioの取得
	Microsoft::WRL::ComPtr<IXAudio2> GetXAudio2() const { return xAudio2_; }

private:

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_ = nullptr;
	IXAudio2MasteringVoice* masterVoice_ = nullptr;

	HRESULT hr;
};

