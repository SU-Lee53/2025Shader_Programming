#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <cassert>
#include <vector>


Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}

Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	// Compile all shader programs
	CompileAllShaderPrograms();

	//Create VBOs
	CreateVertexBufferObjects();

	// Create Grid Mesh (10.13)
	//CreateGridMesh(1000, 1000);
	CreateGridMesh(200, 200);	// 1000 이 깔이 좋지만 너무 느려서 줄임

	GenerateParticles(10000);
	
	// 11.17
	m_RGBTexture = CreatePngTexture("./rgb.png", GL_NEAREST);
	m_YTTexture = CreatePngTexture("./Youtube_logo.png", GL_NEAREST);

	m_0Texture = CreatePngTexture("./0.png", GL_NEAREST);
	m_1Texture = CreatePngTexture("./1.png", GL_NEAREST);
	m_2Texture = CreatePngTexture("./2.png", GL_NEAREST);
	m_3Texture = CreatePngTexture("./3.png", GL_NEAREST);
	m_4Texture = CreatePngTexture("./4.png", GL_NEAREST);
	m_5Texture = CreatePngTexture("./5.png", GL_NEAREST);
	m_6Texture = CreatePngTexture("./6.png", GL_NEAREST);
	m_7Texture = CreatePngTexture("./7.png", GL_NEAREST);
	m_8Texture = CreatePngTexture("./8.png", GL_NEAREST);
	m_9Texture = CreatePngTexture("./9.png", GL_NEAREST);
	m_NumTexture = CreatePngTexture("./numbers.png", GL_NEAREST);

	// 12.02
	m_ParticleTexture = CreatePngTexture("./particle.png", GL_NEAREST);

	// 11.25
	// Create FBOs
	CreateFBOs();

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}

	// 10.21 빗방울 위치
	for (int i = 0; i < 100; i += 4) {
		float x = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		float y = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		float st = ((float)rand() / (float)RAND_MAX) * 5;
		float lt = ((float)rand() / (float)RAND_MAX) * 1;

		m_Points[i] = x;
		m_Points[i + 1] = y;
		m_Points[i + 2] = st;
		m_Points[i + 3] = lt;

	}

}

void Renderer::CompileAllShaderPrograms()
{
	//Load shaders
	m_SolidRectShader = CompileShaders(
		"./Shaders/SolidRect.vs",
		"./Shaders/SolidRect.fs");

	m_TestShader = CompileShaders(
		"./Shaders/Test.vs",
		"./Shaders/Test.fs");

	m_ParticleShader = CompileShaders(
		"./Shaders/Particle.vs",
		"./Shaders/Particle.fs");

	m_GridMeshShader = CompileShaders(
		"./Shaders/GridMesh.vs",
		"./Shaders/GridMesh.fs");

	m_FullScreenShader = CompileShaders(
		"./Shaders/FullScreen.vs",
		"./Shaders/FullScreen.fs");

	m_FSShader = CompileShaders(
		"./Shaders/FS.vs",
		"./Shaders/FS.fs");

	m_TexShader = CompileShaders(
		"./Shaders/Texture.vs",
		"./Shaders/Texture.fs");

}

