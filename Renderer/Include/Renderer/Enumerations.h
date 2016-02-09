#pragma once

namespace sge
{ 
	enum class VertexSemantic
	{
		BINORMAL,
		COLOR,
		NORMAL,
		POSITION,
		TANGENT,
		TEXCOORD
	};

	enum class ShaderType
	{
		VERTEX,
		PIXEL
	};

	enum class BufferType
	{
		VERTEX,
		INDEX,
		UNIFORM
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