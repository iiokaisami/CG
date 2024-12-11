#pragma once

#include "../GameObject/GameObject.h"
#include <Object3d.h>
#include <memory>

class Skydome : public GameObject
{
public:
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;

private:
    std::unique_ptr<Object3d> object_ = nullptr;
};