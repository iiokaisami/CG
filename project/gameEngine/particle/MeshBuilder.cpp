#include "MeshBuilder.h"

#include <numbers>

void MeshBuilder::BuildRing(Model* model)
{
    model->ClearVertexData();

    const uint32_t kRingDivide = 32;
    const float kOuterRadius = 1.0f;
    const float kInnerRadius = 0.2f;
    const float radianPerDivide = 2.0f * std::numbers::pi_v<float> / float(kRingDivide);

    for (uint32_t index = 0; index < kRingDivide; ++index)
    {
        float sin = std::sin(index * radianPerDivide);
        float cos = std::cos(index * radianPerDivide);
        float sinNext = std::sin((index + 1) * radianPerDivide);
        float cosNext = std::cos((index + 1) * radianPerDivide);
        float u = float(index) / float(kRingDivide);
        float uNext = float(index + 1) / float(kRingDivide);

        uint32_t baseIndex = model->GetVertexCount();

        model->AddVertex({ -sin * kOuterRadius, cos * kOuterRadius, 0.0f, 1.0f }, { u, 0.0f }, { 0.0f, 0.0f, 1.0f });
        model->AddVertex({ -sin * kInnerRadius, cos * kInnerRadius, 0.0f, 1.0f }, { u, 1.0f }, { 0.0f, 0.0f, 1.0f });
        model->AddVertex({ -sinNext * kOuterRadius, cosNext * kOuterRadius, 0.0f, 1.0f }, { uNext, 0.0f }, { 0.0f, 0.0f, 1.0f });

        model->AddIndex(baseIndex + 0);
        model->AddIndex(baseIndex + 1);
        model->AddIndex(baseIndex + 2);

        model->AddVertex({ -sinNext * kOuterRadius, cosNext * kOuterRadius, 0.0f, 1.0f }, { uNext, 0.0f }, { 0.0f, 0.0f, 1.0f });
        model->AddVertex({ -sin * kInnerRadius, cos * kInnerRadius, 0.0f, 1.0f }, { u, 1.0f }, { 0.0f, 0.0f, 1.0f });
        model->AddVertex({ -sinNext * kInnerRadius, cosNext * kInnerRadius, 0.0f, 1.0f }, { uNext, 1.0f }, { 0.0f, 0.0f, 1.0f });

        model->AddIndex(baseIndex + 3);
        model->AddIndex(baseIndex + 4);
        model->AddIndex(baseIndex + 5);
    }

    model->UpdateVertexBuffer();
    model->UpdateIndexBuffer();
}

void MeshBuilder::BuildCylinder(Model* model)
{
    model->ClearVertexData();

    const uint32_t kDivide = 32;
    const float kRadius = 1.0f;
    const float kHeight = 3.0f;
    const float radianPerDivide = 2.0f * std::numbers::pi_v<float> / float(kDivide);

    for (uint32_t i = 0; i < kDivide; ++i)
    {
        float sin = std::sin(i * radianPerDivide);
        float cos = std::cos(i * radianPerDivide);
        float sinNext = std::sin((i + 1) * radianPerDivide);
        float cosNext = std::cos((i + 1) * radianPerDivide);
        float u = float(i) / float(kDivide);
        float uNext = float(i + 1) / float(kDivide);

        Vector3 top1 = { -sin * kRadius, kHeight, cos * kRadius };
        Vector3 top2 = { -sinNext * kRadius, kHeight, cosNext * kRadius };
        Vector3 bottom1 = { -sin * kRadius, 0.0f, cos * kRadius };
        Vector3 bottom2 = { -sinNext * kRadius, 0.0f, cosNext * kRadius };

        uint32_t baseIndex = model->GetVertexCount();

        model->AddVertex({ top1.x, top1.y, top1.z, 1.0f }, { u, 0.0f }, { -sin, 0.0f, cos });
        model->AddVertex({ top2.x, top2.y, top2.z, 1.0f }, { uNext, 0.0f }, { -sinNext, 0.0f, cosNext });
        model->AddVertex({ bottom1.x, bottom1.y, bottom1.z, 1.0f }, { u, 1.0f }, { -sin, 0.0f, cos });

        model->AddVertex({ bottom1.x, bottom1.y, bottom1.z, 1.0f }, { u, 1.0f }, { -sin, 0.0f, cos });
        model->AddVertex({ top2.x, top2.y, top2.z, 1.0f }, { uNext, 0.0f }, { -sinNext, 0.0f, cosNext });
        model->AddVertex({ bottom2.x, bottom2.y, bottom2.z, 1.0f }, { uNext, 1.0f }, { -sinNext, 0.0f, cosNext });

        model->AddIndex(baseIndex + 0);
        model->AddIndex(baseIndex + 1);
        model->AddIndex(baseIndex + 2);
        model->AddIndex(baseIndex + 3);
        model->AddIndex(baseIndex + 4);
        model->AddIndex(baseIndex + 5);
    }

    model->UpdateVertexBuffer();
    model->UpdateIndexBuffer();
}

