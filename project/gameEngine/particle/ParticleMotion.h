#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include <random>
#include <MyMath.h>

#include "Particle.h"

class ParticleMotion
{
public:

    using MotionFunc = std::function<Particle(std::mt19937&, const Vector3&)>;

    // モーションの登録
    static void Register(const std::string& name, MotionFunc func);

    // モーションの生成（文字列で）
    static Particle Create(const std::string& name, std::mt19937& rand, const Vector3& pos);

    // 初期化（登録関数をまとめて呼び出す）
    static void Initialize();

    // 登録済み一覧取得（ImGui などUI表示用）
    static const std::unordered_map<std::string, MotionFunc>& GetAll();

    // 各モーション関数の定義
    static Particle MakeHoming(std::mt19937& rand, const Vector3& target);
    static Particle MakeOrbit(std::mt19937& rand, const Vector3& target);
    static Particle MakeExplosion(std::mt19937& rand, const Vector3& center);
    static Particle MakeFountain(std::mt19937& rand, const Vector3& base);
    static Particle MakeWiggle(std::mt19937& rand, const Vector3& origin);

    static Particle MakeCylinder(std::mt19937& rand, const Vector3& position);
    static Particle MakeSlash(std::mt19937& rand, const Vector3& translate);

public: // セッター

	// "UP"下から上
	// "DOWN"上から下
	// "LEFT"右から左
	// "RIGHT"左から右
    static void SetDirection(const std::string& direction);

private:

    static std::unordered_map<std::string, MotionFunc> motions_;
	static std::string direction_;

};

