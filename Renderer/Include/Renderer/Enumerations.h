#pragma once

namespace sge
{
	enum class ShaderType
	{
		VERTEX,
		PIXEL
	};

	enum class BufferType
	{
		VERTEX,
		INDEX
	};

	enum class BufferUsage
	{
		STATIC,
		DYNAMIC
	};

	enum class PrimitiveTopology
	{
		TRIANGLE
	};
}