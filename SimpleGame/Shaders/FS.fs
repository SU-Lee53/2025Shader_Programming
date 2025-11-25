#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_UV;
uniform float u_Time;

uniform int u_Digit;

uniform sampler2D u_RGBTexture;
uniform sampler2D u_DigitTexture;
uniform sampler2D u_NumTexture;

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

void Brick_Horizontal()
{
	vec2 newUV = vec2(v_UV.x, v_UV.y);      // 0 ~ 1, left top (0,0)
    
    //float x = (newUV.x * 2) - ceil(v_UV.y * 2) / 2;
    //float y = newUV.y * 2;
    
    float x = fract(newUV.x * 2) + floor(newUV.y * 2) * 0.5;
    float y = fract(newUV.y * 2);

	vec4 sampledColor = texture(u_RGBTexture, vec2(x, y));
    
	FragColor = sampledColor;
}

void Brick_Vertical()
{
	vec2 newUV = vec2(v_UV.x, v_UV.y);      // 0 ~ 1, left top (0,0)

    float x = fract(newUV.x * 2);
    float y = fract(newUV.y * 2) - floor(newUV.x * 2) * 0.5;    // -0.5 ~ 1.5

	vec4 sampledColor = texture(u_RGBTexture, vec2(x, y));
    
	FragColor = sampledColor;
}

void Brick_Horizontal2()
{
	vec2 newUV = vec2(v_UV.x, v_UV.y);      // 0 ~ 1, left top (0,0)
    float rCount = 2;
    float sAmount = 0.5;

    float x = fract(newUV.x * rCount) + floor(newUV.y * rCount + 1) * sAmount;
    float y = fract(newUV.y * rCount);

	vec4 sampledColor = texture(u_RGBTexture, vec2(x, y));
    
	FragColor = sampledColor;
}

void Digit()
{
    FragColor = texture(u_DigitTexture, v_UV);
}

void Digit_Num()
{
    vec2 newUV = v_UV;

    int digit = int(u_Time) % 10;
    int tileIndex = (digit + 9) % 10;

    float offX = float(tileIndex % 5) / 5;
    float offY = floor(float(tileIndex) / 5) / 2;

    float x = (newUV.x * 0.2) + offX;
    float y = (newUV.y * 0.5) + offY;

	vec4 sampledColor = texture(u_NumTexture, vec2(x, y));
	FragColor = sampledColor;
}

void Digit_Num_AI()
{
    // 표시할 값 (0~99999)
    int value = int(u_Time) % 100000;

    // 5자리 분해
    int digits[5];
    digits[0] = (value / 10000) % 10;
    digits[1] = (value / 1000)  % 10;
    digits[2] = (value / 100)   % 10;
    digits[3] = (value / 10)    % 10;
    digits[4] = value % 10;

    // 한 자리의 화면 폭
    float digitWidth = 1.0 / 5.0;

    // 현재 픽셀이 어느 자리인지
    int index = int(v_UV.x / digitWidth);
    index = clamp(index, 0, 4);

    // 로컬 UV (0~1)
    float localX = (v_UV.x - float(index) * digitWidth) * 5.0;
    vec2 localUV = vec2(localX, v_UV.y);

    // 이 픽셀이 보여줄 숫자
    int digit = digits[index];

    // 스프라이트 타일 인덱스 (배치: 1~5 / 6~9,0)
    int tileIndex = (digit + 9) % 10;

    // 타일 좌표 계산
    float offX = float(tileIndex % 5) / 5.0;
    float offY = floor(float(tileIndex) / 5.0) / 2.0;

    float sx = localUV.x * 0.2 + offX;
    float sy = localUV.y * 0.5 + offY;

    // 최종 색상 출력
    FragColor = texture(u_NumTexture, vec2(sx, sy));
}

void main()
{
    Circles();
    //Flag();
    //Q1();
    //Q2();
    //Q3();
    //Brick_Horizontal();
    //Brick_Vertical();
    //Brick_Horizontal2();
    //Digit();
    //Digit_Num();
    //Digit_Num_AI();
}
