#include "GameObject.h"

#include <functional>
#include <sstream>

GameObject::GameObject()
{
    objectName_ = "unnamed-object";
    scale_ = { 1.0f, 1.0f, 1.0f };
    rotation_ = { 0.0f, 0.0f, 0.0f };
    position_ = { 0.0f, 0.0f, 0.0f };
    hp_ = 100.0f;
}

void GameObject::CorrectOverlap(const AABB& otherAABB, AABB& selfAABB, Vector3& position)
{
    Vector3 penetrationVector{};

    // x軸(左右)
    float overlapLeftX = otherAABB.max.x - selfAABB.min.x;
    float overlapRightX = selfAABB.max.x - otherAABB.min.x;
    float correctionX = (overlapLeftX < overlapRightX) ? overlapLeftX : -overlapRightX;

    // y軸(上下)
    float overlapBelowY = otherAABB.max.y - selfAABB.min.y;
    float overlapAboveY = selfAABB.max.y - otherAABB.min.y;
    float correctionY = (overlapBelowY < overlapAboveY) ? overlapBelowY : -overlapAboveY;

    // z軸(前後)
    float overlapBackZ = otherAABB.max.z - selfAABB.min.z;
    float overlapFrontZ = selfAABB.max.z - otherAABB.min.z;
    float correctionZ = (overlapBackZ < overlapFrontZ) ? overlapBackZ : -overlapFrontZ;

    // 最小の重なりを持つ軸を選択
    float absX = std::abs(correctionX);
    float absY = std::abs(correctionY);
    float absZ = std::abs(correctionZ);

    if (absX <= absY && absX <= absZ)
        penetrationVector.x = correctionX;
    else if (absY <= absZ)
        penetrationVector.y = correctionY;
    else
        penetrationVector.z = correctionZ;

    // 位置を修正
    position += penetrationVector;
}

bool GameObject::IsAABBOverlap(const AABB& a, const AABB& b)
{
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
        (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
        (a.min.z <= b.max.z && a.max.z >= b.min.z);
}