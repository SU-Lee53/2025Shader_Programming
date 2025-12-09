#version 330

layout(location=0) out vec4 FragColor;
layout(location=1) out vec4 FragColor1;

in vec2 v_UV;
uniform float u_Time;

uniform int u_Digit;

uniform sampler2D u_RGBTexture;
uniform sampler2D u_DigitTexture;
uniform sampler2D u_NumTexture;

const float c_PI = 3.141592f;

vec4 GaussianBlur()
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

    vec4 finalColor = vec4(result, 1.0);
    return finalColor;
}

vec4 Test()
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

	return sampledColor;
}

vec4 Circles()
{
	vec2 newUV = v_UV;      // 0 ~ 1, left top (0,0)
    vec2 center = vec2(0.5, 0.5);
    vec4 newColor = vec4(0);
    float d = distance(newUV, center);

    float value = sin(d  * 4 * c_PI * 4 - u_Time);   // 0 ~ 1
    newColor = vec4(value);
    
	return newColor;
}

vec4 Sin()
{
    vec2 newUV = vec2(v_UV.x, v_UV.y - 0.5f);
    vec2 center = vec2(0.5, 0.5);
    vec4 newColor = vec4(0);
    float sinValue = sin(v_UV.x * 4 * c_PI + u_Time) * 0.5 ;
    if(distance(newUV.y, sinValue) < 0.05 && newUV.y > sinValue){
        newColor = vec4(1);
    }

    return newColor;
}

vec4 Flag()
{
	vec2 newUV = vec2(v_UV.x, 1 - v_UV.y - 0.5);      // 0 ~ 1, left middle (0,0)
    vec4 newColor = vec4(0);
   
    // sin 곡선을 따라 꿀렁이는 두께있는 선
    float width = 0.2 * (1 - newUV.x);  // (1 - newUV.x) : 끝부분을 날카롭게
    float sinValue = 0.2 * sin(newUV.x * 2 * c_PI - u_Time) * newUV.x;   // * newUV.x : 왼쪽을 고정

    if (newUV.y < sinValue + width &&
        newUV.y > sinValue - width) {
        newColor = vec4((sinValue + 1) / 2);
    }
    else{
        newColor = vec4(0,0,0,1);
    }
    
	return newColor;
}

// 좌표꼬기 시작
// 시험문제 무조건 나옴

vec4 Q1()
{
	vec2 newUV = vec2(v_UV.x, v_UV.y);      // 0 ~ 1, left top (0,0)
    
    float x = newUV.x;  // 0 ~ 1
    float y = 1 - abs((newUV.y - 0.5) * 2); // 0 ~ 1 ~ 0

	vec4 sampledColor = texture(u_RGBTexture, vec2(x, y));
    
	return sampledColor;
}

vec4 Q2()
{
	vec2 newUV = vec2(v_UV.x, v_UV.y);      // 0 ~ 1, left top (0,0)
    
    float x = fract(newUV.x * 3);   // 0~1 0~1 0~1
    float y = (2 - floor(newUV.x * 3)) / 3 + newUV.y / 3;

	vec4 sampledColor = texture(u_RGBTexture, vec2(x, y));
    
	return sampledColor;
}

vec4 Q3()
{
	vec2 newUV = vec2(v_UV.x, v_UV.y);      // 0 ~ 1, left top (0,0)
    
    float x = fract(newUV.x * 3);
    float y = (2 - floor(newUV.x  * 3)) / 3 + newUV.y / 3;

	vec4 sampledColor = texture(u_RGBTexture, vec2(x, y));
    
	return sampledColor;
}

vec4 Q4()
{
	vec2 newUV = vec2(v_UV.x, v_UV.y);      // 0 ~ 1, left top (0,0)
    
    float x = fract(newUV.x * 3);
    float y = floor(newUV.x * 3) / 3 + newUV.y / 3;

	vec4 sampledColor = texture(u_RGBTexture, vec2(x, y));
    
	return sampledColor;
}

vec4 Q5()
{
    vec2 newUV = vec2(v_UV.x, v_UV.y);
    float x = newUV.x;
    float y = fract(v_UV.y * 3) / 3 + (2 - floor(v_UV.y * 3)) / 3;

    vec4 sampledColor = texture(u_RGBTexture, vec2(x,y));
    return sampledColor;
}


vec4 Brick_Horizontal()
{
	vec2 newUV = vec2(v_UV.x, v_UV.y);      // 0 ~ 1, left top (0,0)
    
    //float x = (newUV.x * 2) - ceil(v_UV.y * 2) / 2;
    //float y = newUV.y * 2;
    
    //float x = fract(newUV.x * 2) + floor(newUV.y * 2) * 0.5;
    float x = fract((newUV.x * 2)) + ((floor(newUV.y * 2) * 2) - 1) * fract(u_Time);
    float y = fract(newUV.y * 2);

	vec4 sampledColor = texture(u_RGBTexture, vec2(x, y));
    
	return sampledColor;
}

