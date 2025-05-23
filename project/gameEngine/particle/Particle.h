#pragma once
#include "MyMath.h"

class Particle
{
public:
    struct Transform
    {
        Vector3 scale;
        Vector3 rotate;
        Vector3 translate;
    };

    // パーティクルの状態
    Transform transform;
    Vector3 velocity;
    Vector4 color;
    float lifeTime;
    float currentTime;

    Particle()
		: velocity(0.0f, 0.0f, 0.0f), color(1.0f, 1.0f, 1.0f, 1.0f), lifeTime(1.0f), currentTime(0.0f)
    {
        transform.scale = Vector3(1.0f, 1.0f, 1.0f);
        transform.rotate = Vector3(0.0f, 0.0f, 0.0f);
        transform.translate = Vector3(0.0f, 0.0f, 0.0f);
    }
};
