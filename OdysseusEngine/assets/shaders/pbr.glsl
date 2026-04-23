// Basic Texture Shader
#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in vec2 a_TexCoord;
layout(location = 4) in float a_TexIndex;
layout(location = 5) in float a_TilingFactor;
layout(location = 6) in int a_EntityID;
layout(location = 7) in float a_Roughness;
layout(location = 8) in float a_Metallic;
layout(location = 9) in float a_AO;

// uniform mat4 u_ViewProjection;

layout(std140, binding = 0) uniform CameraBuffer
{
	mat4 u_ViewProjection;
	mat4 u_Model;
	mat3 u_NormalMatrix;
	vec3 u_CameraPosition;
};

layout(std140, binding = 1) uniform LightBuffer
{
	vec3 u_LightBufferPos;
	vec3 u_LightBufferColor;
	float u_LightBufferIntensity;
};

struct VertexOutput
{
	vec4 Color;
	vec3 Normal;
	vec2 TexCoord;
	float TilingFactor;
	mat3 TBN;
};

out vec3 FragPos;
out flat float v_TexIndex;
out flat int v_EntityID;
out vec3 v_CameraPosition;

struct Light {
	vec3 u_LightPos;
	vec3 u_LightColor;
	float u_LightIntensity;
};

struct Material
{
	float Roughness;
	float Metallic;
	float AO;
};

out Light LightOutput;
out VertexOutput Output;
out Material MaterialOutput;