vec4 Brick_Vertical()
{
	vec2 newUV = vec2(v_UV.x, v_UV.y);      // 0 ~ 1, left top (0,0)

    float x = fract(newUV.x * 2);
    //float y = fract(newUV.y * 2) - floor(newUV.x * 2) * 0.5;    // -0.5 ~ 1.5
    float y = fract((newUV.y * 2) + 0.5);


	vec4 sampledColor = texture(u_RGBTexture, vec2(x, y));
    
	return sampledColor;
}

vec4 Brick_Horizontal2()
{
	vec2 newUV = vec2(v_UV.x, v_UV.y);      // 0 ~ 1, left top (0,0)
    float rCount = 2;
    float sAmount = 0.5;

    float x = fract(newUV.x * rCount) + floor(newUV.y * rCount + 1) * sAmount;
    float y = fract(newUV.y * rCount);

	vec4 sampledColor = texture(u_RGBTexture, vec2(x, y));
    
	return sampledColor;
}

vec4 Digit()
{
    return texture(u_DigitTexture, v_UV);
}

vec4 Digit_Num()
{
    vec2 newUV = v_UV;

    int digit = int(u_Time) % 10;
    int tileIndex = (digit + 9) % 10;

    float offX = float(tileIndex % 5) / 5;
    float offY = floor(float(tileIndex) / 5) / 2;

    float x = (newUV.x * 0.2) + offX;
    float y = (newUV.y * 0.5) + offY;

	vec4 sampledColor = texture(u_NumTexture, vec2(x, y));
	return sampledColor;
}

vec4 Digit_Num_AI()
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
    vec4 sampledColor = texture(u_NumTexture, vec2(sx, sy));
	return sampledColor;
}

vec4 P1()
{
    vec2 newUV = vec2(v_UV.x, v_UV.y);

    float x = fract(v_UV.x * 3);
    float y = v_UV.y + floor(v_UV.x * 3) / 3;

    vec4 sampledColor = texture(u_RGBTexture, vec2(x,y));
    return sampledColor;
}

vec4 P2()
{
    vec2 newUV = vec2(v_UV.x, v_UV.y);
    vec2 center = vec2(0.5, 0.5);
    float dist = distance(newUV, center);

    vec4 sampledColor = texture(u_RGBTexture, v_UV) * 1 - round(dist);
    return sampledColor;
}

vec4 P3()
{
    vec2 newUV = vec2(v_UV.x, v_UV.y);

    float x = fract(v_UV.x * 2);
    float y = fract(v_UV.y * 2);
    

    vec4 sampledColor = texture(u_RGBTexture, vec2(x,y));
    return sampledColor;
}

vec4 P4()
{
    vec2 newUV = vec2(v_UV.x, v_UV.y);

    float x =  v_UV.y;
    float y = 1 - v_UV.x;
    

    vec4 sampledColor = texture(u_RGBTexture, vec2(x,y));
    return sampledColor;
}

vec4 P5()
{
    vec2 newUV = vec2(v_UV.x, v_UV.y);

    float x = newUV.x * 2 - 0.5;
    float y = newUV.y * 2;

    vec4 sampledColor = vec4(0);
    if(x > 0 && x < 1)
    {
        sampledColor = texture(u_RGBTexture, vec2(x,y));
    }
    return sampledColor;
}

vec4 P6()
{
    vec2 newUV = vec2(v_UV.x, v_UV.y);

    float shear = 0.06;

    float x = newUV.x - shear * floor(newUV.y * 15);
    float y = newUV.y;
    
    vec4 sampledColor = texture(u_RGBTexture, vec2(x,y));
    return sampledColor;
}

vec4 P7()
{
    vec2 newUV = vec2(v_UV.x, v_UV.y);

    float x = sign((fract(newUV.y * 4) - 0.5) * 2) * abs((fract(newUV.x * 4) - 0.5) * 2);
    float y = fract(newUV.y * 8);

    vec4 sampledColor = texture(u_RGBTexture, vec2(x,y));
    //vec4 sampledColor = vec4(x, 0, 0, 1);
    return sampledColor;
}

vec4 P8()
{
    vec2 newUV = vec2(v_UV.x, v_UV.y);

    float x = 1 - abs((fract(v_UV.x) - 0.5) * 2);
    float y = 1 - abs((fract(v_UV.y) - 0.5) * 2);
    
    vec4 sampledColor = texture(u_RGBTexture, vec2(x,y));
    return sampledColor;
}

vec4 P9()
{
    vec2 newUV = v_UV;

    float x = fract(v_UV.x * 4);
    float y = v_UV.y;
    
    vec4 sampledColor = texture(u_RGBTexture, vec2(x,y));
    return sampledColor;
}

vec4 P10()
{
    vec2 newUV = v_UV;

    float x = abs(fract(v_UV.x * 4.0) * 2.0 - 1.0);
    float y = abs(fract(v_UV.y * 4.0) * 2.0 - 1.0);

    vec4 sampledColor = texture(u_RGBTexture, vec2(x,y));
    //vec4 sampledColor = vec4(x, 0, 0, 1);
    return sampledColor;
}

