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
out gl_PerVertex {
vec4 gl_Position;
float gl_PointSize;
float gl_ClipDistance[];};
layout(location = 0) uniform struct UniformDataVS_Type {
	mat4 MVP;
} UniformDataVS;
layout(location = 0)  in  vec4 dcl_Input0;
vec4 Input0;
layout(location = 1)  in  vec4 dcl_Input1;
vec4 Input1;
#undef Output0
#define Output0 phase0_Output0
vec4 phase0_Output0;
layout(location = 1)  out  vec4 VtxGeoOutput1;
#define Output1 VtxGeoOutput1
vec4 Temp[1];
ivec4 Temp_int[1];
uvec4 Temp_uint[1];
void main()
{
    Input0 = dcl_Input0;
    Input1 = dcl_Input1;
    Temp[0] = Input0.yyyy * UniformDataVS.MVP[1];
    Temp[0] = UniformDataVS.MVP[0] * Input0.xxxx + Temp[0];
    Temp[0] = UniformDataVS.MVP[2] * Input0.zzzz + Temp[0];
    Output0 = UniformDataVS.MVP[3] * Input0.wwww + Temp[0];
    Output1.xy = Input1.xy;
    gl_Position = vec4(phase0_Output0);
    return;
}