void Renderer::DeleteAllShaderPrograms()
{
	glDeleteShader(m_SolidRectShader);
	glDeleteShader(m_TestShader);
	glDeleteShader(m_ParticleShader);
	glDeleteShader(m_GridMeshShader);
	glDeleteShader(m_FullScreenShader);
	glDeleteShader(m_FSShader);
	glDeleteShader(m_TexShader);
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::ReloadAllShaderPrograms()
{
	DeleteAllShaderPrograms();
	CompileAllShaderPrograms();
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	// Lecture 2 (09.08)
	float temp = 0.5f;
	float size = 0.1f;
	float testPos[]
		=
	{
		 (0.f - temp) * size,	 (0.f - temp) * size,	0.f,	0.5f,
		 (1.f - temp) * size,	 (0.f - temp) * size,	0.f,	0.5f,
		 (1.f - temp) * size,	 (1.f - temp) * size,	0.f,	0.5f,	
		 (0.f - temp) * size,	 (0.f - temp) * size,	0.f,	0.5f,
		 (1.f - temp) * size,	 (1.f - temp) * size,	0.f,	0.5f,
		 (0.f - temp) * size,	 (1.f - temp) * size,	0.f,	0.5f,	// Quad1(09.22)

		 (0.f - temp) * size,	 (0.f - temp) * size,	0.f,	1.0f,
		 (1.f - temp) * size,	 (0.f - temp) * size,	0.f,	1.0f,
		 (1.f - temp) * size,	 (1.f - temp) * size,	0.f,	1.0f,	
		 (0.f - temp) * size,	 (0.f - temp) * size,	0.f,	1.0f,
		 (1.f - temp) * size,	 (1.f - temp) * size,	0.f,	1.0f,
		 (0.f - temp) * size,	 (1.f - temp) * size,	0.f,	1.0f	// Quad 2 (09.22)
	};

	glGenBuffers(1, &m_VBOTestPos);	// GPU가 VBO를 만들고 그 ID 를 testID 에 보관 -> ID 만 생성하고 GPU 메모리는 할당하지 않음
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestPos);
	/*
	    + --------------- +
		| GL_ARRAY_BUFFER | 
	    + --------------- +
	    |     testID      | <- glBindBuffer() 의 결과로 testID 의 Buffer Object 가 이런식으로 구성됨
		+ --------------- +

		나중에 VBO를 찾아갈 때 GL_ARRAY_BUFFER 안에 있는 testID를 찾아 리소스를 read/write 하게됨
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(testPos), testPos, GL_STATIC_DRAW); 
	// Bind된 VBO(testID) 에 데이터를 할당함 -> 이 함수의 호출로 실제 메모리가 VRAM에 잡힘
	// 함수 특성상 Async 하고 시간이 오래 걸릴 수 있음

	// Lecture 3 (09.09)
	// 정점 색상 추가
	// 채널별 RGBA 0.f ~ 1.0 사이의 값을 가짐
	float testColor[]
		=
	{
		 1.f,	0.f,	0.f,	1.f,
		 0.f,	1.f,	0.f,	1.f,
		 0.f,	0.f,	1.f,	1.f,
		 1.f,	0.f,	0.f,	1.f,
		 0.f,	1.f,	0.f,	1.f,
		 0.f,	0.f,	1.f,	1.f,	// Quad 1 (09.22)

		 1.f,	0.f,	0.f,	1.f,
		 0.f,	1.f,	0.f,	1.f,
		 0.f,	0.f,	1.f,	1.f,
		 1.f,	0.f,	0.f,	1.f,
		 0.f,	1.f,	0.f,	1.f,
		 0.f,	0.f,	1.f,	1.f		// Quad 2 (09.22)
	};

	glGenBuffers(1, &m_VBOTestColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(testColor), testColor, GL_STATIC_DRAW);


	// 10.14 FullScreen Rect
	float fullRect[]
		=
	{
		-1.f, -1.f, 0.f, -1.f, 1.f, 0.f, 1.f, 1.f, 0.f, //Triangle1
		-1.f, -1.f, 0.f,  1.f, 1.f, 0.f, 1.f, -1.f, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBOFullScreen);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFullScreen);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fullRect), fullRect, GL_STATIC_DRAW);


	float FSRect[]
		=
	{
		-1.f , -1.f , 0.f,
		-1.f ,1.f , 0.f,
		1.f , 1.f , 0.f, //Triangle1
		-1.f , -1.f , 0.f,
		1.f , 1.f , 0.f,
		1.f , -1.f , 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBOFS);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFS);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fullRect), fullRect, GL_STATIC_DRAW);

	// m_TexVBO
	float texRect[]
		=
	{
		-1.f, -1.f, 0.f, 0.f, 1.f,
		1.f, 1.f, 0.f, 1.f, 0.f,
		-1.f, 1.f, 0.f, 0.f, 0.f,	//Triangle1

		-1.f, -1.f, 0.f, 0.f, 1.f,
		1.f, -1.f, 0.f, 1.f, 1.f,
		1.f, 1.f, 0.f,	1.f, 0.f//Triangle2
	};

	glGenBuffers(1, &m_TexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_TexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texRect), texRect, GL_STATIC_DRAW);
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];

	size_t slen = strlen(pShaderText);
	if (slen > INT_MAX) {
		// Handle error
	}
	GLint len = (GLint)slen;

	Lengths[0] = len;
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawTest()
{
	//Program select
	glUseProgram(m_TestShader);

	// Lecture 4 (09.16)
	m_Time += 0.00016;
	int uTimeLoc = glGetUniformLocation(m_TestShader, "u_Time");
	glUniform1f(uTimeLoc, m_Time);

	// m_TestShader 프로그램에서 a_Position 이라는 Attribute Location 을 가져와라
	int aPosLoc = glGetAttribLocation(m_TestShader, "a_Position");
	// 직접 Get 하기 싫다면 layout(location = n) 을 사용하고
	// glEnableVertexAttribArray(n); 을 사용한다

	// Enable 해야 사용이 가능함(바로 아래 Bind, AttribPorinter 등)
	glEnableVertexAttribArray(aPosLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestPos);	// Bind 해주어야 아래 AttribPointer 같은 작업이 가능
	glVertexAttribPointer(
		aPosLoc, 3, GL_FLOAT, 
		GL_FALSE, sizeof(float) * 4, 0); // VBO 를 어떻게 해석할지 정보를 지정함
	// a_Radius 가 추가된 상황에서 size 는 3이 맞이 맞음
	// 다만 Stride 는 다음 정점의 시작위치를 알려면 sizeof(float) * 4 만큼 이동해야 하므로 변하지 않음

	// Lecture 6 (09.22)
	int aRadiusLoc = glGetAttribLocation(m_TestShader, "a_Radius");
	glEnableVertexAttribArray(aRadiusLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestPos);	// Bind 해주어야 아래 AttribPointer 같은 작업이 가능
	glVertexAttribPointer(
		aRadiusLoc, 1, GL_FLOAT,
		GL_FALSE, sizeof(float) * 4, 
		(GLvoid*)(sizeof(float) * 3)); // VBO 를 어떻게 해석할지 정보를 지정함

	// [   x   ][   y   ][   z   ][ Radius ] | [   x   ][   y   ][   z   ][ Radius ] 
	// a_Radius 의 경우에도 마찬가지로 stride 는 sizeof(float) * 4
	// 다만 어디서 값을 읽어와야 하는지(마지막 param) 이 달라지는데 float 3개를 건너뛰고(xyz) 읽어오도록 지정함

	// Lecture 3 (09.09)
	int aColorLoc = glGetAttribLocation(m_TestShader, "a_Color");
	glEnableVertexAttribArray(aColorLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestColor);
	glVertexAttribPointer(
		aColorLoc, 4, GL_FLOAT,
		GL_FALSE, sizeof(float) * 4, 0);	// size 와 stride 가 바뀌어야 함
	glDrawArrays(GL_TRIANGLES, 0, 12);

	glDisableVertexAttribArray(aPosLoc);
	glDisableVertexAttribArray(aColorLoc);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawParticle()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);

	GLuint shader = m_ParticleShader;
	glUseProgram(shader);

	GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, DrawBuffers);

	m_Time += 0.0016;
	int uTimeLoc = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTimeLoc, m_Time);

	int uForceLoc = glGetUniformLocation(shader, "u_Force");
	glUniform3f(uForceLoc, std::sin(m_Time) * 10.f, 0, 0);

	int uTexLoc = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(uTexLoc, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ParticleTexture);

	int aPosLoc = glGetAttribLocation(shader, "a_Position");
	int aValueLoc = glGetAttribLocation(shader, "a_Value");
	int aColorLoc = glGetAttribLocation(shader, "a_Color");
	int aSTimeLoc = glGetAttribLocation(shader, "a_STime");
	int aVelocityLoc = glGetAttribLocation(shader, "a_Velocity");
	int aLTimeLoc = glGetAttribLocation(shader, "a_LifeTime");
	int aMassLoc = glGetAttribLocation(shader, "a_Mass");
	int aPeriodLoc = glGetAttribLocation(shader, "a_Period");
	int aTexLoc = glGetAttribLocation(shader, "a_Tex");

	glEnableVertexAttribArray(aPosLoc);
	glEnableVertexAttribArray(aValueLoc);
	glEnableVertexAttribArray(aColorLoc);
	glEnableVertexAttribArray(aSTimeLoc);
	glEnableVertexAttribArray(aVelocityLoc);
	glEnableVertexAttribArray(aLTimeLoc);
	glEnableVertexAttribArray(aMassLoc);
	glEnableVertexAttribArray(aPeriodLoc);
	glEnableVertexAttribArray(aTexLoc);

	int stride = 17;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticle);

	glVertexAttribPointer(
		aPosLoc, 3, GL_FLOAT,
		GL_FALSE, sizeof(float) * stride, 0);

	glVertexAttribPointer(
		aValueLoc, 1, GL_FLOAT,
		GL_FALSE, sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 3)); 

	glVertexAttribPointer(
		aColorLoc, 4, GL_FLOAT,
		GL_FALSE, sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 4));	

	glVertexAttribPointer(
		aSTimeLoc, 1, GL_FLOAT,
		GL_FALSE, sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 8));

	glVertexAttribPointer(
		aVelocityLoc, 3, GL_FLOAT,
		GL_FALSE, sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 9));

	glVertexAttribPointer(
		aLTimeLoc, 1, GL_FLOAT,
		GL_FALSE, sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 12));

	glVertexAttribPointer(
		aMassLoc, 1, GL_FLOAT,
		GL_FALSE, sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 13));

	glVertexAttribPointer(
		aPeriodLoc, 1, GL_FLOAT,
		GL_FALSE, sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 14));

	glVertexAttribPointer(
		aTexLoc, 2, GL_FLOAT,
		GL_FALSE, sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 15));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOParticleVertexCount/* - (6 * 1000)*/);
	//glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPosLoc);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glDisable(GL_BLEND);
}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