vec4 FlagTest()
{
    vec2 newUV = vec2(v_UV.x, v_UV.y - 0.5);

    float width = 0.4 * (1 - newUV.x);
    float amp = 0.2;
    float sinValue = amp * sin(newUV.x * 4 * c_PI - u_Time) * newUV.x;

    vec4 newColor = vec4(0);

    if(newUV.y > sinValue - width && newUV.y < sinValue + width){
        newColor = vec4((sinValue + 1) / 2);
    }

    return newColor;
}

vec4 WaterDrop()
{
    vec2 newUV = v_UV;
    vec4 newColor = vec4(0);
    vec2 center = vec2(0.5, 0.5);
    float dist = distance(center, newUV);

    float sinValue = (sin(dist * 4 * c_PI - u_Time) + 1) / 2;
    newColor = vec4(sinValue);

    return newColor;
}

vec4 P11()
{
    vec2 newUV = v_UV;
    
    float x = fract(v_UV.x * 3);
    float y = v_UV.y;

    vec4 sampledColor = texture(u_RGBTexture, vec2(x,y));
    return sampledColor;
}

vec4 P12()
{
    vec2 newUV = v_UV;
    
    float x = 1 - abs(fract(v_UV.x) - 0.5) * 2;
    float y = v_UV.y;

    vec4 sampledColor = texture(u_RGBTexture, vec2(x,y));
    return sampledColor;
}

vec4 P13()
{
    vec2 newUV = v_UV;
    
    float x = newUV.x;
    float y = fract(newUV.y * 3);

    vec4 sampledColor = texture(u_RGBTexture, vec2(x,y));
    return sampledColor;
}

vec4 P14()
{
    vec2 newUV = v_UV;
    
    float x = 1 - abs(fract(v_UV.x * 1.5) - 0.5) * 2;
    float y = 1 - abs(fract(v_UV.y * 1.5) - 0.5) * 2;

    vec4 sampledColor = texture(u_RGBTexture, vec2(x,y));
    return sampledColor;
}

vec4 P15()
{
    vec2 newUV = v_UV;
    
    float x = fract(v_UV.x * 4);
    float y = v_UV.y + floor(v_UV.x * 4) * 0.25;

    vec4 sampledColor = texture(u_RGBTexture, vec2(x,y));
    return sampledColor;
}

vec4 P16()
{
    vec2 newUV = v_UV;
    
    float x = v_UV.x;
    float y = -abs(fract(v_UV.y * 1.5) - 0.5) * 2;

    vec4 sampledColor = texture(u_RGBTexture, vec2(x,y));
    return sampledColor;
}

vec4 P17()
{
    vec2 newUV = v_UV;
    
    float x = fract(v_UV.x * 3);
    float y = floor(v_UV.x * 3) / 3 + v_UV.y / 3;


    vec4 sampledColor = texture(u_RGBTexture, vec2(x,y));
    return sampledColor;
}

vec4 BrickTestV()
{
    vec2 newUV = v_UV;

    float x = fract(newUV.x * 2) + (1 - floor(newUV.y * 2)) * 0.5;
    float y = fract(newUV.y * 2);

    vec4 sampledColor = texture(u_RGBTexture, vec2(x,y));
    return sampledColor;
}

vec4 BrickTestH()
{
    vec2 newUV = v_UV;
    
    float x = -sign(mod(floor(v_UV.x * 4), 2) - 0.5) * fract(v_UV.x * 4);
    float y = 1 - abs(fract(newUV.y * 2) - 0.5) * 2 + floor(newUV.x * 4) * 0.33333333333333;

    vec4 sampledColor = texture(u_RGBTexture, vec2(x,y));
    return sampledColor;
}

vec4 Checker()
{
    vec2 newUV = v_UV;

    float x = sin(v_UV.x * 8 * c_PI);
    float y = cos(v_UV.y * 8 * c_PI);
    vec3 rgb = vec3(x + y);

    vec4 newColor = vec4(rgb, 1.f);
    return newColor;
}

vec4 Wave()
{
    vec2 newUV = vec2(v_UV.x, v_UV.y - 0.5);
    vec4 newColor = vec4(0,0,0,1);
    float sinValue = 0.5 * sin(v_UV.x * 4 * c_PI);
    float width = 0.01;

    if(newUV.y > sinValue - width && newUV.y < sinValue){
        newColor = vec4(1);
    }

    return newColor;
}

vec4 Circle()
{
    vec2 newUV = v_UV;
    vec2 center = vec2(0.5f, 0.5f);
    float dist = distance(center, newUV);
    float radius = 0.25f;
    float width = 0.01;
    
    float sinValue = sin(dist * 4 * c_PI - u_Time);

    float gradient = sin((dist * 2) * c_PI);
    gradient = pow(gradient, 32);

    vec4 newColor = vec4(sinValue * gradient);


    return newColor;
}

void main()
{
    //Circles();
    //Flag();
    //Q1();
    //Q2();
    //Q3();
    //Brick_Horizontal();
    //Brick_Vertical();
    //Brick_Horizontal2();
    //Digit();
    //Digit_Num();

    FragColor = Circle();
    //FragColor1 = Flag();
}
