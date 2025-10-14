#version 330

in vec3 a_Position;

out vec4 v_Color;

uniform float u_Time;

const float c_PI = 3.141592;

void Flag()
{
	// -0.5 <= a_Position.x <= +0.5
	vec4 newPosition = vec4(a_Position, 1);

	float t = fract(u_Time / 1.f) * 1.f;
	float value = a_Position.x + 0.5;	// 0 ~ 1

	newPosition.y = newPosition.y * (1 - value);
	
	float dX = 0;			// cos(2 * valueX * c_PI - u_Time) * (valueX * 0.3);
	float dY = sin(2 * value * c_PI - (u_Time * 5)) * (value * 0.3);	
	// sin 인자에 u_Time 이 더해져야 함 (곱X)
	
	float newColor = (sin(2 * value * c_PI - (u_Time * 5)) + 1) / 2;

	newPosition += vec4(dX, dY, 0, 0);

	gl_Position = newPosition;

	v_Color = vec4(newColor);
}

void Wave()
{
	vec4 newPosition = vec4(a_Position, 1);

	float dX = 0;
	float dY = 0;
	
	vec2 pos = vec2(a_Position.xy);
	vec2 center = vec2(0,0);
	float dist = distance(pos, center);
	float v = clamp(0.5 - dist, 0, 1);
	float newColor = v * sin((dist * 4 * c_PI) * 5 - (u_Time * 10));

	//dY = sin((dist * 4) * c_PI - (u_Time * 3)) * 0.03;	

	newPosition += vec4(dX, dY, 0, 0);
	gl_Position = newPosition;

	v_Color = vec4(newColor);
}

void main()
{
	// Flag();
	Wave();
}
