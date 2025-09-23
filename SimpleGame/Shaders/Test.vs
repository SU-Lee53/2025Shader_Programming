#version 330

// 사용자가 정의한 입력값
in vec3 a_Position;	
in float a_Radius;
in vec4 a_Color;

out vec4 v_Color;

uniform float u_Time;

/*
	a_ : ahttribute
	u_ : uniform
*/

const float c_PI = 3.141592;

void main()
{
	vec4 newPosition = vec4(a_Position, 1);

	float fValue = (fract(u_Time) * 2) - 1;
	float rad = (fValue + 1) * c_PI;		// 0 ~ 2PI
	float x = 0; a_Radius * cos(rad);
	float y = 0; a_Radius * sin(rad);

	newPosition.xy = newPosition.xy + vec2(x, y);
	gl_Position = newPosition;
	// gl_Position : D3D의 System value 와 비슷함, -1 ~ 1 사이의 Projection 된 좌표를 가짐

	v_Color = a_Color;
}
