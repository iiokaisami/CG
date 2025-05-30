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
	Register("Flame", MakeFlame);
	Register("Magic1", MakeMagic1);
	Register("Magic2", MakeMagic2);
	Register("Laser", MakeLaser);
	Register("Petal", MakePetal);
	Register("Water", MakeWater);
	Register("Bubble", MakeBubble);
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
    particle.transform.scale = { 0.5f, 0.5f, 0.5f };
    particle.velocity = Normalize(target - particle.transform.translate) * 0.5f;
    particle.lifeTime = 2.0f;
    particle.currentTime = 0.0f;
    particle.color = { 0.4f, 0.8f, 1.0f, 1.0f }; // 青白い
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
    particle.transform.scale = { 1.0f, 1.0f, 1.0f };
    particle.velocity = Normalize(Vector3(dist(rand), dist(rand), dist(rand))) * 0.2f;
    particle.lifeTime = 2.5f;
    particle.currentTime = 0.0f;
    particle.color = { 1.0f, 0.5f, 0.0f, 1.0f }; // オレンジ色（炎）
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

Particle ParticleMotion::MakeFlame(std::mt19937& rand, const Vector3& base)
{
    std::uniform_real_distribution<float> distXY(-0.2f, 0.2f);
    std::uniform_real_distribution<float> distScale(0.5f, 1.5f);
    std::uniform_real_distribution<float> distVelY(0.05f, 0.15f);
    std::uniform_real_distribution<float> distLife(0.8f, 1.5f);
    std::uniform_real_distribution<float> distColor(0.8f, 1.0f);

    Particle p;
    p.transform.translate = base + Vector3(distXY(rand), 0.0f, distXY(rand));
    p.transform.scale = Vector3(0.3f, distScale(rand), 0.3f);
    p.velocity = Vector3(distXY(rand), distVelY(rand), distXY(rand));
    p.color = Vector4(distColor(rand), distColor(rand) * 0.5f, 0.0f, 1.0f); // 赤〜オレンジ
    p.lifeTime = distLife(rand);
    p.currentTime = 0.0f;
    return p;
}

Particle ParticleMotion::MakeMagic1(std::mt19937& rand, const Vector3& translate)
{
    rand;
    Particle p;
    p.transform.translate = translate;
    p.transform.rotate = { 0.0f, 0.0f, 3.14f };
    p.transform.translate.y += p.transform.scale.y * 2.0f;
    p.transform.scale = { 1.0f, 1.0f, 1.0f };
    p.velocity = { 0.0f, 0.0f, 0.0f };
	p.angularVelocity = { 0.0f, 1.0f, 0.0f };
    p.color = { 0.6f, 0.3f, 1.0f, 1.0f };
    p.lifeTime = 100000.0f;
    p.currentTime = 0.0f;
    return p;
}

Particle ParticleMotion::MakeMagic2(std::mt19937& rand, const Vector3& translate)
{
    std::uniform_real_distribution<float> distXZ(-0.9f, 0.9f);
    std::uniform_real_distribution<float> distRot(-std::numbers::pi_v<float>, std::numbers::pi_v<float>);
    std::uniform_real_distribution<float> distScale(0.1f, 0.3f);

    Particle p;

    p.transform.translate = { distXZ(rand), translate.y, distXZ(rand) };

    p.transform.scale = { distScale(rand), distScale(rand), distScale(rand) };
    p.transform.rotate = { distRot(rand), distRot(rand), distRot(rand) }; // パラパラ感
    p.velocity = { 0.0f, 0.3f, 0.0f }; // 少しずつ上昇
    p.angularVelocity = { 0.0f, 3.0f, 0.0f }; // Y軸回転
    p.scaleVelocity = { -0.01f, -0.01f, -0.01f };
    p.color = { 0.6f, 0.3f, 1.0f, 1.0f };
    p.lifeTime = 2.5f;
    p.currentTime = 0.0f;

    return p;
}

