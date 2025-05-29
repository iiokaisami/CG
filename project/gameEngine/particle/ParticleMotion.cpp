#include "ParticleMotion.h"

#include <numbers>
#include <cmath>

// 登録用のマップ
std::unordered_map<std::string, ParticleMotion::MotionFunc> ParticleMotion::motions_;
std::string ParticleMotion::direction_;

void ParticleMotion::Register(const std::string& name, MotionFunc func)
{
	motions_[name] = func;
}

Particle ParticleMotion::Create(const std::string& name, std::mt19937& rand, const Vector3& pos)
{
    auto it = motions_.find(name);
    if (it != motions_.end()) 
    {
        return it->second(rand, pos);
    }

    // 無効な名前 → デフォルト粒子（何もしない）
    Particle p;
    p.transform.translate = pos;
    p.transform.scale = { 1, 1, 1 };
    p.color = { 1, 0, 0, 1 }; // 赤い警告粒子
    p.lifeTime = 1.0f;
    return p;
}

void ParticleMotion::Initialize()
{
	// モーションの登録
	Register("Homing", MakeHoming);
	Register("Orbit", MakeOrbit);
	Register("Explosion", MakeExplosion);
	Register("Fountain", MakeFountain);
	Register("Wiggle", MakeWiggle);
    Register("Cylinder", MakeCylinder);
    Register("Slash", MakeSlash);
}

const std::unordered_map<std::string, ParticleMotion::MotionFunc>& ParticleMotion::GetAll()
{
    return motions_;
}

Particle ParticleMotion::MakeHoming(std::mt19937& rand, const Vector3& target)
{
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    Particle particle;
    particle.transform.translate = target + Vector3(dist(rand), dist(rand), dist(rand));
	particle.transform.scale = { 1.0f, 1.0f, 1.0f }; // スケールを設定
    particle.velocity = Normalize(target - particle.transform.translate) * 0.5f;
    particle.lifeTime = 2.0f;
    particle.currentTime = 0.0f;
    return particle;

}

Particle ParticleMotion::MakeOrbit(std::mt19937& rand, const Vector3& target)
{
    std::uniform_real_distribution<float> distAngle(0.0f, 2.0f * std::numbers::pi_v<float>);
    float angle = distAngle(rand);
    float radius = 2.0f; // 半径を拡大して軌道を明確に

    // 初期位置は円周上
    Vector3 offset = Vector3(std::cos(angle) * radius, 0.0f, std::sin(angle) * radius);
    Vector3 tangent = Vector3(-std::sin(angle), 0.0f, std::cos(angle)); // 接線方向

    Particle particle;
    particle.transform.translate = target + offset;
    particle.transform.scale = { 1.0f, 1.0f, 1.0f }; // スケールを設定
    particle.velocity = tangent * 0.1f; // 円運動するように初期速度を設定
    particle.lifeTime = 3.0f;
    particle.currentTime = 0.0f;
    return particle;
}

Particle ParticleMotion::MakeExplosion(std::mt19937& rand, const Vector3& center)
{
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    Particle particle;
    particle.transform.translate = center;
    particle.transform.scale = { 1.0f, 1.0f, 1.0f }; // スケールを設定
    particle.velocity = Normalize(Vector3(dist(rand), dist(rand), dist(rand))) * 0.2f;
    particle.lifeTime = 2.5f;
    particle.currentTime = 0.0f;
    return particle;
}

Particle ParticleMotion::MakeFountain(std::mt19937& rand, const Vector3& base)
{
    std::uniform_real_distribution<float> distXY(-1.5f, 1.5f);
    Particle particle;
    particle.transform.translate = base + Vector3(distXY(rand), 0.0f, distXY(rand));
    particle.transform.scale = { 1.0f, 1.0f, 1.0f }; // スケールを設定
    particle.velocity = Vector3(0.0f, 0.1f, 0.0f);
    particle.lifeTime = 2.0f;
    particle.currentTime = 0.0f;
    return particle;
}

Particle ParticleMotion::MakeWiggle(std::mt19937& rand, const Vector3& origin)
{
    std::uniform_real_distribution<float> dist(-0.4f, 0.4f);
    Particle particle;
    particle.transform.translate = origin + Vector3(dist(rand), 0.0f, dist(rand));
    particle.transform.scale = { 1.0f, 1.0f, 1.0f }; // スケールを設定
    particle.velocity = Vector3(dist(rand), dist(rand), dist(rand)) * 0.5f;
    particle.lifeTime = 2.0f;
    particle.currentTime = 0.0f;
    return particle;
}

Particle ParticleMotion::MakeCylinder(std::mt19937& rand, const Vector3& position)
{
    rand;

    Particle p;
    p.transform.scale = { 1.0f, 1.0f, 1.0f };
    p.transform.translate = position;
    p.transform.scale = { 1.0f, 1.0f, 1.0f }; // スケールを設定
    p.velocity = { 0, 0, 0 };
    p.color = { 0.0f, 0.0f, 0.5f, 1.0f };
    p.lifeTime = 3.0f;
    p.currentTime = 0.0f;

    if (direction_ == "UP")
    {
        p.transform.rotate = { 0.0f, 0.0f, 3.14f };
        p.transform.translate.y += p.transform.scale.y * 2.0f;
    }
    else if (direction_ == "DOWN") 
    {
        p.transform.rotate = { 0.0f, 0.0f, 0.0f };
    }
    else if (direction_ == "LEFT") 
    {
        p.transform.rotate = { 0.0f, 0.0f, 1.57f };
    }
    else if (direction_ == "RIGHT") 
    {
        p.transform.rotate = { 0.0f, 0.0f, -1.57f };
    }

    return p;
}

Particle ParticleMotion::MakeSlash(std::mt19937& rand, const Vector3& translate)
{
    std::uniform_real_distribution<float> scaleDist(0.5f, 1.5f);
    std::uniform_real_distribution<float> rotateDist(-std::numbers::pi_v<float>, std::numbers::pi_v<float>);

    Particle p;
    p.transform.scale = { 0.1f, scaleDist(rand), 1.0f };
    p.transform.rotate = { 0.0f, 0.0f, rotateDist(rand) };
    p.transform.translate = translate;
    p.velocity = { 0, 0, 0 };
    p.color = { 1.0f, 1.0f, 1.0f, 1.0f };
    p.lifeTime = 1.0f;
    p.currentTime = 0.0f;

    return p;
}

void ParticleMotion::SetDirection(const std::string& direction)
{
	direction_ = direction;
}
