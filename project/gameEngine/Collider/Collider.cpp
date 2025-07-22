#include "Collider.h"

#include<sstream>

#include"ColliderManager.h"

Collider::Collider()
{
}

Collider::~Collider()
{
}

const bool Collider::IsRegisteredCollidingPtr(const Collider* _ptr) const
{
    for (auto itr = collidingPtrList_.begin(); itr != collidingPtrList_.end(); ++itr)
    {
        if (_ptr == *itr) return true;
    }
    return false;
}

void Collider::EraseCollidingPtr(const Collider* _ptr)
{
    collidingPtrList_.remove_if([_ptr](const Collider* _pCollider) 
        {
        return _pCollider == _ptr;
        });
    return;
}

void Collider::SetAttribute(uint32_t _attribute)
{
    collisionAttribute_ = _attribute;
}

void Collider::SetMask(uint32_t* _mask)
{
    pCollisionMask_ = _mask;
}

void Collider::OnCollisionTrigger(const Collider* _other)
{
    if (onCollisionTriggerFunction_)
        onCollisionTriggerFunction_(_other);
    return;
}
