#pragma once

#include "../../BaseObject/GameObject.h"
#include "Bullet/EnemyBullet.h"
#include "BehaviorState/NormalEmemyState/EnemyBehaviorState.h"
#include"../../Collider/ColliderManager.h"
#include "../../../gameEngine/particle/ParticleEmitter.h"

#include <Object3d.h>
#include <Sprite.h>
#include <Framework.h>

class TrapEnemy : public GameObject
{
public:

	TrapEnemy() = default;
	~TrapEnemy() = default;
	
	// 初期化
	void Initialize() override;
	
	// 終了
	void Finalize() override;
	
	// 更新
	void Update() override;
	
	// 描画
	void Draw() override;
	
	// スプライト描画
	void Draw2D();
	
	// ImGui
	void ImGuiDraw();

	// 移動
	void Move();

	// 罠設置
	void SetTrap();



};

