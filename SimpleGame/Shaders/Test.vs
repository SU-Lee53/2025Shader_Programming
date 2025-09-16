#version 330

in vec3 a_Position;	// ����ڰ� ������ �Է°�
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
	float x = cos(rad);
	float y = sin(rad);

	newPosition.xy = newPosition.xy + fract(u_Time / 5) * vec2(x, y);
	gl_Position = newPosition;
	// gl_Position : D3D�� System value �� �����, -1 ~ 1 ������ Projection �� ��ǥ�� ����

	v_Color = a_Color;
}
