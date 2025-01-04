#pragma once

#include "../GameObject/GameObject.h"
#include "../math/MyMath.h"

#include <Object3d.h>
#include <Sprite.h>

class Enemy : public GameObject
{
public:

	Enemy() = default;
	~Enemy() = default;

    // 初期化
    void Initialize() override;

    // 終了
    void Finalize() override;

    // 更新処理
    void Update() override;

    // 描画処理
    void Draw() override;

    void Draw2d();

    // 攻撃
    void Attack();

public:

    void SetPlayerPosition(Vector3 _playerPosition) { playerPosition_ = _playerPosition; }

private:

    // 3Dオブジェクト
    std::unique_ptr<Object3d> object_ = nullptr;

    // 移動速度
    Vector3 moveVelocity_{};
    float moveSpeed_ = 0.05f;
    
    Vector3 playerPosition_{};

};

