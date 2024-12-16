#pragma once

#include <unordered_map>
#include <random> 

#include "Particle.h"
#include "DirectXCommon.h"

#include "ParticleManager.h"

class ParticleEmitter
{
public:

	ParticleEmitter
	(
		std::mt19937 randomEngine,
		std::unordered_map<std::string, ParticleGroup> particleGroups, 
		float emissionInterval
	);

	void Update(float deltaTime);

	void Emit(const std::string& name, const Vector3& translate, uint32_t count);

private:

	// 乱数生成
	std::mt19937 randomEngine_;

	std::unordered_map<std::string, ParticleGroup> particleGroups_;

	float emissionInterval_;  // 発生間隔
	float nextEmissionTime_;  // 次回発生時刻
};

