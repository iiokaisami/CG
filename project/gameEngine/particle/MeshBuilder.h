#pragma once

#include <Model.h>

class MeshBuilder
{
public:

	// Ring作成(円状)
	static void BuildRing(Model* model);

	//　Cylinder作成(円柱状上下ふた無)
	static void BuildCylinder(Model* model);

	// Cone作成(円錐)
	static void BuildCone(Model* model);

	// Spiral作成(ねじれ線)
	static void BuildSpiral(Model* model);

	// Torus作成(ドーナツ)
	static void BuildTorus(Model* model);

	// Helix作成(ばね状)
	static void BuildHelix(Model* model);

	// 小さい球
	static void BuildSphere(Model* model);

	// 花びら
	static void BuildPetal(Model* model);

	// ペラペラな三角形
	static void BuildTriangle(Model* model);

	// 小さい立方体
	static void BuildCube(Model* model);

	// 線形状
	static void BuildLine(Model* model);

};

