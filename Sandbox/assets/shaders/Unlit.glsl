// Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;


void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_BaseColor;
uniform float u_TextureTiling = 1.0;


void main()
{
	color = vec4(u_BaseColor) * texture(u_Texture, v_TexCoord * u_TextureTiling);
}