#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "ColorRGB.h"

namespace dae
{
	struct Vertex
	{
		Vector3 position{ };
		ColorRGB color{ };
	};

	struct Vertex_Out
	{
		Vector4 position{ };
		ColorRGB color{ };
	};
}