Particle ParticleMotion::MakeLaser(std::mt19937& rand, const Vector3& translate)
{
    std::uniform_real_distribution<float> distWiggle(-0.05f, 0.05f);
    std::uniform_real_distribution<float> distLen(2.0f, 4.0f);

    Particle p;
    p.transform.translate = translate;
    p.transform.scale = { 0.8f, 0.0f, 0.8f };
    p.transform.rotate = { 0.0f, 0.0f, 0.0f };
    p.velocity = { distWiggle(rand), 0.0f, distWiggle(rand) };
	p.scaleVelocity = { -0.2f, 2.0f, -0.2f };
    p.color = { 1.0f, 0.2f, 0.2f, 1.0f };
    p.lifeTime = 4.0f;
    p.currentTime = 0.0f;
    return p;
}

Particle ParticleMotion::MakePetal(std::mt19937& rand, const Vector3& translate)
{
    translate;
    // XZ: -5～5, Y: 5
    std::uniform_real_distribution<float> distXZ(-3.0f, 3.0f);
    std::uniform_real_distribution<float> distRot(-std::numbers::pi_v<float>, std::numbers::pi_v<float>);
    std::uniform_real_distribution<float> distScale(0.2f, 0.5f);
    std::uniform_real_distribution<float> distColorR(0.8f, 1.0f); // ピンク系
    std::uniform_real_distribution<float> distColorG(0.4f, 0.7f);
    std::uniform_real_distribution<float> distColorB(0.7f, 0.9f);
    std::uniform_real_distribution<float> distAngVel(-1.0f, 1.0f);
    std::uniform_real_distribution<float> distWiggle(-0.05f, 0.05f);

    Particle p;
    p.transform.translate = Vector3(distXZ(rand), 4.5f, distXZ(rand));
    p.transform.scale = { distScale(rand), distScale(rand), distScale(rand) };
    p.transform.rotate = { distRot(rand), distRot(rand), distRot(rand) };
    // ひらひら感: XZに微小な揺れ、Yはゆっくり落下
    p.velocity = { distWiggle(rand), -0.35f, distWiggle(rand) };
    // 回転しながら落ちる
    p.angularVelocity = { distAngVel(rand), distAngVel(rand), distAngVel(rand) };
    // 花びららしい色
    p.color = { distColorR(rand), distColorG(rand), distColorB(rand), 1.0f };
    p.lifeTime = 6.0f;
    p.currentTime = 0.0f;
    return p;
}

Particle ParticleMotion::MakeWater(std::mt19937& rand, const Vector3& translate)
{
    // XZ方向ランダム、Yは上向き
    std::uniform_real_distribution<float> distXZ(-0.5f, 0.5f);
    std::uniform_real_distribution<float> distVelY(0.7f, 1.2f);
    std::uniform_real_distribution<float> distScale(0.15f, 0.25f);

    Particle p;
    p.transform.translate = translate;
    p.transform.scale = { distScale(rand), distScale(rand), distScale(rand) };
    p.transform.rotate = { 0.0f, 0.0f, 0.0f };
    // 初速：XZランダム、Y上向き
    p.velocity = { distXZ(rand), distVelY(rand), distXZ(rand) };
    p.angularVelocity = { 0.0f, 0.0f, 0.0f };
    p.color = { 0.3f, 0.5f, 1.0f, 1.0f }; // 水色
    p.lifeTime = 1.2f;
    p.currentTime = 0.0f;
    return p;
}

Particle ParticleMotion::MakeBubble(std::mt19937& rand, const Vector3& translate)
{
    // XZ: -1.0～1.0の範囲でランダムに出現
    std::uniform_real_distribution<float> distXZ(-1.0f, 1.0f);
    std::uniform_real_distribution<float> distScale(0.08f, 0.15f);
    std::uniform_real_distribution<float> distVelY(0.15f, 0.25f);
    std::uniform_real_distribution<float> distColor(0.7f, 1.0f);

    Particle p;
    p.transform.translate = translate + Vector3(distXZ(rand), 0.0f, distXZ(rand));
    p.transform.scale = { distScale(rand), distScale(rand), distScale(rand) };
    p.transform.rotate = { 0.0f, 0.0f, 0.0f };
    // 泡らしく上昇
    p.velocity = { 0.0f, distVelY(rand), 0.0f };
    p.angularVelocity = { 0.0f, 0.0f, 0.0f };
    p.color = { distColor(rand), distColor(rand), 1.0f, 0.7f }; // 透明感のある青
    p.lifeTime = 2.0f;
    p.currentTime = 0.0f;
    return p;
}

void ParticleMotion::SetDirection(const std::string& direction)
{
	direction_ = direction;
}
