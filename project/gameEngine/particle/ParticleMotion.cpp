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
    std::uniform_real_distribution<float> d(-1.5f, 1.5f);
    Vector3 offset = { d(rand), d(rand), d(rand) };
    Vector3 pos = target + offset;
    Vector3 dir = Normalize(target - pos);

    Particle p;
    p.transform.translate = pos;
    p.velocity = dir * 0.5f;
    p.transform.scale = { 1, 1, 1 };
    p.color = { 0, 1, 1, 1 };
    p.lifeTime = 3.0f;
    p.currentTime = 0;
    return p;

}

Particle ParticleMotion::MakeOrbit(std::mt19937& rand, const Vector3& target)
{
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * std::numbers::pi_v<float>);
    std::uniform_real_distribution<float> radiusDist(0.5f, 1.5f);
    float angle = angleDist(rand);
    float radius = radiusDist(rand);
    Vector3 offset = { std::cos(angle) * radius, 0.0f, std::sin(angle) * radius };

    Particle p;
    p.transform.translate = target + offset;
    p.velocity = Normalize(offset) * 0.5f;
    p.transform.scale = { 0.5f, 0.5f, 0.5f };
    p.color = { 1.0f, 0.8f, 0.2f, 1.0f };
    p.lifeTime = 2.0f;
    p.currentTime = 0.0f;
    return p;
}

Particle ParticleMotion::MakeExplosion(std::mt19937& rand, const Vector3& center)
{
    std::uniform_real_distribution<float> d(-1.0f, 1.0f);
    Vector3 dir = Normalize({ d(rand), d(rand), d(rand) });

    Particle p;
    p.transform.translate = center;
    p.velocity = dir * 0.25f;
    p.transform.scale = { 1.0f, 1.0f, 1.0f };
    p.color = { 1.0f, 0.3f, 0.3f, 1.0f };
    p.lifeTime = 1.0f;
    p.currentTime = 0.0f;
    return p;
}

Particle ParticleMotion::MakeFountain(std::mt19937& rand, const Vector3& base)
{
    std::uniform_real_distribution<float> h(-0.1f, 0.1f);
    std::uniform_real_distribution<float> v(0.25f, 0.4f);
    Vector3 velocity = { h(rand), v(rand), h(rand) };

    Particle p;
    p.transform.translate = base;
    p.velocity = velocity;
    p.transform.scale = { 0.3f, 0.3f, 0.3f };
    p.color = { 0.6f, 0.6f, 1.0f, 1.0f };
    p.lifeTime = 2.5f;
    p.currentTime = 0.0f;
    return p;
}

Particle ParticleMotion::MakeWiggle(std::mt19937& rand, const Vector3& origin)
{
    std::uniform_real_distribution<float> d(-0.05f, 0.05f);
    std::uniform_real_distribution<float> s(0.2f, 0.6f);

    Particle p;
    p.transform.translate = origin;
    p.velocity = { d(rand) * 3.0f, d(rand) + 0.02f + 0.06f, d(rand) * 3.0f };
    p.transform.scale = { s(rand), s(rand), s(rand) };
    p.color = { 1.0f, 1.0f, 1.0f, 1.0f };
    p.lifeTime = 4.0f;
    p.currentTime = 0.0f;
    return p;
}

Particle ParticleMotion::MakeCylinder(std::mt19937& rand, const Vector3& position)
{
    rand;

    Particle p;
    p.transform.scale = { 1.0f, 1.0f, 1.0f };
    p.transform.translate = position;
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
