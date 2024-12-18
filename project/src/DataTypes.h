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
		Vector2 uv		{ };
		Vector3 normal	{ };
		Vector3 tangent	{ };
	};

	struct Vertex_Out
	{
		Vector4 position{ };
		Vector2 uv		{ };
		Vector3 normal	{ };
		Vector3 tangent	{ };
	};
}