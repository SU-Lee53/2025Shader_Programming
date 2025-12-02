#version 330

layout(location=0) out vec4 FragColor;

uniform sampler2D u_TexID;
uniform sampler2D u_TexID1;
uniform int u_Method;       // 0 : normal, 1 : BlurH, 2 : BlurV, 3 : Merge
uniform float u_Time;

in vec2 v_Tex;

const float c_PI = 3.141592f;
const float u_Intensity = 5.f;     // 색수차 강도 (필요 시 추가)
const float weight[5]
    = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216); // Gaussian Kernel

vec4 ChromaticAberration()
{
    // 기본 텍스처 좌표
    vec2 uv = vec2(v_Tex.x, 1.0 - v_Tex.y);

    float fNoise = noise1(u_Time * sin(c_PI) * 2) * 10.f;

    // 색별 오프셋
    float rOffset =  fNoise * 0.002;
    float gOffset =  0.0;                 // 중심 채널 유지
    float bOffset = -fNoise * 0.002;

    // 좌우로 번지는 형태 (radial 방식도 가능)
    vec2 dir = normalize(uv - vec2(0.5)); // 중심에서 방향 (근거 부족 가능성 → 단순 radial 방식)
    if(!all(greaterThan(abs(dir), vec2(0.0))))
        dir = vec2(1.0, 0.0);             // 중심 픽셀 보정

    vec2 uvR = uv + dir * rOffset;
    vec2 uvG = uv + dir * gOffset;
    vec2 uvB = uv + dir * bOffset;

    float r = texture(u_TexID, uvR).r;
    float g = texture(u_TexID, uvG).g;
    float b = texture(u_TexID, uvB).b;

    return vec4(r, g, b, 1.0);
}

vec4 CRTScanline()
{
    vec2 uv = vec2(v_Tex.x, 1.0 - v_Tex.y);

    // 원본 색
    vec4 baseColor = texture(u_TexID, uv);

    // 스캔라인 밀도 (화면 해상도에 맞춰 조절)
    // 값이 클수록 라인이 촘촘해짐
    float density = 150.0;   // 근거 있는 표준값은 아님(특정 CRT 느낌을 모사한 임의 값)

    // 스캔라인 강도
    float intensity = 0.5;  // 0~1 범위 권장

    // uv.y(세로 위치)에 따라 주기적 어두워짐 생성
    float line = sin(uv.y * density * 3.141592 * 2.0);

    // sin 범위 (-1~1)을 0~1로 변환
    line = (line * 0.5) + 0.5;

    // 스캔라인은 '어두워지는' 패턴이므로 반전 적용
    float scan = mix(1.0 - intensity, 1.0, line);

    return vec4(baseColor.rgb * scan, baseColor.a);
}

vec4 Pixelization()
{
    float fResolution = (sin(u_Time) + 1) * 100;

    float tx = floor(v_Tex.x * fResolution) / fResolution; // 0~1 -> 0, 0.2, 0.4, ...
    float ty = floor(v_Tex.y * fResolution) / fResolution;
    
    return texture(u_TexID, vec2(tx, ty));
}

vec4 BlurH()
{             
    vec2 tex_offset = 1.0 / textureSize(u_TexID, 0); // gets size of single texel
    vec3 result = texture(u_TexID, v_Tex).rgb * weight[0]; // current fragment's contribution

    for(int i = 1; i < 5; ++i)
    {
        result += texture(u_TexID, v_Tex + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        result += texture(u_TexID, v_Tex - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
    }

    return vec4(result, 1.0);
}

vec4 BlurV()
{             
    vec2 tex_offset = 1.0 / textureSize(u_TexID, 0); // gets size of single texel
    vec3 result = texture(u_TexID, v_Tex).rgb * weight[0]; // current fragment's contribution

    for(int i = 1; i < 5; ++i)
    {
        result += texture(u_TexID, v_Tex + vec2(0.f, tex_offset.y * i)).rgb * weight[i];
        result += texture(u_TexID, v_Tex - vec2(0.f, tex_offset.y * i)).rgb * weight[i];
    }

    return vec4(result, 1.0);
}

vec4 Merge()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(u_TexID, vec2(v_Tex.x, 1 - v_Tex.y)).rgb;      
    vec3 bloomColor = texture(u_TexID1, v_Tex).rgb;
    hdrColor += bloomColor; 

    // tone mapping
    float fExposure = 3.0;
    vec3 result = vec3(1.0) - exp(-hdrColor * fExposure);

    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    return vec4(result, 1.0);
} 

void main()
{
    FragColor = vec4(0);

    if(u_Method == 0)
    {
        FragColor = texture(u_TexID, vec2(v_Tex.x, 1 - v_Tex.y));
    }
    else if(u_Method == 1)
    {
        FragColor = BlurH();
    }
    else if(u_Method == 2)
    {
        FragColor = BlurV();
    }
    else if(u_Method == 3)
    {
        FragColor = Merge();
    }

	//FragColor = ChromaticAberration();
    //FragColor *= CRTScanline();
    //FragColor *= Pixelization();
}
