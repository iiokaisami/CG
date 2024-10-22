#include "Sprite.h"

#include "SpriteCommon.h"

void Sprite::Initialize(SpriteCommon* spriteCommon)
{
	spriteCommon_ = spriteCommon;

	vertexResource_ = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * 4);

	VertexData* vertexData_ = nullptr;
	vertexResource_->Map(
		0,
		nullptr,
		reinterpret_cast<void**>(&vertexData_)
	);
}
