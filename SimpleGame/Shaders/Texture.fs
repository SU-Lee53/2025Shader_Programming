#version 330

layout(location=0) out vec4 FragColor;

uniform sampler2D u_TexID;

in vec2 v_Tex;

const float c_PI = 3.141592f;

void main()
{
	//vec4 newColor = texture(u_TexID, v_Tex);
	//newColor += texture(u_TexID, vec2(v_Tex.x - 0.01, v_Tex.y - 0.01));
	//newColor += texture(u_TexID, vec2(v_Tex.x - 0.02, v_Tex.y - 0.02));
	//newColor += texture(u_TexID, vec2(v_Tex.x + 0.01, v_Tex.y + 0.01));
	//newColor += texture(u_TexID, vec2(v_Tex.x + 0.02, v_Tex.y + 0.02));
	//newColor /= 5;
	//FragColor = newColor;	


	FragColor = texture(u_TexID, vec2(v_Tex.x, 1 - v_Tex.y));
}
