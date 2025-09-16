#version 330

in vec3 a_Position;	// ����ڰ� ������ �Է°�
in vec4 a_Color;

out vec4 v_Color;

/*
	a_ : Attribute
	u_ : uniform
*/

void main()
{
	vec4 newPosition = vec4(a_Position, 1);
	newPosition.xy = newPosition.xy + vec2(0,1);
	gl_Position = newPosition;
	// gl_Position : D3D�� System value �� �����, -1 ~ 1 ������ Projection �� ��ǥ�� ����

	v_Color = a_Color;
}
