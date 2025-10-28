#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include <random>
#include <MyMath.h>

#include "Particle.h"

/// <summary>
/// パーティクルのモーション管理
/// 登録、生成、一覧取得など
/// </summary>
class ParticleMotion
{
public:

    using MotionFunc = std::function<Particle(std::mt19937&, const Vector3&)>;

    /// <summary>
	/// モーションの登録
    /// </summary>
	/// <param name="name">モーション名</param>
	/// <param name="func">モーション生成関数</param>
    static void Register(const std::string& name, MotionFunc func);

	/// <summary>
	/// パーティクルの生成
	/// </summary>
	/// <param name="name">モーション名</param>
	/// <param name="rand">乱数生成器</param>
	/// <param name="pos">生成位置</param>
	/// <returns>生成したパーティクル</returns>
    static Particle Create(const std::string& name, std::mt19937& rand, const Vector3& pos);

    /// <summary>
	/// 初期化(モーション登録)
    /// </summary>
    static void Initialize();

    /// <summary>
    /// 登録済み一覧取得(ImGui などUI表示用)
    /// </summary>
	/// <returns>モーション関数のマップ</returns>
    static const std::unordered_map<std::string, MotionFunc>& GetAll();

    // 各モーション関数の定義
 
    /// <summary>
	/// ターゲットに向かって飛ぶ
    /// </summary>
	/// <param name="rand">ランダム生成器</param>
	/// <param name="target">目標位置</param>
	/// <returns>生成したパーティクル</returns>
    static Particle MakeHoming(std::mt19937& rand, const Vector3& target);
    /// <summary>
	/// ターゲットの周りを回る
    /// </summary>
	/// <param name="rand">ランダム生成器</param>
    /// <param name="target"></param>
    /// <returns></returns>
    static Particle MakeOrbit(std::mt19937& rand, const Vector3& target);
	// 中心からランダムに放射状に飛ぶ
    static Particle MakeExplosion(std::mt19937& rand, const Vector3& center);
	// baseから上に向かって噴出
    static Particle MakeFountain(std::mt19937& rand, const Vector3& base);
	// originを中心にランダムに揺れる
    static Particle MakeWiggle(std::mt19937& rand, const Vector3& origin);
	// 円柱状に放射
    static Particle MakeCylinder(std::mt19937& rand, const Vector3& position);
	// 斜めにスラッシュ
    static Particle MakeSlash(std::mt19937& rand, const Vector3& translate);
	// 炎のように上昇
    static Particle MakeFlame(std::mt19937& rand, const Vector3& base);

    // 魔法陣
    static Particle MakeMagic1(std::mt19937& rand, const Vector3& translate);
	static Particle MakeMagic2(std::mt19937& rand, const Vector3& translate);
	static Particle MakeLaser(std::mt19937& rand, const Vector3& translate);

	// 花びら
	static Particle MakePetal(std::mt19937& rand, const Vector3& translate);

    // 水
	static Particle MakeWater(std::mt19937& rand, const Vector3& translate);

    // 泡
	static Particle MakeBubble(std::mt19937& rand, const Vector3& translate);

    // 土埃
	static Particle MakeDust(std::mt19937& rand, const Vector3& translate);

    // デバフ
	static Particle MakeDebuff(std::mt19937& rand, const Vector3& translate);

    // スパーク
	static Particle MakeSpark(std::mt19937& rand, const Vector3& translate);

    // 弾けるスパーク
	static Particle MakeSparkBurst(std::mt19937& rand, const Vector3& translate);
 
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