// Lecture 5 (09.22)
void Renderer::GenerateParticles(int numParticles)
{
	// (x, y, z), (value), (r, g, b, a), (sTime), (vx, vy, vz), (lifeTime), (mass), (period) + (tx, ty)
	int floatCountPerVertex = 3 + 1 + 4 + 1 + 3 + 1 + 1 + 1 + 2;
	int verticesCountPerParticle= 6;
	int floatCountPerParticle = floatCountPerVertex * verticesCountPerParticle;
	int totalVerticesCount = numParticles * verticesCountPerParticle;
	int totalFloatCount = floatCountPerVertex * totalVerticesCount;

	m_floatCountPerVertex = floatCountPerVertex;

	float* pVertices = new float[totalFloatCount];
	if (!pVertices) {
		return;
	}

	for (int i = 0; i < numParticles; ++i) {
		float x, y, z, value, r, g, b, a;
		x = 0.f;  // ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;	// -1.f ~ 1.f
		y = 0.f; // ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;	// -1.f ~ 1.f
		z = 0.f;  // 0.f;
		value = (float)rand() / (float)RAND_MAX;
		r = (float)rand() / (float)RAND_MAX;
		g = (float)rand() / (float)RAND_MAX;
		b = (float)rand() / (float)RAND_MAX;
		a = (float)rand() / (float)RAND_MAX;

		float size;
		size = ((float)rand() / (float)RAND_MAX) * 0.01;	// 0~1 은 너무 크므로 적당히 줄임

		float sTime = ((float)rand() / (float)RAND_MAX) * 2.f;

		// 시작속도 적용 (09.29)
		float vx, vy, vz;
		vx = 0;	//	(((float)rand() / (float)RAND_MAX) * 2.f - 1.f) * 1.5f;	
		vy = 0;	//	((((float)rand() / (float)RAND_MAX) * 2.f - 1.f) + 1.5f) * 1.5f;
		vz = 0.f;

		// 개별적인 LifeTime (09.29)
		float fLifeTime;
		fLifeTime = ((float)rand() / (float)(RAND_MAX));

		// 질량 (09.29)
		float fMass;
		fMass = ((float)rand() / (float)(RAND_MAX)) + 1.f;
		
		// 주기 (09.30)
		float fPeriod;
		fPeriod = ((float)rand() / (float)(RAND_MAX)) + 1.f;

		int index = i * floatCountPerParticle;
		pVertices[index] = x - size;	index++;	// v1 좌하단
		pVertices[index] = y - size;	index++;
		pVertices[index] = z;			index++;
		pVertices[index] = value;		index++;
		pVertices[index] = r;			index++;
		pVertices[index] = g;			index++;
		pVertices[index] = b;			index++;
		pVertices[index] = a;			index++;
		pVertices[index] = sTime;		index++;
		pVertices[index] = vx;			index++;
		pVertices[index] = vy;			index++;
		pVertices[index] = vz;			index++;
		pVertices[index] = fLifeTime;	index++;
		pVertices[index] = fMass;		index++;
		pVertices[index] = fPeriod;		index++;
		pVertices[index] = 0;		index++;
		pVertices[index] = 1;		index++;
		
		pVertices[index] = x + size;	index++;	// v2 우상단
		pVertices[index] = y + size;	index++;
		pVertices[index] = z;			index++;
		pVertices[index] = value;		index++;
		pVertices[index] = r;			index++;
		pVertices[index] = g;			index++;
		pVertices[index] = b;			index++;
		pVertices[index] = a;			index++;
		pVertices[index] = sTime;		index++;
		pVertices[index] = vx;			index++;
		pVertices[index] = vy;			index++;
		pVertices[index] = vz;			index++;
		pVertices[index] = fLifeTime;	index++;
		pVertices[index] = fMass;		index++;
		pVertices[index] = fPeriod;		index++;
		pVertices[index] = 1;		index++;
		pVertices[index] = 0;		index++;
		
		pVertices[index] = x - size;	index++;	// v3 좌상단
		pVertices[index] = y + size;	index++;
		pVertices[index] = z;			index++;
		pVertices[index] = value;		index++;
		pVertices[index] = r;			index++;
		pVertices[index] = g;			index++;
		pVertices[index] = b;			index++;
		pVertices[index] = a;			index++;
		pVertices[index] = sTime;		index++;
		pVertices[index] = vx;			index++;
		pVertices[index] = vy;			index++;
		pVertices[index] = vz;			index++;
		pVertices[index] = fLifeTime;	index++;
		pVertices[index] = fMass;		index++;
		pVertices[index] = fPeriod;		index++;
		pVertices[index] = 0;		index++;
		pVertices[index] = 0;		index++;
		
		pVertices[index] = x - size;	index++;	// v4 좌하단
		pVertices[index] = y - size;	index++;
		pVertices[index] = z;			index++;
		pVertices[index] = value;		index++;
		pVertices[index] = r;			index++;
		pVertices[index] = g;			index++;
		pVertices[index] = b;			index++;
		pVertices[index] = a;			index++;
		pVertices[index] = sTime;		index++;
		pVertices[index] = vx;			index++;
		pVertices[index] = vy;			index++;
		pVertices[index] = vz;			index++;
		pVertices[index] = fLifeTime;	index++;
		pVertices[index] = fMass;		index++;
		pVertices[index] = fPeriod;		index++;
		pVertices[index] = 0;		index++;
		pVertices[index] = 1;		index++;
		
		pVertices[index] = x + size;	index++;	// v5 우하단
		pVertices[index] = y - size;	index++;
		pVertices[index] = z;			index++;
		pVertices[index] = value;		index++;
		pVertices[index] = r;			index++;
		pVertices[index] = g;			index++;
		pVertices[index] = b;			index++;
		pVertices[index] = a;			index++;
		pVertices[index] = sTime;		index++;
		pVertices[index] = vx;			index++;
		pVertices[index] = vy;			index++;
		pVertices[index] = vz;			index++;
		pVertices[index] = fLifeTime;	index++;
		pVertices[index] = fMass;		index++;
		pVertices[index] = fPeriod;		index++;
		pVertices[index] = 1;		index++;
		pVertices[index] = 1;		index++;
		
		pVertices[index] = x + size;	index++;	// v6 우상단
		pVertices[index] = y + size;	index++;
		pVertices[index] = z;			index++;
		pVertices[index] = value;		index++;
		pVertices[index] = r;			index++;
		pVertices[index] = g;			index++;
		pVertices[index] = b;			index++;
		pVertices[index] = a;			index++;
		pVertices[index] = sTime;		index++;
		pVertices[index] = vx;			index++;
		pVertices[index] = vy;			index++;
		pVertices[index] = vz;			index++;
		pVertices[index] = fLifeTime;	index++;
		pVertices[index] = fMass;		index++;
		pVertices[index] = fPeriod;		index++;
		pVertices[index] = 1;		index++;
		pVertices[index] = 0;		index++;
	}

	glGenBuffers(1, &m_VBOParticle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCount, pVertices, GL_STATIC_DRAW);

	delete[] pVertices;

	m_VBOParticleVertexCount = numParticles * verticesCountPerParticle;
}

