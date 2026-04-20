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

// uniform mat4 u_ViewProjection;

layout(std140, binding = 0) uniform CameraBuffer
{
	mat4 u_ViewProjection;
	mat4 u_Model;
	vec3 u_CameraPosition;
};

layout(std140, binding = 1) uniform LightBuffer
{
	vec3 u_LightBufferPos;
	vec3 u_LightBufferColor;
	float u_LightBufferIntensity;
};

out vec4 v_Color;
out vec3 v_Normal;
out vec3 FragPos;
out vec2 v_TexCoord;
out  float v_TexIndex;
out float v_TilingFactor;
out flat int v_EntityID;
out vec3 v_CameraPosition;

struct Light {
	vec3 u_LightPos;
	vec3 u_LightColor;
	float u_LightIntensity;
};

out Light LightOutput;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	v_EntityID = a_EntityID;
	v_Normal = a_Normal;

	v_CameraPosition = u_CameraPosition;

	LightOutput.u_LightPos = u_LightBufferPos;
	LightOutput.u_LightColor = u_LightBufferColor;
	LightOutput.u_LightIntensity = u_LightBufferIntensity;

	FragPos = vec3( u_Model * vec4(a_Position, 1.0f));
	gl_Position = u_ViewProjection *  vec4(a_Position, 1.0);
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

in Light LightOutput;
in vec4 v_Color;
in vec3 v_Normal;
in vec3 FragPos;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilingFactor;
in flat int v_EntityID;
in vec3 v_CameraPosition;

uniform sampler2D u_Textures[32];

void main()
{
	vec4 objectColor = v_Color;

	switch(int(v_TexIndex))
	{
		case  0: objectColor *= texture(u_Textures[ 0], v_TexCoord * v_TilingFactor); break;
		case  1: objectColor *= texture(u_Textures[ 1], v_TexCoord * v_TilingFactor); break;
		case  2: objectColor *= texture(u_Textures[ 2], v_TexCoord * v_TilingFactor); break;
		case  3: objectColor *= texture(u_Textures[ 3], v_TexCoord * v_TilingFactor); break;
		case  4: objectColor *= texture(u_Textures[ 4], v_TexCoord * v_TilingFactor); break;
		case  5: objectColor *= texture(u_Textures[ 5], v_TexCoord * v_TilingFactor); break;
		case  6: objectColor *= texture(u_Textures[ 6], v_TexCoord * v_TilingFactor); break;
		case  7: objectColor *= texture(u_Textures[ 7], v_TexCoord * v_TilingFactor); break;
		case  8: objectColor *= texture(u_Textures[ 8], v_TexCoord * v_TilingFactor); break;
		case  9: objectColor *= texture(u_Textures[ 9], v_TexCoord * v_TilingFactor); break;
		case 10: objectColor *= texture(u_Textures[10], v_TexCoord * v_TilingFactor); break;
		case 11: objectColor *= texture(u_Textures[11], v_TexCoord * v_TilingFactor); break;
		case 12: objectColor *= texture(u_Textures[12], v_TexCoord * v_TilingFactor); break;
		case 13: objectColor *= texture(u_Textures[13], v_TexCoord * v_TilingFactor); break;
		case 14: objectColor *= texture(u_Textures[14], v_TexCoord * v_TilingFactor); break;
		case 15: objectColor *= texture(u_Textures[15], v_TexCoord * v_TilingFactor); break;
		case 16: objectColor *= texture(u_Textures[16], v_TexCoord * v_TilingFactor); break;
		case 17: objectColor *= texture(u_Textures[17], v_TexCoord * v_TilingFactor); break;
		case 18: objectColor *= texture(u_Textures[18], v_TexCoord * v_TilingFactor); break;
		case 19: objectColor *= texture(u_Textures[19], v_TexCoord * v_TilingFactor); break;
		case 20: objectColor *= texture(u_Textures[20], v_TexCoord * v_TilingFactor); break;
		case 21: objectColor *= texture(u_Textures[21], v_TexCoord * v_TilingFactor); break;
		case 22: objectColor *= texture(u_Textures[22], v_TexCoord * v_TilingFactor); break;
		case 23: objectColor *= texture(u_Textures[23], v_TexCoord * v_TilingFactor); break;
		case 24: objectColor *= texture(u_Textures[24], v_TexCoord * v_TilingFactor); break;
		case 25: objectColor *= texture(u_Textures[25], v_TexCoord * v_TilingFactor); break;
		case 26: objectColor *= texture(u_Textures[26], v_TexCoord * v_TilingFactor); break;
		case 27: objectColor *= texture(u_Textures[27], v_TexCoord * v_TilingFactor); break;
		case 28: objectColor *= texture(u_Textures[28], v_TexCoord * v_TilingFactor); break;
		case 29: objectColor *= texture(u_Textures[29], v_TexCoord * v_TilingFactor); break;
		case 30: objectColor *= texture(u_Textures[30], v_TexCoord * v_TilingFactor); break;
		case 31: objectColor *= texture(u_Textures[31], v_TexCoord * v_TilingFactor); break;
	}
	float ambientStrength = 0.1f;
	vec3 Ambient = ambientStrength * vec3(1.0f);

	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(LightOutput.u_LightPos - FragPos);

	float LdotN = dot(norm, lightDir);

	float diff = max(LdotN, 0.0f);

	vec3 Diffuse = diff * LightOutput.u_LightColor * LightOutput.u_LightIntensity; 

	float specularStrength = 0.5f;
	vec3 viewDir = normalize( v_CameraPosition - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 Specular = specularStrength * spec * LightOutput.u_LightColor;

	FragColor.rgb = (Ambient + Diffuse + Specular) * objectColor.rgb;
	FragColor.a = objectColor.a;

	ID = v_EntityID;
}