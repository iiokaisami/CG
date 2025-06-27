#pragma once

#include <sstream>

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

protected:

	// CSVファイルの読み込み
	void LoadCSV(const std::string& csvPath);

	// 敵の発生コマンドを更新
	void UpdateEnemyPopCommands(EnemyManager* _pEnemyManager);

	std::string wave_;
	EnemyManager* pEnemyManager_ = nullptr;

	// 敵の発生コマンド
	std::stringstream enemyPopCommands_;
	// 敵機中フラグ
	bool isEnemyWaiting_ = true;
	// 敵待機タイマー
	int32_t enemyWaitingTimer_ = 9;

};

