#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_UV;
uniform float u_Time;

const float c_PI = 3.141592f;

void Wave()
{
    vec2 center = vec2(0.5, 0.5);
    float dist = length(v_UV - center);

    // 기본 파동
    float frequency = 12.0;   // 낮을수록 간격 넓음
    float speed = 6.0;
    float wave = sin(dist * frequency - u_Time * 0.02 * speed);

    // 링 강조: 절댓값을 쓰고 경계선만 남기기
    float ring = smoothstep(0.95, 1.0, abs(wave)); // 0.95 ~ 1.0 근처만 밝게

    // 감쇠 (멀수록 사라짐)
    float attenuation = exp(-dist * 3.0);

    float finalValue = ring * attenuation;

    // 회색조
    FragColor = vec4(vec3(finalValue), 1.0);
}

void main()
{
	//	vec4 newColor = vec4(0.f);
	//	float xValue = pow(abs(sin(v_UV.x * 2 * c_PI * 4 - u_Time)), 4096);
	//	float yValue = pow(abs(sin(v_UV.y * 2 * c_PI * 4 - u_Time)), 4096);
	//	// pow : 강도
	//	
	//	newColor = vec4(xValue + yValue);
	//	
	//	FragColor = newColor;

	Wave();
}
