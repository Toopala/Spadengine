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
layout(location = 1) uniform struct GlobalsPS_Type {
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
layout(location = 0) uniform struct UniformDataPS_Type {
	int numberOfLights;
	dirLight_Type dirLight;
	pointLights_Type pointLights;
	vec3 viewPos;
} UniformDataPS;
layout(location = 2) uniform sampler2D diffuseTex;
layout(location = 3) uniform sampler2D normalTex;
layout(location = 4) uniform sampler2D specularTex;
layout(location = 1)  in  vec4 VtxGeoOutput1;
vec4 Input1;
layout(location = 3)  in  vec4 VtxGeoOutput3;
vec4 Input3;
layout(location = 4)  in  vec4 VtxGeoOutput4;
vec4 Input4;
layout(location = 5)  in  vec4 VtxGeoOutput5;
vec4 Input5;
layout(location = 6)  in  vec4 VtxGeoOutput6;
vec4 Input6;
layout(location = 0) out  vec4 PixOutput0;
#define Output0 PixOutput0
vec4 Temp[7];
ivec4 Temp_int[7];
uvec4 Temp_uint[7];
void main()
{
    Input1 = VtxGeoOutput1;
    Input3 = VtxGeoOutput3;
    Input4 = VtxGeoOutput4;
    Input5 = VtxGeoOutput5;
    Input6 = VtxGeoOutput6;
    Temp[0].xyz = (-Input1.xyz) + UniformDataPS.viewPos.xyzx.xyz;
    Temp[0].w = dot(Temp[0].xyz, Temp[0].xyz);
    Temp[0].w = inversesqrt(Temp[0].w);
    Temp[0].xyz = Temp[0].www * Temp[0].xyz;
    Temp[1].xyz = Temp[0].yyy * Input5.xyz;
    Temp[0].xyw = Input4.xyz * Temp[0].xxx + Temp[1].xyz;
    Temp[0].xyz = Input6.xyz * Temp[0].zzz + Temp[0].xyw;
    Temp[1].xyz = Input5.xyz * UniformDataPS.pointLights.position.xyzx.yyy;
    Temp[1].xyz = Input4.xyz * UniformDataPS.pointLights.position.xyzx.xxx + Temp[1].xyz;
    Temp[1].xyz = Input6.xyz * UniformDataPS.pointLights.position.xyzx.zzz + Temp[1].xyz;
    Temp[1].xyz = Temp[1].xyz + (-Input1.xyz);
    Temp[0].w = dot(Temp[1].xyz, Temp[1].xyz);
    Temp[1].w = inversesqrt(Temp[0].w);
    Temp[1].xyz = Temp[1].www * Temp[1].xyz;
    Temp[2].xyz = texture(normalTex, Input3.xy).xyz;
    Temp[2].xyz = Temp[2].xyz * vec3(intBitsToFloat(int(0x40000000u)), intBitsToFloat(int(0x40000000u)), intBitsToFloat(int(0x40000000u))) + vec3(intBitsToFloat(int(0xBF800000u)), intBitsToFloat(int(0xBF800000u)), intBitsToFloat(int(0xBF800000u)));
    Temp[1].w = dot(Temp[2].xyz, Temp[2].xyz);
    Temp[1].w = inversesqrt(Temp[1].w);
    Temp[2].xyz = Temp[1].www * Temp[2].xyz;
    Temp[1].w = dot((-Temp[1].xyz), Temp[2].xyz);
    Temp[1].w = Temp[1].w + Temp[1].w;
    Temp[3].xyz = Temp[2].xyz * (-Temp[1].www) + (-Temp[1].xyz);
    Temp[1].x = dot(Temp[2].xyz, Temp[1].xyz);
    Temp[1].x = max(Temp[1].x, intBitsToFloat(0x0));
    Temp[1].xyz = Temp[1].xxx * UniformDataPS.pointLights.diffuse.xyzx.xyz;
    Temp[1].w = dot(Temp[0].xyz, Temp[3].xyz);
    Temp[1].w = max(Temp[1].w, intBitsToFloat(0x0));
    Temp[1].w = log2(Temp[1].w);
    Temp[1].w = Temp[1].w * GlobalsPS.shininess;
    Temp[1].w = exp2(Temp[1].w);
    Temp[3].xyz = Temp[1].www * UniformDataPS.pointLights.specular.xyzx.xyz;
    Temp[4].xyz = texture(specularTex, Input3.xy).xyz;
    Temp[3].xyz = Temp[3].xyz * Temp[4].xyz;
    Temp[1].w = sqrt(Temp[0].w);
    Temp[1].w = UniformDataPS.pointLights.mylinear * Temp[1].w + UniformDataPS.pointLights.constant;
    Temp[0].w = UniformDataPS.pointLights.quadratic * Temp[0].w + Temp[1].w;
    Temp[0].w = float(intBitsToFloat(0x3F800000)) / Temp[0].w;
    Temp[5].xyz = texture(diffuseTex, Input3.xy).xyz;
    Temp[1].xyz = Temp[1].xyz * Temp[5].xyz;
    Temp[1].xyz = Temp[0].www * Temp[1].xyz;
    Temp[6].xyz = Temp[5].xyz * UniformDataPS.pointLights.ambient.xyzx.xyz;
    Temp[1].xyz = Temp[6].xyz * Temp[0].www + Temp[1].xyz;
    Temp[1].xyz = Temp[3].xyz * Temp[0].www + Temp[1].xyz;
    Temp[0].w = dot((-UniformDataPS.dirLight.direction.xyzx.xyz), (-UniformDataPS.dirLight.direction.xyzx.xyz));
    Temp[0].w = inversesqrt(Temp[0].w);
    Temp[3].xyz = Temp[0].www * (-UniformDataPS.dirLight.direction.xyzx.xyz);
    Temp[0].w = dot((-Temp[3].xyz), Temp[2].xyz);
    Temp[0].w = Temp[0].w + Temp[0].w;
    Temp[6].xyz = Temp[2].xyz * (-Temp[0].www) + (-Temp[3].xyz);
    Temp[0].w = dot(Temp[2].xyz, Temp[3].xyz);
    Temp[0].w = max(Temp[0].w, intBitsToFloat(0x0));
    Temp[2].xyz = Temp[0].www * UniformDataPS.dirLight.diffuse.xyzx.xyz;
    Temp[2].xyz = Temp[5].xyz * Temp[2].xyz;
    Temp[2].xyz = UniformDataPS.dirLight.ambient.xyzx.xyz * Temp[5].xyz + Temp[2].xyz;
    Temp[0].x = dot(Temp[0].xyz, Temp[6].xyz);
    Temp[0].x = max(Temp[0].x, intBitsToFloat(0x0));
    Temp[0].x = log2(Temp[0].x);
    Temp[0].x = Temp[0].x * GlobalsPS.shininess;
    Temp[0].x = exp2(Temp[0].x);
    Temp[0].xyz = Temp[0].xxx * UniformDataPS.dirLight.specular.xyzx.xyz;
    Temp[0].xyz = Temp[0].xyz * Temp[4].xyz + Temp[2].xyz;
    Output0.xyz = Temp[1].xyz + Temp[0].xyz;
    Output0.w = intBitsToFloat(0x3F800000);
    return;
}
