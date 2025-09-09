#version 330

in vec3 a_Position;	// 사용자가 정의한 입력값
in vec4 a_Color;

out vec4 v_Color;

uniform vec4 u_Trans;
uniform vec4 u_Color;

/*
	a_ : Attribute
	u_ : uniform
*/

void main()
{
	vec4 newPosition;
	newPosition.xy = a_Position.xy*u_Trans.w + u_Trans.xy;
	newPosition.z = 0;
	newPosition.w = 1;
	gl_Position = newPosition;
	// gl_Position : D3D의 System value 와 비슷함, -1 ~ 1 사이의 Projection 된 좌표를 가짐

	v_Color = a_Color;
}