void Renderer::CreateGridMesh(int x, int y)
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;
	
	// 10.21
	//float basePosX = -1.0f;
	//float basePosY = -1.0f;
	//float targetPosX = 1.0f;
	//float targetPosY = 1.0f;

	int pointCountX = x;
	int pointCountY = y;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX * pointCountY * 2];
	float* vertices = new float[(pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3];
	m_GridMeshVertexCount = (pointCountX - 1) * (pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y * pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y * pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}

	glGenBuffers(1, &m_GridMeshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_GridMeshVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);

	delete[] point;
	delete[] vertices;
}

GLuint Renderer::CreatePngTexture(char* filePath, GLuint samplingMethod)
{
	//Load Png
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		std::cout << "PNG image loading failed:" << filePath << std::endl;
		assert(0);
	}

	GLuint temp;
	glGenTextures(1, &temp);
	glBindTexture(GL_TEXTURE_2D, temp);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, &image[0]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMethod);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMethod);

	return temp;
}

void Renderer::CreateFBOs()
{
	for (int i = 0; i < _countof(m_FBOs); ++i) {
		glGenTextures(1, &m_RTs_0[i]);
		glBindTexture(GL_TEXTURE_2D, m_RTs_0[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		// Gen Depth Buffer
		GLuint depthBuffer;
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// Gen FBO
		glGenFramebuffers(1, &m_FBOs[i]);

		// Attach to FBO
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,	// GL_COLOR_ATTACHMENT0 => layout(location = 0)
			GL_TEXTURE_2D, m_RTs_0[i], 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, depthBuffer);

		// Check
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			assert(false);
		}

	}
	
	for (int i = 0; i < _countof(m_FBOs); ++i) {
		glGenTextures(1, &m_RTs_1[i]);
		glBindTexture(GL_TEXTURE_2D, m_RTs_1[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		// Attach to FBO
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,	// GL_COLOR_ATTACHMENT1 => layout(location = 1)
			GL_TEXTURE_2D, m_RTs_1[i], 0);

		// Check
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			assert(false);
		}

	}

	// Restore current frame buffer to original frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &m_HDRRT0_0);
	glBindTexture(GL_TEXTURE_2D, m_HDRRT0_0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);	// GL_RGBA16F -> 채널당16비트 float으로 texture 생성

	glGenTextures(1, &m_HDRRT0_1);
	glBindTexture(GL_TEXTURE_2D, m_HDRRT0_1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);	// GL_RGBA16F -> 채널당16비트 float으로 texture 생성

	// Gen FBO
	glGenFramebuffers(1, &m_HDRFBO0);

	// Gen Depth Buffer
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Attach to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, m_HDRFBO0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, m_HDRRT0_0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, m_HDRRT0_1, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, depthBuffer);

	// Check
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		assert(false);
	}


	glGenFramebuffers(2, m_PingpongFBO);
	glGenTextures(2, m_PingpongTexture);
	for (int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_PingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, m_PingpongTexture[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA16F, 512, 512, 0, GL_RGBA, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PingpongTexture[i], 0);
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "fbo creation failed" << std::endl;
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// 10.13
void Renderer::DrawGridMesh()
{
	// Time
	m_Time += 0.0016;

	int shader = m_GridMeshShader;
	//Program select
	glUseProgram(shader);

	int uniformTex = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(uniformTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_YTTexture);

	int uTimeLoc = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTimeLoc, m_Time);
	// 10.21
	int uPoints = glGetUniformLocation(shader, "u_Points");
	glUniform4fv(uPoints, 50, m_Points); // 4개의 float 의 array(v) -> 4fv

	int attribPosition = glGetAttribLocation(
		shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_GridMeshVBO);
	glVertexAttribPointer(attribPosition, 3, 
		GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);	// x,y,z

	//glDrawArrays(GL_LINE_STRIP, 0, m_GridMeshVertexCount);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_GridMeshVertexCount);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// 10.14
void Renderer::DrawFullScreenColor(float r, float g, float b, float a)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int shader = m_FullScreenShader;
	//Program select
	glUseProgram(shader);

	glUniform4f(glGetUniformLocation(shader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFullScreen);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_BLEND);
}

