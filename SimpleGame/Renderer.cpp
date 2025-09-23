#include "stdafx.h"
#include "Renderer.h"

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
	GenerateParticles(10000);

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
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

}

void Renderer::DeleteAllShaderPrograms()
{
	glDeleteShader(m_SolidRectShader);
	glDeleteShader(m_TestShader);
	glDeleteShader(m_ParticleShader);
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
	GLuint shader = m_ParticleShader;
	glUseProgram(shader);

	m_Time += 0.00016;
	int uTimeLoc = glGetUniformLocation(m_ParticleShader, "u_Time");
	glUniform1f(uTimeLoc, m_Time);

	int aPosLoc = glGetAttribLocation(m_ParticleShader, "a_Position");
	glEnableVertexAttribArray(aPosLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticle);	
	glVertexAttribPointer(
		aPosLoc, 3, GL_FLOAT,
		GL_FALSE, sizeof(float) * 12, 0);// x, y, z, value, r, g, b, a -> Stride 는 sizeof(float) * 8

	int aRadiusLoc = glGetAttribLocation(m_ParticleShader, "a_Radius");
	glEnableVertexAttribArray(aRadiusLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticle);
	glVertexAttribPointer(
		aRadiusLoc, 1, GL_FLOAT,
		GL_FALSE, sizeof(float) * 12,
		(GLvoid*)(sizeof(float) * 3)); 

	int aColorLoc = glGetAttribLocation(m_ParticleShader, "a_Color");
	glEnableVertexAttribArray(aColorLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticle);
	glVertexAttribPointer(
		aColorLoc, 4, GL_FLOAT,
		GL_FALSE, sizeof(float) * 12, 
		(GLvoid*)(sizeof(float) * 4));	

	int aSTimeLoc = glGetAttribLocation(m_ParticleShader, "a_STime");
	glEnableVertexAttribArray(aSTimeLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticle);
	glVertexAttribPointer(
		aSTimeLoc, 1, GL_FLOAT,
		GL_FALSE, sizeof(float) * 12, 
		(GLvoid*)(sizeof(float) * 8));

	int aVelocityLoc = glGetAttribLocation(m_ParticleShader, "a_Velocity");
	glEnableVertexAttribArray(aSTimeLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticle);
	glVertexAttribPointer(
		aSTimeLoc, 3, GL_FLOAT,
		GL_FALSE, sizeof(float) * 12, 
		(GLvoid*)(sizeof(float) * 9));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOParticleVertexCount - (6 * 9500));

	glDisableVertexAttribArray(aPosLoc);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

// Lecture 5 (09.22)
void Renderer::GenerateParticles(int numParticles)
{
	// (x, y, z), (value), (r, g, b, a), (sTime), (vx, vy, vz)
	int floatCountPerVertex = 3 + 1 + 4 + 1 + 3;
	int verticesCountPerParticle= 6;
	int floatCountPerParticle = floatCountPerVertex * verticesCountPerParticle;
	int totalVerticesCount = numParticles * verticesCountPerParticle;
	int totalFloatCount = floatCountPerVertex * totalVerticesCount;

	float* pVertices = new float[totalFloatCount];
	if (!pVertices) {
		return;
	}

	for (int i = 0; i < numParticles; ++i) {
		float x, y, z, value, r, g, b, a;
		x = 0;	// ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;	// -1.f ~ 1.f
		y = 0;	// ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;	// -1.f ~ 1.f
		z = 0.f;
		value = (float)rand() / (float)RAND_MAX;
		r = (float)rand() / (float)RAND_MAX;
		g = (float)rand() / (float)RAND_MAX;
		b  = (float)rand() / (float)RAND_MAX;
		a = (float)rand() / (float)RAND_MAX;

		float size;
		size = ((float)rand() / (float)RAND_MAX) * 0.01;	// 0~1 은 너무 크므로 적당히 줄임

		float sTime = ((float)rand() / (float)RAND_MAX) * 2.f;

		float vx, vy, vz;
		vx = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;	// -1.f ~ 1.f
		vy = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;	// -1.f ~ 1.f
		vz = 0.f;

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

	}

	glGenBuffers(1, &m_VBOParticle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCount, pVertices, GL_STATIC_DRAW);

	delete[] pVertices;

	m_VBOParticleVertexCount = numParticles * verticesCountPerParticle;
}
