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
vec4 Temp[9];
ivec4 Temp_int[9];
uvec4 Temp_uint[9];
void main()
{
    Input1 = VtxGeoOutput1;
    Input3 = VtxGeoOutput3;
    Input4 = VtxGeoOutput4;
    Input5 = VtxGeoOutput5;
    Input6 = VtxGeoOutput6;
    Temp[0].xyz = texture(normalTex, Input3.xy).xyz;
    Temp[0].xyz = Temp[0].xyz * vec3(intBitsToFloat(int(0x40000000u)), intBitsToFloat(int(0x40000000u)), intBitsToFloat(int(0x40000000u))) + vec3(intBitsToFloat(int(0xBF800000u)), intBitsToFloat(int(0xBF800000u)), intBitsToFloat(int(0xBF800000u)));
    Temp[0].w = dot(Temp[0].xyz, Temp[0].xyz);
    Temp[0].w = inversesqrt(Temp[0].w);
    Temp[0].xyz = Temp[0].www * Temp[0].xyz;
    Temp[1].xyz = (-Input1.xyz) + UniformDataPS.viewPos.xyzx.xyz;
    Temp[0].w = dot(Temp[1].xyz, Temp[1].xyz);
    Temp[0].w = inversesqrt(Temp[0].w);
    Temp[1].xyz = Temp[0].www * Temp[1].xyz;
    Temp[2].xyz = Temp[1].yyy * Input5.xyz;
    Temp[1].xyw = Input4.xyz * Temp[1].xxx + Temp[2].xyz;
    Temp[1].xyz = Input6.xyz * Temp[1].zzz + Temp[1].xyw;
    Temp[0].w = dot((-UniformDataPS.dirLight.direction.xyzx.xyz), (-UniformDataPS.dirLight.direction.xyzx.xyz));
    Temp[0].w = inversesqrt(Temp[0].w);
    Temp[2].xyz = Temp[0].www * (-UniformDataPS.dirLight.direction.xyzx.xyz);
    Temp[3].xyz = Temp[2].yyy * Input5.xyz;
    Temp[2].xyw = Input4.xyz * Temp[2].xxx + Temp[3].xyz;
    Temp[2].xyz = Input6.xyz * Temp[2].zzz + Temp[2].xyw;
    Temp[0].w = dot(Temp[0].xyz, Temp[2].xyz);
    Temp[0].w = max(Temp[0].w, intBitsToFloat(0x0));
    Temp[1].w = dot((-Temp[2].xyz), Temp[0].xyz);
    Temp[1].w = Temp[1].w + Temp[1].w;
    Temp[2].xyz = Temp[0].xyz * (-Temp[1].www) + (-Temp[2].xyz);
    Temp[3].xyz = Temp[2].yyy * Input5.xyz;
    Temp[2].xyw = Input4.xyz * Temp[2].xxx + Temp[3].xyz;
    Temp[2].xyz = Input6.xyz * Temp[2].zzz + Temp[2].xyw;
    Temp[1].w = dot(Temp[1].xyz, Temp[2].xyz);
    Temp[1].w = max(Temp[1].w, intBitsToFloat(0x0));
    Temp[1].w = log2(Temp[1].w);
    Temp[1].w = Temp[1].w * GlobalsPS.shininess;
    Temp[1].w = exp2(Temp[1].w);
    Temp[2].xyz = texture(diffuseTex, Input3.xy).xyz;
    Temp[3].xyz = Temp[0].www * UniformDataPS.dirLight.diffuse.xyzx.xyz;
    Temp[3].xyz = Temp[2].xyz * Temp[3].xyz;
    Temp[4].xyz = Temp[1].www * UniformDataPS.dirLight.specular.xyzx.xyz;
    Temp[5].xyz = texture(specularTex, Input3.xy).xyz;
    Temp[3].xyz = UniformDataPS.dirLight.ambient.xyzx.xyz * Temp[2].xyz + Temp[3].xyz;
    Temp[3].xyz = Temp[4].xyz * Temp[5].xyz + Temp[3].xyz;
    Temp[4].xyz = Temp[3].xyz;
    Temp[0].w = intBitsToFloat(0x0);
    while(true){
        // IGE+BREAKC opt
        if ((floatBitsToInt(Temp[0]).w>= UniformDataPS.numberOfLights)) { break; }
        Temp[1].w = uintBitsToFloat((floatBitsToInt(Temp[0]).w>=UniformDataPS.numberOfLights) ? 0xFFFFFFFFu : 0u);
        Temp[1].w = intBitsToFloat(floatBitsToInt(Temp[0]).w * 0x5);
        Temp[6].xyz = (-Input1.xyz) + UniformDataPS.pointLights.position[floatBitsToInt(Temp[1]).w].xyzx.xyz;
        Temp[2].w = dot(Temp[6].xyz, Temp[6].xyz);
        Temp[3].w = inversesqrt(Temp[2].w);
        Temp[6].xyz = Temp[3].www * Temp[6].xyz;
        Temp[7].xyz = Temp[6].yyy * Input5.xyz;
        Temp[6].xyw = Input4.xyz * Temp[6].xxx + Temp[7].xyz;
        Temp[6].xyz = Input6.xyz * Temp[6].zzz + Temp[6].xyw;
        Temp[3].w = dot(Temp[0].xyz, Temp[6].xyz);
        Temp[3].w = max(Temp[3].w, intBitsToFloat(0x0));
        Temp[4].w = dot((-Temp[6].xyz), Temp[0].xyz);
        Temp[4].w = Temp[4].w + Temp[4].w;
        Temp[6].xyz = Temp[0].xyz * (-Temp[4].www) + (-Temp[6].xyz);
        Temp[7].xyz = Temp[6].yyy * Input5.xyz;
        Temp[6].xyw = Input4.xyz * Temp[6].xxx + Temp[7].xyz;
        Temp[6].xyz = Input6.xyz * Temp[6].zzz + Temp[6].xyw;
        Temp[4].w = dot(Temp[1].xyz, Temp[6].xyz);
        Temp[4].w = max(Temp[4].w, intBitsToFloat(0x0));
        Temp[4].w = log2(Temp[4].w);
        Temp[4].w = Temp[4].w * GlobalsPS.shininess;
        Temp[4].w = exp2(Temp[4].w);
        Temp[5].w = sqrt(Temp[2].w);
        Temp[5].w = UniformDataPS.pointLights.mylinear[floatBitsToInt(Temp[1]).w] * Temp[5].w + UniformDataPS.pointLights.constant[floatBitsToInt(Temp[1]).w];
        Temp[2].w = UniformDataPS.pointLights.quadratic[floatBitsToInt(Temp[1]).w] * Temp[2].w + Temp[5].w;
        Temp[2].w = float(intBitsToFloat(0x3F800000)) / Temp[2].w;
        Temp[6].xyz = Temp[2].xyz * UniformDataPS.pointLights.ambient[floatBitsToInt(Temp[1]).w].xyzx.xyz;
        Temp[7].xyz = Temp[3].www * UniformDataPS.pointLights.diffuse[floatBitsToInt(Temp[1]).w].xyzx.xyz;
        Temp[7].xyz = Temp[2].xyz * Temp[7].xyz;
        Temp[8].xyz = Temp[4].www * UniformDataPS.pointLights.specular[floatBitsToInt(Temp[1]).w].xyzx.xyz;
        Temp[8].xyz = Temp[5].xyz * Temp[8].xyz;
        Temp[7].xyz = Temp[2].www * Temp[7].xyz;
        Temp[6].xyz = Temp[6].xyz * Temp[2].www + Temp[7].xyz;
        Temp[6].xyz = Temp[8].xyz * Temp[2].www + Temp[6].xyz;
        Temp[4].xyz = Temp[4].xyz + Temp[6].xyz;
        Temp[0].w = intBitsToFloat(floatBitsToInt(Temp[0]).w + 0x1);
    }
    Output0.xyz = Temp[4].xyz;
    Output0.w = intBitsToFloat(0x3F800000);
    return;
}
