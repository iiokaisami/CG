#pragma once

#include "../../baseObject/GameObject.h"
#include"../../../gameEngine/collider/ColliderManager.h"

#include <Object3d.h>
#include <memory>
#include <Framework.h>

/// <summary>
/// フィールド
/// </summary>
class Field : public GameObject
{
public:
	
	Field() = default;
	~Field() = default;
	
	// 初期化
	void Initialize() override;
	
	// 終了
	void Finalize() override;
	
	// 更新
	void Update() override;
	
	// 描画
	void Draw() override;

	// ImGui
	void ImGuiDraw();

private: // 衝突判定

	void OnCollisionTrigger();

private: // メンバ変数

	// 3Dオブジェクト
	std::unique_ptr<Object3d> object_ = nullptr;
	
	// 当たり判定関係
	ColliderManager* colliderManager_ = nullptr;
	Collider collider_;
	AABB aabb_;
	Collider::ColliderDesc desc = {};

};