#version 330

#define MAX_POINTS 50

in vec3 a_Position;

out vec2 v_UV;
out vec4 v_Color;

uniform float u_Time;

const float c_PI = 3.141592;
//const vec2 c_Points[3] = vec2[](vec2(0, 0.2), vec2(-0.2, -0.2), vec2(0.2, -0.2));

// z -> 시작시간 (startTime)
// w -> 지속시간 (lifeTime)
//const vec4 c_Points[3] = vec4[](vec4(0, 0.2, 2, 2), 
//                                vec4(-0.2, -0.2, 3, 3),
//                               vec4(0.2, -0.2, 4, 4));


uniform vec4 u_Points[MAX_POINTS];


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
	v_UV = vec2(a_Position.x + 0.5, 0.5 - a_Position.y);
}

void Wave()
{
	vec4 newPosition = vec4(a_Position, 1);

	float dX = 0;
	float dY = 0;
	
	vec2 pos = vec2(a_Position.xy);
	vec2 center = vec2(0,0);
	float dist = distance(pos, center);
	float v = 2 * clamp(0.5 - dist, 0, 1);	// [0 ~ 0.5] -> [0 ~ 1]
	float newColor = v * sin((dist * 4 * c_PI) * 10 - (u_Time * 30));

	//dY = newColor;

	//dY = sin((dist * 4) * c_PI - (u_Time * 3)) * 0.03;	

	newPosition += vec4(dX, dY, 0, 0);
	gl_Position = newPosition;

	v_Color = vec4(newColor);
}

void RainDrop()
{
	vec4 newPosition = vec4(a_Position, 1);

	float dX = 0;
	float dY = 0;
	
	vec2 pos = vec2(a_Position.xy);
	float newColor = 0;
	
	for (int i = 0; i < MAX_POINTS; ++i) {
		float sTime = u_Points[i].z;
		float lTime = u_Points[i].w;
		float newTime = u_Time - sTime;
		if (newTime > 0) {
			float baseTime = fract(newTime / lTime);
			float oneMinus = 1 - baseTime;
			float t = baseTime * lTime;	// t : 0 ~ lifeTime
			float range = baseTime * lTime / 10;
			vec2 center = u_Points[i].xy;
			float dist = distance(pos, center);
			float v = 100 * clamp(range - dist, 0, 1);	// [0 ~ 0.5] -> [0 ~ 1] , 0.5 : 퍼져나가는 크기
			newColor += v * sin((dist * 4 * c_PI) * 10 - (t * 10)) * oneMinus;
		}


	}

	//dY = newColor;

	//dY = sin((dist * 4) * c_PI - (u_Time * 3)) * 0.03;	

	newPosition += vec4(dX, dY, 0, 0);
	gl_Position = newPosition;

	v_Color = vec4(newColor);
}

void main()
{
	Flag();
	//Wave();
	// RainDrop();
}
