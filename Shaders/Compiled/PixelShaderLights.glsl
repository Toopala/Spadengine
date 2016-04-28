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
layout(location = 0) uniform struct GlobalsPS_Type {
	float shininess;
} GlobalsPS;
struct dirLight_Type {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct pointLights_Type {
	vec3 position;
	float constant;
	float mylinear;
	float quadratic;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
layout(location = 1) uniform struct UniformDataPS_Type {
	int numberOfLights;
	dirLight_Type dirLight;
	pointLights_Type pointLights;
	vec3 viewPos;
} UniformDataPS;
layout(location = 2) uniform sampler2D diffuseTex;
layout(location = 3) uniform sampler2D specularTex;
layout(location = 1)  in  vec4 VtxGeoOutput1;
vec4 Input1;
layout(location = 2)  in  vec4 VtxGeoOutput2;
vec4 Input2;
layout(location = 3)  in  vec4 VtxGeoOutput3;
vec4 Input3;
layout(location = 0) out  vec4 PixOutput0;
#define Output0 PixOutput0
vec4 Temp[10];
ivec4 Temp_int[10];
uvec4 Temp_uint[10];
vec4 TempArray0[280];
ivec4 TempArray0_int[280];
uvec4 TempArray0_uint[280];
void main()
{
    Input1 = VtxGeoOutput1;
    Input2 = VtxGeoOutput2;
    Input3 = VtxGeoOutput3;
    TempArray0[0].xyz = vec3(intBitsToFloat(0x3F800000), intBitsToFloat(int(0x40000000u)), intBitsToFloat(0x0));
    TempArray0[1].x = intBitsToFloat(0x3F800000);
    TempArray0[2].x = intBitsToFloat(0x3DB851EC);
    TempArray0[3].x = intBitsToFloat(0x3D03126F);
    TempArray0[4].xyz = vec3(intBitsToFloat(0x3D4CCCCD), intBitsToFloat(0x3D4CCCCD), intBitsToFloat(0x3D4CCCCD));
    TempArray0[5].xyz = vec3(intBitsToFloat(0x3F4CCCCD), intBitsToFloat(0x3F4CCCCD), intBitsToFloat(0x3F4CCCCD));
    TempArray0[6].xyz = vec3(intBitsToFloat(0x3F800000), intBitsToFloat(0x3F800000), intBitsToFloat(0x3F800000));
    Temp[0].x = dot(Input2.xyz, Input2.xyz);
    Temp[0].x = inversesqrt(Temp[0].x);
    Temp[0].xyz = Temp[0].xxx * Input2.xyz;
    Temp[1].xyz = (-Input1.xyz) + UniformDataPS.viewPos.xyzx.xyz;
    Temp[0].w = dot(Temp[1].xyz, Temp[1].xyz);
    Temp[0].w = inversesqrt(Temp[0].w);
    Temp[1].xyz = Temp[0].www * Temp[1].xyz;
    Temp[0].w = max(Temp[0].y, intBitsToFloat(0x0));
    Temp[1].w = Temp[0].y * intBitsToFloat(int(0xC0000000u));
    Temp[2].xyz = Temp[0].xyz * (-Temp[1].www) + vec3(intBitsToFloat(int(0x80000000u)), intBitsToFloat(int(0xBF800000u)), intBitsToFloat(int(0x80000000u)));
    Temp[1].w = dot(Temp[1].xyz, Temp[2].xyz);
    Temp[1].w = max(Temp[1].w, intBitsToFloat(0x0));
    Temp[1].w = log2(Temp[1].w);
    Temp[1].w = Temp[1].w * GlobalsPS.shininess;
    Temp[1].w = exp2(Temp[1].w);
    Temp[2].xyz = texture(diffuseTex, Input3.xy).xyz;
    Temp[0].w = Temp[0].w * intBitsToFloat(0x3F4CCCCD);
    Temp[3].xyz = Temp[2].xyz * Temp[0].www;
    Temp[0].w = Temp[1].w * intBitsToFloat(0x3F000000);
    Temp[4].xyz = texture(specularTex, Input3.xy).xyz;
    Temp[3].xyz = Temp[2].xyz * vec3(intBitsToFloat(0x3D4CCCCD), intBitsToFloat(0x3D4CCCCD), intBitsToFloat(0x3D4CCCCD)) + Temp[3].xyz;
    Temp[3].xyz = Temp[0].www * Temp[4].xyz + Temp[3].xyz;
    Temp[5].xyz = Temp[3].xyz;
    Temp[0].w = intBitsToFloat(0x0);
    while(true){
        // IGE+BREAKC opt
        if ((floatBitsToInt(Temp[0]).w>= UniformDataPS.numberOfLights)) { break; }
        Temp[1].w = uintBitsToFloat((floatBitsToInt(Temp[0]).w>=UniformDataPS.numberOfLights) ? 0xFFFFFFFFu : 0u);
        Temp[1].w = intBitsToFloat(floatBitsToInt(Temp[0]).w * 0x7);
        Temp[6].xyz = TempArray0[floatBitsToInt(Temp[1]).w].xyz;
        Temp[2].w = TempArray0[1+floatBitsToInt(Temp[1]).w].x;
        Temp[3].w = TempArray0[2+floatBitsToInt(Temp[1]).w].x;
        Temp[4].w = TempArray0[3+floatBitsToInt(Temp[1]).w].x;
        Temp[7].xyz = TempArray0[4+floatBitsToInt(Temp[1]).w].xyz;
        Temp[8].xyz = TempArray0[5+floatBitsToInt(Temp[1]).w].xyz;
        Temp[9].xyz = TempArray0[6+floatBitsToInt(Temp[1]).w].xyz;
        Temp[6].xyz = Temp[6].xyz + (-Input1.xyz);
        Temp[1].w = dot(Temp[6].xyz, Temp[6].xyz);
        Temp[5].w = inversesqrt(Temp[1].w);
        Temp[6].xyz = Temp[5].www * Temp[6].xyz;
        Temp[5].w = dot(Temp[0].xyz, Temp[6].xyz);
        Temp[5].w = max(Temp[5].w, intBitsToFloat(0x0));
        Temp[6].w = dot((-Temp[6].xyz), Temp[0].xyz);
        Temp[6].w = Temp[6].w + Temp[6].w;
        Temp[6].xyz = Temp[0].xyz * (-Temp[6].www) + (-Temp[6].xyz);
        Temp[6].x = dot(Temp[1].xyz, Temp[6].xyz);
        Temp[6].x = max(Temp[6].x, intBitsToFloat(0x0));
        Temp[6].x = log2(Temp[6].x);
        Temp[6].x = Temp[6].x * GlobalsPS.shininess;
        Temp[6].x = exp2(Temp[6].x);
        Temp[6].y = sqrt(Temp[1].w);
        Temp[2].w = Temp[3].w * Temp[6].y + Temp[2].w;
        Temp[1].w = Temp[4].w * Temp[1].w + Temp[2].w;
        Temp[1].w = float(intBitsToFloat(0x3F800000)) / Temp[1].w;
        Temp[6].yzw = Temp[2].xyz * Temp[7].xyz;
        Temp[7].xyz = Temp[5].www * Temp[8].xyz;
        Temp[7].xyz = Temp[2].xyz * Temp[7].xyz;
        Temp[8].xyz = Temp[6].xxx * Temp[9].xyz;
        Temp[8].xyz = Temp[4].xyz * Temp[8].xyz;
        Temp[7].xyz = Temp[1].www * Temp[7].xyz;
        Temp[6].xyz = Temp[6].yzw * Temp[1].www + Temp[7].xyz;
        Temp[6].xyz = Temp[8].xyz * Temp[1].www + Temp[6].xyz;
        Temp[5].xyz = Temp[5].xyz + Temp[6].xyz;
        Temp[0].w = intBitsToFloat(floatBitsToInt(Temp[0]).w + 0x1);
    }
    Output0.xyz = Temp[5].xyz;
    Output0.w = intBitsToFloat(0x3F800000);
    return;
}
