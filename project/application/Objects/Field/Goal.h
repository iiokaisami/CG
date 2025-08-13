#pragma once

#include "../../BaseObject/GameObject.h"
#include"../../../gameEngine/Collider/ColliderManager.h"

#include <Object3d.h>
#include <memory>
#include <Framework.h>

class Goal : public GameObject
{
public:

	Goal() = default;
	~Goal() = default;

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

public: // セッター

	// ゴール出現フラグをセット
	void SetGoalVisible(bool _isVisible) { isGoalVisible_ = _isVisible; }
	
public: // ゲッター

	// クリアフラグを取得
	bool IsCleared() const { return isCleared_; }


private: // 衝突判定

	void OnCollisionTrigger(const Collider* _other);

private:

	// 3Dオブジェクト
	std::unique_ptr<Object3d> object_ = nullptr;

	// 当たり判定関係
	ColliderManager* colliderManager_ = nullptr;
	Collider collider_;
	AABB aabb_;

	// クリアフラグ
	bool isCleared_ = false;

	// ゴール出現フラグ
	bool isGoalVisible_ = false;

};