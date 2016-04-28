#version 440
subroutine void SubroutineType();
struct vec1 {
	float x;
};
struct uvec1 {
	uint x;
};
struct ivec1 {
	int x;
};
layout(location = 1) uniform struct UniformDataPS_Type {
	vec4 color;
} UniformDataPS;
layout(location = 1) uniform sampler2D tex;
layout(location = 1)  in  vec4 VtxGeoOutput1;
vec4 Input1;
layout(location = 0) out  vec4 PixOutput0;
#define Output0 PixOutput0
vec4 Temp[1];
ivec4 Temp_int[1];
uvec4 Temp_uint[1];
void main()
{
    Input1 = VtxGeoOutput1;
    Temp[0] = texture(tex, Input1.xy);
    Output0 = Temp[0] * UniformDataPS.color;
    return;
}