void main()
{
		Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	Output.TilingFactor = a_TilingFactor;
	v_EntityID = a_EntityID;
	Output.Normal = a_Normal;
	MaterialOutput.Roughness = a_Roughness;

	v_CameraPosition = u_CameraPosition;

	LightOutput.u_LightPos = u_LightBufferPos;
	LightOutput.u_LightColor = u_LightBufferColor;
	LightOutput.u_LightIntensity = u_LightBufferIntensity;

	FragPos = vec3( u_Model * vec4(a_Position, 1.0f));
	gl_Position = u_ViewProjection *  vec4(a_Position, 1.0f);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int ID;

struct Light {
	vec3 u_LightPos;
	vec3 u_LightColor;
	float u_LightIntensity;
};

struct VertexOutput
{
	vec4 Color;
	vec3 Normal;
	vec2 TexCoord;
	float TilingFactor;
	mat3 TBN;
};

struct Material
{
	float Roughness;
	float Metallic;
	float AO;
};

in Light LightOutput;
in VertexOutput Output;
in Material MaterialOutput;
in vec3 FragPos;
in flat float v_TexIndex;
in flat int v_EntityID;
in vec3 v_CameraPosition;

const float PI = 3.14159265359;

layout (binding = 0) uniform sampler2D u_Texture;
layout (binding = 1) uniform sampler2D u_NormalMap;
layout (binding = 2) uniform sampler2D u_ORM;

float CalculateAttenuation(vec3 pos0, vec3 pos1);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{
	vec2 UV = Output.TexCoord * Output.TilingFactor;
	vec4 albedo = texture(u_Texture, UV) * Output.Color;

	float ao = texture(u_ORM, UV).r;
	float roughness = texture(u_ORM, UV).g;
	float metallic = texture(u_ORM, UV).b;

	// switch(int(v_TexIndex))
	// {
	// 	case  0: objectColor *= texture(u_Textures[ 0], Output.TexCoord * Output.TilingFactor); break;
	// 	case  1: objectColor *= texture(u_Textures[ 1], Output.TexCoord * Output.TilingFactor); break;
	// 	case  2: objectColor *= texture(u_Textures[ 2], Output.TexCoord * Output.TilingFactor); break;
	// 	case  3: objectColor *= texture(u_Textures[ 3], Output.TexCoord * Output.TilingFactor); break;
	// 	case  4: objectColor *= texture(u_Textures[ 4], Output.TexCoord * Output.TilingFactor); break;
	// 	case  5: objectColor *= texture(u_Textures[ 5], Output.TexCoord * Output.TilingFactor); break;
	// 	case  6: objectColor *= texture(u_Textures[ 6], Output.TexCoord * Output.TilingFactor); break;
	// 	case  7: objectColor *= texture(u_Textures[ 7], Output.TexCoord * Output.TilingFactor); break;
	// 	case  8: objectColor *= texture(u_Textures[ 8], Output.TexCoord * Output.TilingFactor); break;
	// 	case  9: objectColor *= texture(u_Textures[ 9], Output.TexCoord * Output.TilingFactor); break;
	// 	case 10: objectColor *= texture(u_Textures[10], Output.TexCoord * Output.TilingFactor); break;
	// 	case 11: objectColor *= texture(u_Textures[11], Output.TexCoord * Output.TilingFactor); break;
	// 	case 12: objectColor *= texture(u_Textures[12], Output.TexCoord * Output.TilingFactor); break;
	// 	case 13: objectColor *= texture(u_Textures[13], Output.TexCoord * Output.TilingFactor); break;
	// 	case 14: objectColor *= texture(u_Textures[14], Output.TexCoord * Output.TilingFactor); break;
	// 	case 15: objectColor *= texture(u_Textures[15], Output.TexCoord * Output.TilingFactor); break;
	// 	case 16: objectColor *= texture(u_Textures[16], Output.TexCoord * Output.TilingFactor); break;
	// 	case 17: objectColor *= texture(u_Textures[17], Output.TexCoord * Output.TilingFactor); break;
	// 	case 18: objectColor *= texture(u_Textures[18], Output.TexCoord * Output.TilingFactor); break;
	// 	case 19: objectColor *= texture(u_Textures[19], Output.TexCoord * Output.TilingFactor); break;
	// 	case 20: objectColor *= texture(u_Textures[20], Output.TexCoord * Output.TilingFactor); break;
	// 	case 21: objectColor *= texture(u_Textures[21], Output.TexCoord * Output.TilingFactor); break;
	// 	case 22: objectColor *= texture(u_Textures[22], Output.TexCoord * Output.TilingFactor); break;
	// 	case 23: objectColor *= texture(u_Textures[23], Output.TexCoord * Output.TilingFactor); break;
	// 	case 24: objectColor *= texture(u_Textures[24], Output.TexCoord * Output.TilingFactor); break;
	// 	case 25: objectColor *= texture(u_Textures[25], Output.TexCoord * Output.TilingFactor); break;
	// 	case 26: objectColor *= texture(u_Textures[26], Output.TexCoord * Output.TilingFactor); break;
	// 	case 27: objectColor *= texture(u_Textures[27], Output.TexCoord * Output.TilingFactor); break;
	// 	case 28: objectColor *= texture(u_Textures[28], Output.TexCoord * Output.TilingFactor); break;
	// 	case 29: objectColor *= texture(u_Textures[29], Output.TexCoord * Output.TilingFactor); break;
	// 	case 30: objectColor *= texture(u_Textures[30], Output.TexCoord * Output.TilingFactor); break;
	// 	case 31: objectColor *= texture(u_Textures[31], Output.TexCoord * Output.TilingFactor); break;
	// }

	// // obtain normal from normal map in range [0,1]
    // vec3 normal = texture(u_NormalMap, UV).rgb;
    // // transform normal vector to range [-1,1]
    // normal = normalize(normal * 2.0f - 1);  // this normal is in tangent space
	// //normal = normalize(Output.TBN * normal);

    // vec3 N = normal * Output.Normal;
    // vec3 V = normalize(v_CameraPosition - FragPos);

    // vec3 F0 = vec3(0.04f);
    // F0 = mix(F0, albedo.rgb, metallic);

    // vec3 Lo = vec3(0.0f);

    // vec3 L = normalize(LightOutput.u_LightPos - FragPos);
    // vec3 H = normalize(L + V);
    // float NdotL = max(dot(N, L), 0.0f);
    // float attenuation = CalculateAttenuation(FragPos,LightOutput.u_LightPos) + LightOutput.u_LightIntensity;
    // vec3 radiance = LightOutput.u_LightColor * attenuation;

    // float NDF = DistributionGGX(N, H, roughness);        
    // float G   = GeometrySmith(N, V, L,roughness); 
    // vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    // vec3 kS = F;
    // vec3 kD = vec3(1.0f) - kS;
    // kD *= 1.0f - metallic;

    // vec3 numerator = NDF * G * F;
    // float denominator = 4.0f * max(dot(N, V), 0.0f) * NdotL + 0.0001f;
    // vec3 specular = numerator / denominator;

    // Lo += (kD * albedo.rgb / PI * specular) * radiance * NdotL;

    // vec3 ambient = vec3(0.03f) * albedo.rgb * ao;
    // vec3 finalColor = Lo + ambient;

    // finalColor = finalColor / (finalColor + vec3(1.0f));
    // finalColor = pow(finalColor, vec3(1.0f/2.2f));

    // FragColor.rgb = finalColor.rgb;
	// FragColor.a = albedo.a;

	// ID = v_EntityID;

	

	float ambientStrength = 0.05f;
	vec3 Ambient = ambientStrength * LightOutput.u_LightColor;

	vec3 lightDir = normalize(LightOutput.u_LightPos - FragPos);
	vec3 norm = normalize(Output.Normal);
	vec3 viewDir = normalize( v_CameraPosition - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	float LdotN = dot(norm, lightDir);
	float diff = LdotN * 0.5 + 0.5;


	vec3 Diffuse = diff * LightOutput.u_LightColor; 
	Diffuse *= LightOutput.u_LightIntensity;

	float specularStrength = 0.5f;
	float spec = pow(dot(norm, halfwayDir)*0.5f + 0.5f, 256.0f * (1.0f - MaterialOutput.Roughness));
	vec3 Specular = specularStrength * spec * LightOutput.u_LightColor * LightOutput.u_LightIntensity;

	FragColor.rgb = (Ambient + Diffuse + Specular) * albedo.rgb;
	FragColor.a = albedo.a;

	ID = v_EntityID;
}

float CalculateAttenuation(vec3 pos0, vec3 pos1)
{
    float distance = length(pos1 - pos0);

	float numerator = clamp(pow(1-(distance/500.f), 4), 0.0f, 1.0f);

    float attenuation = (numerator * numerator) / (distance * distance) + 1;
		
    return attenuation;
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}
