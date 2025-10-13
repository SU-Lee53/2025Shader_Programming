#version 330

in vec3 a_Position;
in float a_Value;
in vec4 a_Color;
in float a_STime;
in vec3 a_Velocity;
in float a_LifeTime;
in float a_Mass;
in float a_Period;

out vec4 v_Color;

uniform float u_Time;
uniform vec3 u_Force;	// ¿ÜºÎ Èû

const float c_PI = 3.141592;
const vec2 c_Gravity = vec2(0, -9.8);

void raining()
{
	vec4 newPosition = vec4(a_Position, 1);
	float newTime = u_Time - a_STime;
	float newAlpha = 1.0;

	if (newTime > 0) { 
		float t = fract(newTime / a_LifeTime) * a_LifeTime;		// t : 0 ~ lifeTime
		float tt = t * t;

		float forceX = u_Force.x + c_Gravity.x * a_Mass;
		float forceY = u_Force.y + c_Gravity.y * a_Mass;

		float aX = forceX / a_Mass;
		float aY = forceY / a_Mass;

		float x = (a_Velocity.x * t) + (0.5 * aX * tt);
		float y = (a_Velocity.y * t) + (0.5 * aY * tt);
		
		newPosition.xy += vec2(x, y);
		newAlpha = a_LifeTime - (t / a_LifeTime);		// a_LifeTime ~ 0
	}
	else {
		newPosition.xy = vec2(-100000, 0);
	}

	
	gl_Position = newPosition;
	v_Color = vec4(a_Color.rgb, newAlpha);
}

void sinParticle()
{
	vec4 centerC = vec4(1, 0, 0, 0);
	vec4 borderC = vec4(1, 1, 1, 0);
	mix(centerC, borderC, 0.5);

	float newTime = u_Time - a_STime;
	float lifeTime = a_LifeTime;
	float newAlpha = 1.0;
	vec4 newColor = a_Color;
	vec4 newPosition = vec4(a_Position, 1);

	if(newTime > 0){
		float period = a_Period * 4;
		float t = fract(newTime / a_LifeTime) * a_LifeTime;	// 0 ~ a_LifeTime
		float tt = t*t;
		float x = 2 * t - 1;
		float y = t * sin(2 * t * c_PI * period) * ((a_Value - 0.5) * 2);
		y *= sin(fract(newTime / lifeTime) * c_PI);

		newPosition.xy += vec2(x, y);
		newAlpha = 1.0 - (t / lifeTime);		// a_LifeTime ~ 0

		newColor = mix(centerC, borderC, abs(y * 8));
	}
	else {
		newPosition.xy = vec2(-100000, 0);
	}

	gl_Position = newPosition;
	v_Color = vec4(newColor.rgb, newAlpha);
}

void circleParticle()
{
	float newTime = u_Time - a_STime;
	float lifeTime = a_LifeTime;
	float newAlpha = 1.0;
	vec4 newColor = a_Color;
	vec4 newPosition = vec4(a_Position, 1);

	if(newTime > 0){
		float t = fract(newTime / lifeTime) * lifeTime;	// 0 ~ a_LifeTime
		float tt = t*t;
		float x = sin(a_Value * 2 * c_PI);
		float y = cos(a_Value * 2 * c_PI);
		float newX = x + 0.5 * c_Gravity.x * tt;
		float newY = y + 0.5 * c_Gravity.y * tt;

		newPosition.xy += vec2(newX, newY);

		newAlpha = 1.0 - (t / lifeTime);		// a_LifeTime ~ 0
	}
	else {
		newPosition.xy = vec2(-100000, 0);
	}

	gl_Position = newPosition;
	v_Color = vec4(a_Color.rgb, newAlpha);
}


void main()
{
	//raining();
	//sinParticle();
	circleParticle();
}
