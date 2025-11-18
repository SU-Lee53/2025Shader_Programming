#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_UV;
uniform float u_Time;

uniform sampler2D u_RGBTexture;

const float c_PI = 3.141592f;

void GaussianBlur()
{
	float sigma = 2.0f;
    float twoSigma2 = 2.0 * sigma * sigma;

    // 텍스처 좌표 오프셋(픽셀 사이즈 계산)
    vec2 texelSize = 1.0 / textureSize(u_RGBTexture, 0);

    vec3 result = vec3(0.0);
    float weightSum = 0.0;

    // 블러 radius (sigma에 비례)
    int radius = int(ceil(3.0 * sigma)); // 3-sigma rule

    for (int i = -radius; i <= radius; i++)
    {
        // 가우시안 weight 계산식
        float x = float(i);
        float weight = exp(-(x * x) / twoSigma2);

        vec2 offsetUV = v_UV + vec2(x * texelSize.x, 0.0);
        result += texture(u_RGBTexture, offsetUV).rgb * weight;

        weightSum += weight;
    }

    result /= weightSum;

    FragColor = vec4(result, 1.0);
}

void Test()
{
	vec2 newUV = v_UV;
	float dx = 0.1 * sin(v_UV.y * 2 * c_PI * 3 + u_Time);
	float dy = 0.1 * sin(v_UV.x * 2 * c_PI * 3 + u_Time);
	newUV += vec2(dx, dy);
	vec4 sampledColor = texture(u_RGBTexture, newUV);
	// sampledColor += texture(u_RGBTexture, vec2(newUV.x - 0.02, newUV.y));
	// sampledColor += texture(u_RGBTexture, vec2(newUV.x - 0.04, newUV.y));
	// sampledColor += texture(u_RGBTexture, vec2(newUV.x + 0.02, newUV.y));
	// sampledColor += texture(u_RGBTexture, vec2(newUV.x + 0.04, newUV.y));
	// sampledColor /= 5;

	FragColor = sampledColor;
}

void Circles()
{
	vec2 newUV = v_UV;      // 0 ~ 1, left top (0,0)
    vec2 center = vec2(0.5, 0.5);
    vec4 newColor = vec4(0);
    float d = distance(newUV, center);

    float value = sin(d  * 4 * c_PI * 4 - u_Time);   // 0 ~ 1
    newColor = vec4(value);

	FragColor = newColor;
}

void Flag()
{
	vec2 newUV = vec2(v_UV.x, 1 - v_UV.y - 0.5);      // 0 ~ 1, left middle (0,0)
    vec4 newColor = vec4(0);
   
    // sin 곡선을 따라 꿀렁이는 두께있는 선
    float width = 0.2 * (1 - newUV.x);  // (1 - newUV.x) : 끝부분을 날카롭게
    float sinValue = 0.2 * sin(newUV.x * 2 * c_PI - u_Time) * newUV.x;   // * newUV.x : 왼쪽을 고정

    if (newUV.y < sinValue + width &&
        newUV.y > sinValue - width) {
        newColor = vec4(1);
    }
    else{
        discard;
    }

    FragColor = newColor;
}

// 좌표꼬기 시작
// 시험문제 무조건 나옴

void Q1()
{
	vec2 newUV = vec2(v_UV.x, v_UV.y);      // 0 ~ 1, left top (0,0)
    
    float x = newUV.x;  // 0 ~ 1
    float y = 1 - abs((newUV.y - 0.5) * 2); // 0 ~ 1 ~ 0

	vec4 sampledColor = texture(u_RGBTexture, vec2(x, y));
    
	FragColor = sampledColor;
}

void Q2()
{
	vec2 newUV = vec2(v_UV.x, v_UV.y);      // 0 ~ 1, left top (0,0)
    
    float x = fract(newUV.x * 3);   // 0~1 0~1 0~1
    float y = (2 - floor(newUV.x * 3)) / 3 + newUV.y / 3;

	vec4 sampledColor = texture(u_RGBTexture, vec2(x, y));
    
	FragColor = sampledColor;
}

void Q3()
{
	vec2 newUV = vec2(v_UV.x, v_UV.y);      // 0 ~ 1, left top (0,0)
    
    float x = fract(newUV.x * 3);
    float y = floor(newUV.x * 3) / 3 + newUV.y / 3;

	vec4 sampledColor = texture(u_RGBTexture, vec2(x, y));
    
	FragColor = sampledColor;
}

void Q4()
{
	vec2 newUV = vec2(v_UV.x, v_UV.y);      // 0 ~ 1, left top (0,0)
    

    float x = (newUV.x * 2) - ceil(v_UV.y * 2) / 2;
    float y = newUV.y * 2;

	vec4 sampledColor = texture(u_RGBTexture, vec2(x, y));
    
	FragColor = sampledColor;
}

void Q5()
{
	vec2 newUV = vec2(v_UV.x, v_UV.y);      // 0 ~ 1, left top (0,0)

    float x = newUV.x * 2;
    float y = newUV.y * 2;

	vec4 sampledColor = texture(u_RGBTexture, vec2(x, y));
    
	FragColor = sampledColor;
}

void main()
{
    //Circles();
    //Flag();
    //Q1();
    //Q2();
    Q3();
    //Q4();
    //Q5();
}
