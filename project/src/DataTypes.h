#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "ColorRGB.h"

namespace dae
{
	struct Vertex
	{
		Vector3 position{ };
		ColorRGB color{ colors::White };
		Vector2 uv{ };
	};

	struct Vertex_Out
	{
		Vector4 position{ };
		ColorRGB color{ };
		Vector2 uv{ };
	};
}