#version 330

layout(location=0) out vec4 FragColor;
layout(location=1) out vec4 FragColorHigh;

uniform sampler2D u_Texture;

in vec4 v_Color;
in vec2 v_UV;


void main()
{
	vec4 result = texture(u_Texture, v_UV) * v_Color * 1.5;
	FragColor = result;
	
	float brightness = dot(result.rgb, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 1.0)
		FragColorHigh = result;
	else
		FragColorHigh = vec4(0);
}