void MeshBuilder::BuildCone(Model* model)
{
    model->ClearVertexData();

    const uint32_t kDivide = 32;
    const float kRadius = 1.0f;
    const float kHeight = 3.0f;
    const float radianPerDivide = 2.0f * std::numbers::pi_v<float> / float(kDivide);

    Vector3 apex = { 0.0f, kHeight, 0.0f };
    uint32_t apexIndex = model->GetVertexCount();
    model->AddVertex({ apex.x, apex.y, apex.z, 1.0f }, { 0.5f, 0.0f }, { 0, 1, 0 });

    for (uint32_t i = 0; i < kDivide; ++i)
    {
        float sin = std::sin(i * radianPerDivide);
        float cos = std::cos(i * radianPerDivide);
        float sinNext = std::sin((i + 1) * radianPerDivide);
        float cosNext = std::cos((i + 1) * radianPerDivide);

        Vector3 base1 = { -sin * kRadius, 0.0f, cos * kRadius };
        Vector3 base2 = { -sinNext * kRadius, 0.0f, cosNext * kRadius };

        uint32_t base = model->GetVertexCount();
        model->AddVertex({ base1.x, base1.y, base1.z, 1.0f }, { 0.0f, 1.0f }, { -sin, 0, cos });
        model->AddVertex({ base2.x, base2.y, base2.z, 1.0f }, { 1.0f, 1.0f }, { -sinNext, 0, cosNext });

        model->AddIndex(apexIndex);
        model->AddIndex(base);
        model->AddIndex(base + 1);
    }

    model->UpdateVertexBuffer();
    model->UpdateIndexBuffer();
}

void MeshBuilder::BuildSpiral(Model* model)
{
    model->ClearVertexData();

    const uint32_t kTurns = 5;
    const uint32_t kSegmentsPerTurn = 32;
    const float kRadius = 1.0f;
    const float kHeight = 5.0f;

    const uint32_t kTotal = kTurns * kSegmentsPerTurn;

    for (uint32_t i = 0; i < kTotal; ++i)
    {
        float t = float(i) / float(kTotal);
        float angle = t * kTurns * 2.0f * std::numbers::pi_v<float>;

        float x = std::cos(angle) * kRadius;
        float y = t * kHeight;
        float z = std::sin(angle) * kRadius;

        float u = float(i) / float(kTotal);
        model->AddVertex({ x, y, z, 1.0f }, { u, 0.0f }, { 0.0f, 1.0f, 0.0f });

        if (i >= 1) {
            model->AddIndex(i - 1);
            model->AddIndex(i);
        }
    }

    model->UpdateVertexBuffer();
    model->UpdateIndexBuffer();
}

void MeshBuilder::BuildTorus(Model* model)
{
    model->ClearVertexData();

    const uint32_t kTubeDiv = 16;
    const uint32_t kCircleDiv = 32;
    const float kOuterR = 1.5f;
    const float kInnerR = 0.4f;

    for (uint32_t i = 0; i < kCircleDiv; ++i)
    {
        float theta = 2.0f * std::numbers::pi_v<float> *float(i) / float(kCircleDiv);
        float nextTheta = 2.0f * std::numbers::pi_v<float> *float(i + 1) / float(kCircleDiv);
        for (uint32_t j = 0; j < kTubeDiv; ++j)
        {
            float phi = 2.0f * std::numbers::pi_v<float> *float(j) / float(kTubeDiv);
            float nextPhi = 2.0f * std::numbers::pi_v<float> *float(j + 1) / float(kTubeDiv);

            // 4つの点を使って面を作る
            auto point = [&](float t, float p) -> Vector3 {
                float x = (kOuterR + kInnerR * std::cos(p)) * std::cos(t);
                float y = kInnerR * std::sin(p);
                float z = (kOuterR + kInnerR * std::cos(p)) * std::sin(t);
                return { x, y, z };
                };

            Vector3 v0 = point(theta, phi);
            Vector3 v1 = point(nextTheta, phi);
            Vector3 v2 = point(theta, nextPhi);
            Vector3 v3 = point(nextTheta, nextPhi);

            uint32_t base = model->GetVertexCount();
            model->AddVertex({ v0.x, v0.y, v0.z, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
            model->AddVertex({ v1.x, v1.y, v1.z, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
            model->AddVertex({ v2.x, v2.y, v2.z, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
            model->AddVertex({ v3.x, v3.y, v3.z, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });

            model->AddIndex(base + 0);
            model->AddIndex(base + 1);
            model->AddIndex(base + 2);
            model->AddIndex(base + 2);
            model->AddIndex(base + 1);
            model->AddIndex(base + 3);
        }
    }

    model->UpdateVertexBuffer();
    model->UpdateIndexBuffer();
}

void MeshBuilder::BuildHelix(Model* model)
{
    model->ClearVertexData();

    const uint32_t kSegments = 100;
    const float kRadius = 1.0f;
    const float kHeight = 5.0f;
    const uint32_t kTurns = 5;

    for (uint32_t i = 0; i < kSegments; ++i)
    {
        float t = float(i) / float(kSegments);
        float angle = t * kTurns * 2.0f * std::numbers::pi_v<float>;

        float x = std::cos(angle) * kRadius;
        float y = t * kHeight;
        float z = std::sin(angle) * kRadius;

        model->AddVertex({ x, y, z, 1.0f }, { t, 0.0f }, { 0.0f, 1.0f, 0.0f });

        if (i >= 1)
        {
            model->AddIndex(i - 1);
            model->AddIndex(i);
        }
    }

    model->UpdateVertexBuffer();
    model->UpdateIndexBuffer();
}
