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
	mat4 PV;
	mat4 M;
} UniformDataVS;
layout(location = 0)  in  vec4 dcl_Input0;
vec4 Input0;
layout(location = 1)  in  vec4 dcl_Input1;
vec4 Input1;
layout(location = 2)  in  vec4 dcl_Input2;
vec4 Input2;
layout(location = 3)  in  vec4 dcl_Input3;
vec4 Input3;
layout(location = 4)  in  vec4 dcl_Input4;
vec4 Input4;
#undef Output0
#define Output0 phase0_Output0
vec4 phase0_Output0;
layout(location = 1)  out  vec4 VtxGeoOutput1;
#define Output1 VtxGeoOutput1
layout(location = 2)  out  vec4 VtxGeoOutput2;
#define Output2 VtxGeoOutput2
layout(location = 3)  out  vec4 VtxGeoOutput3;
#define Output3 VtxGeoOutput3
layout(location = 4)  out  vec4 VtxGeoOutput4;
#define Output4 VtxGeoOutput4
layout(location = 5)  out  vec4 VtxGeoOutput5;
#define Output5 VtxGeoOutput5
layout(location = 6)  out  vec4 VtxGeoOutput6;
#define Output6 VtxGeoOutput6
vec4 Temp[2];
ivec4 Temp_int[2];
uvec4 Temp_uint[2];
void main()
{
    Input0 = dcl_Input0;
    Input1 = dcl_Input1;
    Input2 = dcl_Input2;
    Input3 = dcl_Input3;
    Input4 = dcl_Input4;
    Temp[0] = Input0.yyyy * UniformDataVS.M[1];
    Temp[0] = UniformDataVS.M[0] * Input0.xxxx + Temp[0];
    Temp[0] = UniformDataVS.M[2] * Input0.zzzz + Temp[0];
    Temp[0] = UniformDataVS.M[3] * Input0.wwww + Temp[0];
    Temp[1] = Temp[0].yyyy * UniformDataVS.PV[1];
    Temp[1] = UniformDataVS.PV[0] * Temp[0].xxxx + Temp[1];
    Temp[1] = UniformDataVS.PV[2] * Temp[0].zzzz + Temp[1];
    Output0 = UniformDataVS.PV[3] * Temp[0].wwww + Temp[1];
    Output1.xyz = Temp[0].xyz;
    Output2.xyz = Input1.xyz;
    Output3.xy = Input4.xy;
    Temp[0].x = dot(UniformDataVS.M[0].xyz, Input2.xyz);
    Temp[0].y = dot(UniformDataVS.M[1].xyz, Input2.xyz);
    Temp[0].z = dot(UniformDataVS.M[2].xyz, Input2.xyz);
    Temp[0].w = dot(Temp[0].xyz, Temp[0].xyz);
    Temp[0].w = inversesqrt(Temp[0].w);
    Output4.xyz = Temp[0].www * Temp[0].xyz;
    Temp[0].x = dot(UniformDataVS.M[0].xyz, Input3.xyz);
    Temp[0].y = dot(UniformDataVS.M[1].xyz, Input3.xyz);
    Temp[0].z = dot(UniformDataVS.M[2].xyz, Input3.xyz);
    Temp[0].w = dot(Temp[0].xyz, Temp[0].xyz);
    Temp[0].w = inversesqrt(Temp[0].w);
    Output5.xyz = Temp[0].www * Temp[0].xyz;
    Temp[0].x = dot(UniformDataVS.M[0].xyz, Input1.xyz);
    Temp[0].y = dot(UniformDataVS.M[1].xyz, Input1.xyz);
    Temp[0].z = dot(UniformDataVS.M[2].xyz, Input1.xyz);
    Temp[0].w = dot(Temp[0].xyz, Temp[0].xyz);
    Temp[0].w = inversesqrt(Temp[0].w);
    Output6.xyz = Temp[0].www * Temp[0].xyz;
    gl_Position = vec4(phase0_Output0);
    return;
}
