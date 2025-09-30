#pragma once

#include <sstream>

#include "../../../../gameEngine/level_editor/LevelDataLoader.h"

class EnemyManager;

class EnemyWaveState
{
public:

	EnemyWaveState(const std::string& _wave, EnemyManager* _pEnemyManager) : wave_(_wave), pEnemyManager_(_pEnemyManager) {};
	virtual~EnemyWaveState();

	// 初期化
	virtual void Initialize() = 0;
	
	// 更新
	virtual void Update() = 0;

	void SetLevelData(std::shared_ptr<LevelData> _levelData) { levelData_ = _levelData; }

protected:

	// CSVファイルの読み込み
	void LoadCSV(const std::string& csvPath);

	// 敵の発生コマンドを更新
	void UpdateEnemyPopCommands(EnemyManager* _pEnemyManager);

	// csvの更新
	void UpdateCSV(EnemyManager* _pEnemyManager);

	std::string wave_;
	EnemyManager* pEnemyManager_ = nullptr;

	
	// 敵待機タイマー
	uint32_t enemyWaitingTimer_ = 9;

	// レベルデータローダー
	std::shared_ptr<LevelData> levelData_ = nullptr;

	// 現在のウェーブ
	uint32_t currentWave_ = 1;



	// 敵の発生コマンド
	std::stringstream enemyPopCommands_;
	// 敵機中フラグ
	bool isEnemyWaiting_ = true;
	// 確認用csv更新用カウント
	uint32_t debugCount_ = 9;
};

