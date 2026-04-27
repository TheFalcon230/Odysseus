// Basic Texture Shader
#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in vec3 a_Normal;
layout(location = 6) in float a_Roughness;
layout(location = 7) in float a_Metallic;
layout(location = 8) in float a_AO;
layout(location = 9) in int a_EntityID;

// uniform mat4 u_ViewProjection;

layout(std140, binding = 0) uniform CameraBuffer
{
	mat4 u_ViewProjection;
	//mat4 u_Model;
	//mat3 u_NormalMatrix;
	vec3 u_CameraPosition;
};

layout(std140, binding = 1) uniform LightBuffer
{
	vec3 u_LightBufferPos;
	vec3 u_LightBufferColor;
	float u_LightBufferIntensity;

	vec3 u_DirectionalLightDirection;
	vec3 u_DirectionalLightColor;
	float u_DirectionalLightIntensity;
};

struct VertexOutput
{
	vec4 Color;
	vec3 Normal;
	vec2 TexCoord;
	float TilingFactor;
};


struct Light {
	vec3 u_LightPos;
	vec3 u_LightColor;
	float u_LightIntensity;

	vec3 DirectionalLightDirection;
	vec3 DirectionalLightColor;
	float DirectionalLightIntensity;
};

struct Material
{
	float Roughness;
	float Metallic;
	float AO;
};

layout (location = 0) out vec3 Pos;
layout (location = 4) out flat float v_TexIndex;
layout (location = 5) out flat int v_EntityID;
layout (location = 6) out vec3 v_CameraPosition;
layout (location = 10) out VertexOutput Output;
layout (location = 19) out Material MaterialOutput;
layout (location = 26) out Light LightOutput;

void main()
{
	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	Output.TilingFactor = a_TilingFactor;
	v_EntityID = a_EntityID;
	Output.Normal = a_Normal;
	MaterialOutput.Roughness = a_Roughness;
	MaterialOutput.AO = a_AO;
	MaterialOutput.Metallic = a_Metallic;

	v_CameraPosition = u_CameraPosition;

	LightOutput.u_LightPos = u_LightBufferPos;
	LightOutput.u_LightColor = u_LightBufferColor;
	LightOutput.u_LightIntensity = u_LightBufferIntensity;

	LightOutput.DirectionalLightDirection = u_DirectionalLightDirection;
	LightOutput.DirectionalLightColor = u_DirectionalLightColor;
	LightOutput.DirectionalLightIntensity = u_DirectionalLightIntensity;

	Pos = a_Position;
	gl_Position = u_ViewProjection * vec4(Pos, 1.0f);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int ID;


struct Light {
	vec3 u_LightPos;
	vec3 u_LightColor;
	float u_LightIntensity;

	vec3 DirectionalLightDirection;
	vec3 DirectionalLightColor;
	float DirectionalLightIntensity;
};

struct VertexOutput
{
	vec4 Color;
	vec3 Normal;
	vec2 TexCoord;
	float TilingFactor;
};

struct Material
{
	float Roughness;
	float Metallic;
	float AO;
};

layout (location = 0) in vec3 Pos;
layout (location = 4) in flat float v_TexIndex;
layout (location = 5) in flat int v_EntityID;
layout (location = 6) in vec3 v_CameraPosition;
layout (location = 10) in VertexOutput Output;
layout (location = 19) in Material MaterialOutput;
layout (location = 26) in Light LightOutput;

const float PI = 3.14159265359;

layout (binding = 0) uniform sampler2D u_Texture;
layout (binding = 1) uniform sampler2D u_NormalMap;
layout (binding = 2) uniform sampler2D u_ORM;

float DiffuseFresnel(vec3 v, vec3 h, vec3 l, vec3 n, float roughness);
float DistributionGGX(vec3 N, vec3 H, float roughness);
vec3 SpecularFresnel(float VdotH, vec3 F0);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);

void main()
{
	vec2 UV = Output.TexCoord * Output.TilingFactor;
	vec4 albedo = texture(u_Texture, UV);

	float ao = mix(1.0f, texture(u_ORM, UV).r, MaterialOutput.AO);
	float roughness = texture(u_ORM, UV).g * MaterialOutput.Roughness;
	roughness = clamp(roughness, 0.1f, 0.8f);
	float metallic = texture(u_ORM, UV).b * MaterialOutput.Metallic;

	float distance = length(LightOutput.u_LightPos - Pos);
	float attenuation = 1 / (distance * distance);
	vec3 radiance = LightOutput.u_LightColor * attenuation;

	vec3 Cdiff = Output.Color.rgb * albedo.rgb;

	// obtain normal from normal map in range [0,1]
    vec3 normal = texture(u_NormalMap, UV).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0f - 1);  // this normal is in tangent space
	//normal = normalize(Output.TBN * normal);

    vec3 N = normal * Output.Normal;
    vec3 V = normalize(v_CameraPosition - Pos);


    vec3 ambient = vec3(0.1f) * Cdiff;
    vec3 Lo = ambient;

	vec3 Li = LightOutput.u_LightColor * LightOutput.u_LightIntensity;

    vec3 L = normalize(LightOutput.u_LightPos - Pos);
    vec3 H = normalize(L + V);
    float NdotL = max(dot(N, L), 0.0f);
    float HdotV = max(dot(H, V), 0.0f);

	float F0 = DiffuseFresnel(V, H, L, N, roughness);

	float NDF = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	vec3 F = SpecularFresnel(HdotV, vec3(0.5f));

	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0f) * NdotL + 0.0001f;
	vec3 specular = numerator / denominator;

	Cdiff *= F0;
	Lo += ((Cdiff * (1 - metallic)) + specular * mix(vec3(1.0f), Cdiff, metallic)) * NdotL * ao * radiance;

    vec3 finalColor = Lo;

    finalColor = finalColor / (finalColor + vec3(1.0f));
    finalColor = pow(finalColor, vec3(1.0f/2.2f));

    FragColor.rgb = finalColor;

	if(gl_FrontFacing)
		FragColor.a = albedo.a;
	else
	{
		FragColor.a = 0.0f;
	}

	ID = v_EntityID;
}

float DiffuseFresnel(vec3 v, vec3 h, vec3 l, vec3 n, float roughness)
{
	float LdotH = dot(l, h);
	float F90 = 0.5f + 2.0f * (roughness * (LdotH * LdotH));
	float F0 = 1.0f;
	float F_Diffuse = mix(F0, F90, dot(n, l)) * mix(F0, F90, dot(v, l));

	return F_Diffuse;
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

vec3 SpecularFresnel(float VdotH, vec3 F0)
{
	float exponent = (-5.55473f * VdotH - 6.98316f ) * VdotH;
	return F0 + (1.0f - F0) * pow(2.0f, exponent);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}