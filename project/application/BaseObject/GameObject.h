#pragma once

#include <Vector3.h>
#include <string>

#include "../../gameEngine/Collider/Shape.h"

class GameObject
{
public:
    GameObject();
    virtual ~GameObject() {};

    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void Finalize() = 0;

public: // ゲッター

    virtual Vector3 GetScale() const { return scale_; };
    virtual Vector3 GetRotation() const { return rotation_; };
    virtual Vector3 GetPosition() const { return position_; };
    virtual float   GetHP() const { return hp_; };
	virtual bool IsDead() const { return isDead_; }
	virtual bool IsActive() const { return isActive_; }

public: // セッター

    virtual void SetScale(const Vector3& _scale) { scale_ = _scale; };
    virtual void SetRotation(const Vector3& _rotation) { rotation_ = _rotation; };
    virtual void SetPosition(const Vector3& _position) { position_ = _position; };
    virtual void SetHP(float _hp) { hp_ = _hp; };
	virtual void SetIsDead(bool _isDead) { isDead_ = _isDead; };
	virtual void SetIsActive(bool _isActive) { isActive_ = _isActive; };


protected: // メンバー

    // 衝突時の押し出し処理
    void CorrectOverlap(const AABB& otherAABB, AABB& selfAABB, Vector3& position);

	// AABBの重なり判定
    bool IsAABBOverlap(const AABB& a, const AABB& b);

    std::string objectName_;
    Vector3 scale_;
    Vector3 rotation_;
    Vector3 position_;
    float hp_;
    // デスフラグ
	bool isDead_ = false; 
    // オブジェクトがアクティブかどうか(回避中など判定を付けたくない場合等)
	bool isActive_ = true;
};