void Renderer::DrawFS()
{
	// Time
	m_Time += 0.016;

	const int shader = m_FSShader;
	glUseProgram(shader);
	
	// Default
	// GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	// glDrawBuffers(1, DrawBuffers);
	
	// GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT1 };
	// glDrawBuffers(1, DrawBuffers);

	GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, DrawBuffers);

	glUniform1f(glGetUniformLocation(shader, "u_Time"), m_Time);

	int uTextureLoc = glGetUniformLocation(shader, "u_RGBTexture");
	glUniform1i(uTextureLoc, 15);

	int uDigitTexLoc = glGetUniformLocation(shader, "u_DigitTexture");
	glUniform1i(uDigitTexLoc, 10);

	int uNumTexture = glGetUniformLocation(shader, "u_NumTexture");
	glUniform1i(uNumTexture, 10);

	int uDigitLoc = glGetUniformLocation(shader, "u_Digit");
	glUniform1i(uDigitLoc, (int)std::floor(m_Time) % 10);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_0Texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_1Texture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_2Texture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_3Texture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_4Texture);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_5Texture);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_6Texture);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, m_7Texture);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, m_8Texture);
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, m_9Texture);


	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture);

	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_2D, m_RGBTexture);

	glActiveTexture(GL_TEXTURE16);
	glBindTexture(GL_TEXTURE_2D, m_YTTexture);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFS);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawTexture(float x, float y, float sx, float sy, GLuint TexID, GLuint TexID1, GLuint method)
{
	int shader = m_TexShader;
	//Program select
	glUseProgram(shader);

	int uTex = glGetUniformLocation(shader, "u_TexID");
	glUniform1i(uTex, 0);

	int uTex1 = glGetUniformLocation(shader, "u_TexID1");
	glUniform1i(uTex1, 1);

	int uSize = glGetUniformLocation(shader, "u_Size");
	glUniform2f(uSize, sx, sy);

	int uTrans = glGetUniformLocation(shader, "u_Trans");
	glUniform2f(uTrans, x, y);

	int uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, m_Time);

	int uMethod = glGetUniformLocation(shader, "u_Method");
	glUniform1i(uMethod, method);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TexID);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TexID1);

	int aPosition = glGetAttribLocation(shader, "a_Pos");
	int aTex = glGetAttribLocation(shader, "a_Tex");
	glEnableVertexAttribArray(aPosition);
	glEnableVertexAttribArray(aTex);


	glBindBuffer(GL_ARRAY_BUFFER, m_TexVBO);
	glVertexAttribPointer(aPosition, 3, GL_FLOAT, 
		GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(aTex, 2, GL_FLOAT, 
		GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPosition);
	glDisableVertexAttribArray(aTex);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::DrawDebugTextures()
{
	glViewport(0, 0, 512, 512);
	//DrawTexture(-0.5, -0.5, 0.5, 0.5, m_PingpongTexture[0], 0, 0);
	//DrawTexture(0.5, -0.5, 0.5, 0.5, m_PingpongTexture[1], 0, 2);
	//DrawTexture(0.5, -0.5, 0.5, 0.5, m_HDRRT0_1, 0, 2);
	//DrawTexture(0, 0, 1, 1, m_PingpongTexture[0], 0, 2);

	//glViewport(0, 0, 512, 512);
	//DrawTexture(0.5, -0.5, 0.5, 0.5, m_RTs_1[0]);
	//DrawTexture(-0.0, -0.8, 0.2, 0.2, m_RTs[2]);
}

void Renderer::DrawFBOs()
{
	//{
	//	// set FBO
	//	glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[0]);
	//	//glViewport(0, 0, 256, 512);		// -> 파티클이 그려진 프레임버퍼만 반토막남
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear after bind

	//	// Draw
	//	DrawFS();
	//}

	//{
	//	// set FBO
	//	glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[1]);
	//	glViewport(0, 0, 512, 512);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear after bind
	//
	//	// Draw
	//	DrawGridMesh();
	//}

	{
		// set FBO
		glBindFramebuffer(GL_FRAMEBUFFER, m_HDRFBO0);
		glViewport(0, 0, 512, 512);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear after bind
	
		// Draw
		DrawParticle();
	}


	//
	//{
	//	// set FBO
	//	glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[2]);
	//	glViewport(0, 0, 512, 512);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear after bind
	//
	//	// Draw
	//	DrawFS();
	//}

	// Restore FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawBloomParticles()
{
	// 1. Render particle effect (HDR Texture)
	// set FBO
	glBindFramebuffer(GL_FRAMEBUFFER, m_HDRFBO0);
	glViewport(0, 0, 512, 512);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear after bind

	// Draw
	DrawParticle();

	// 2. Blur
	glBindFramebuffer(GL_FRAMEBUFFER, m_PingpongFBO[0]);
	DrawTexture(0, 0, 1, 1, m_HDRRT0_1, 0, 1);
	
	for (int i = 0; i < 2; ++i) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_PingpongFBO[1]);
		DrawTexture(0, 0, 1, 1, m_PingpongTexture[0], 0, 2);	// Draw PingpongTexture0 to 1
		glBindFramebuffer(GL_FRAMEBUFFER, m_PingpongFBO[0]);
		DrawTexture(0, 0, 1, 1, m_PingpongTexture[1], 0, 1);	// Draw PingpongTexture1 to 0
	}

	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 3. Main Frame Buffer <- Normal Texture + Blurred Texture
	DrawTexture(0, 0, 1, 1, m_HDRRT0_0, m_PingpongTexture[0], 3);


}
