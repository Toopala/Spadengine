#pragma once

namespace sge
{ 
	static const unsigned int MAX_VERTEX_INPUTS = 15;

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

	enum class VertexFormat
	{
		FLOAT1,
		FLOAT2,
		FLOAT3,
		FLOAT4
	};
}