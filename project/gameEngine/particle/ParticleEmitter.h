#pragma once

#include <unordered_map>
#include <random> 

#include "ParticleManager.h"

class ParticleEmitter
{
public:

	// 非インスタンス化でも使える静的ユーティリティとして提供
	static void Emit(const std::string& groupName, const Vector3& position, uint32_t count = 10);

	static void StartLoop(const std::string& groupName, const std::string& motionName, const Vector3& position, uint32_t count = 10);
	
	// --- 以下はインスタンス用：必要なら使う ---

	ParticleEmitter(ParticleManager* manager, const std::string& groupName, const std::string& motionName = "Homing")
		: manager_(manager), groupName_(groupName), motionName_(motionName)
	{
	}

	// 単発エミット
	void EmitOnce(const Vector3& position, uint32_t count = 10);

	// ループエミット（manager に設定を追加）
	void StartLoopEmit(const Vector3& position, uint32_t count);


private:

	// Managerクラス
	ParticleManager* manager_ = nullptr;
	
	// グループ名
	std::string groupName_;
	
	// モーション名
	std::string motionName_;

};

