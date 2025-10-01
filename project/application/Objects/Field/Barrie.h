#pragma once

#include "../../baseObject/GameObject.h"
#include "../../../gameEngine/collider/ColliderManager.h"
#include "../../../gameEngine/particle/ParticleEmitter.h"

#include <Object3d.h>
#include <memory>
#include <Framework.h>

class Barrie : public GameObject
{
public:

	Barrie() = default;
	~Barrie() = default;

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;


public: // セッター

	// バリア破壊フラグをセット
	void SetBarrierDestroyed(bool _isDestroyed) { isBarrierDestroyed_ = _isDestroyed; }

private: // 衝突判定

	void OnCollisionTrigger(const Collider* _other);

private:

	// 3Dオブジェクト
	std::unique_ptr<Object3d> object_ = nullptr;

	// 当たり判定関係
	ColliderManager* colliderManager_ = nullptr;
	Collider collider_;
	AABB aabb_;

	// バリア破壊フラグ
	bool isBarrierDestroyed_ = false;

	// 元のスケール
	Vector3 defaultScale_ = { 1.5f, 1.5f, 1.5f };
	// 目標スケール
	Vector3 targetScale_ = { 1.5f, 1.5f, 1.5f };  
	float scaleLerpSpeed_ = 0.2f;

	// 爆発演出用カウント
	uint32_t explodeCount_ = 0;      
	// 爆発演出の継続フレーム数
	const uint32_t explodeMaxCount_ = 10; 
	bool isExploding_ = false;

};