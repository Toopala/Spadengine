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
layout(location = 0) uniform sampler2D tex;
layout(location = 1) uniform sampler2D tex2;
layout(location = 1)  in  vec4 VtxGeoOutput1;
vec4 Input1;
layout(location = 2)  in  vec4 VtxGeoOutput2;
vec4 Input2;
layout(location = 4)  in  vec4 VtxGeoOutput4;
vec4 Input4;
layout(location = 0) out  vec4 PixOutput0;
#define Output0 PixOutput0
vec4 Temp[2];
ivec4 Temp_int[2];
uvec4 Temp_uint[2];
void main()
{
    Input1 = VtxGeoOutput1;
    Input2 = VtxGeoOutput2;
    Input4 = VtxGeoOutput4;
    Temp[0].xyz = texture(tex2, Input4.xy).xyz;
    Temp[0].xyz = Temp[0].xyz * vec3(intBitsToFloat(int(0x40000000u)), intBitsToFloat(int(0x40000000u)), intBitsToFloat(int(0x40000000u))) + vec3(intBitsToFloat(int(0xBF800000u)), intBitsToFloat(int(0xBF800000u)), intBitsToFloat(int(0xBF800000u)));
    Temp[0].w = dot(Temp[0].xyz, Temp[0].xyz);
    Temp[0].w = inversesqrt(Temp[0].w);
    Temp[0].xyz = Temp[0].www * Temp[0].xyz;
    Temp[1].xyz = (-Input1.xyz) + Input2.xyz;
    Temp[0].w = dot(Temp[1].xyz, Temp[1].xyz);
    Temp[1].w = inversesqrt(Temp[0].w);
    Temp[0].w = sqrt(Temp[0].w);
    Temp[1].xyz = Temp[1].www * Temp[1].xyz;
    Temp[0].x = dot(Temp[0].xyz, Temp[1].xyz);
    Temp[0].x = max(Temp[0].x, intBitsToFloat(0x0));
    Temp[1].xyz = texture(tex, Input4.xy).xyz;
    Temp[0].xyz = Temp[0].xxx * Temp[1].xyz;
    Temp[1].xyz = Temp[1].xyz * vec3(intBitsToFloat(0x3E4CCCCD), intBitsToFloat(0x3E4CCCCD), intBitsToFloat(0x3E4CCCCD));
    Temp[1].w = Temp[0].w * intBitsToFloat(0x3716FEB5) + intBitsToFloat(0x3F800000);
    Temp[0].w = Temp[0].w * intBitsToFloat(0x3B51B717) + Temp[1].w;
    Temp[0].w = float(intBitsToFloat(0x3F800000)) / Temp[0].w;
    Temp[1].xyz = Temp[0].www * Temp[1].xyz;
    Output0.xyz = Temp[0].xyz * Temp[0].www + Temp[1].xyz;
    Output0.w = intBitsToFloat(0x3F800000);
    return;
}
