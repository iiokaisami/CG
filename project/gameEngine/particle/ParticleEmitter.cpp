#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter
(
    std::mt19937 randomEngine, 
    std::unordered_map<std::string, ParticleGroup>& particleGroups,
    float emissionInterval
)
    : randomEngine_(randomEngine), 
    particleGroups_(particleGroups),
    emissionInterval_(emissionInterval), 
    nextEmissionTime_(0.0f)
{
}

void ParticleEmitter::Update(float deltaTime)
{
    // 次回の発生時刻を進める
    nextEmissionTime_ -= deltaTime;

    // 発生時刻を超えたらパーティクルを発生
    if (nextEmissionTime_ <= 0.0f)
    {
        // パーティクルを発生させる
        for (auto& group : particleGroups_)
        {
            ParticleGroup& particleGroup = group.second;

            // Emit関数を呼び出してパーティクルを発生
            Emit(group.first, particleGroup.particleList.front().transform.translate, 10); // 例として10個のパーティクルを発生させる
        }

        // 次回発生時刻を設定
        nextEmissionTime_ += emissionInterval_;
    }
}

void ParticleEmitter::Emit(const std::string& name, const Vector3& translate, uint32_t count)
{
    // ParticleManagerが登録済みか確認し、Emitを呼び出す
    assert(particleGroups_.find(name) != particleGroups_.end() && "Particle group not found!");

    // ParticleManagerにEmitを呼び出してパーティクルを生成
    ParticleManager::GetInstance()->Emit(name, translate, count);
}