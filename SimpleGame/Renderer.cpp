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

	glGenBuffers(1, &m_VBOTestPos);	// GPU�� VBO�� ����� �� ID �� testID �� ���� -> ID �� �����ϰ� GPU �޸𸮴� �Ҵ����� ����
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestPos);
	/*
	    + --------------- +
		| GL_ARRAY_BUFFER | 
	    + --------------- +
	    |     testID      | <- glBindBuffer() �� ����� testID �� Buffer Object �� �̷������� ������
		+ --------------- +

		���߿� VBO�� ã�ư� �� GL_ARRAY_BUFFER �ȿ� �ִ� testID�� ã�� ���ҽ��� read/write �ϰԵ�
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(testPos), testPos, GL_STATIC_DRAW); 
	// Bind�� VBO(testID) �� �����͸� �Ҵ��� -> �� �Լ��� ȣ��� ���� �޸𸮰� VRAM�� ����
	// �Լ� Ư���� Async �ϰ� �ð��� ���� �ɸ� �� ����

	// Lecture 3 (09.09)
	// ���� ���� �߰�
	// ä�κ� RGBA 0.f ~ 1.0 ������ ���� ����
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
	//���̴� ������Ʈ ����
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
	//���̴� �ڵ带 ���̴� ������Ʈ�� �Ҵ�
	glShaderSource(ShaderObj, 1, p, Lengths);

	//�Ҵ�� ���̴� �ڵ带 ������
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj �� ���������� ������ �Ǿ����� Ȯ��
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL �� shader log �����͸� ������
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram �� attach!!
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
	GLuint ShaderProgram = glCreateProgram(); //�� ���̴� ���α׷� ����

	if (ShaderProgram == 0) { //���̴� ���α׷��� ����������� Ȯ��
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs �� vs ������ �ε���
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs �� fs ������ �ε���
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram �� vs.c_str() ���ؽ� ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram �� fs.c_str() �����׸�Ʈ ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach �Ϸ�� shaderProgram �� ��ŷ��
	glLinkProgram(ShaderProgram);

	//��ũ�� �����ߴ��� Ȯ��
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program �α׸� �޾ƿ�
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

	// m_TestShader ���α׷����� a_Position �̶�� Attribute Location �� �����Ͷ�
	int aPosLoc = glGetAttribLocation(m_TestShader, "a_Position");
	// ���� Get �ϱ� �ȴٸ� layout(location = n) �� ����ϰ�
	// glEnableVertexAttribArray(n); �� ����Ѵ�

	// Enable �ؾ� ����� ������(�ٷ� �Ʒ� Bind, AttribPorinter ��)
	glEnableVertexAttribArray(aPosLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestPos);	// Bind ���־�� �Ʒ� AttribPointer ���� �۾��� ����
	glVertexAttribPointer(
		aPosLoc, 3, GL_FLOAT, 
		GL_FALSE, sizeof(float) * 4, 0); // VBO �� ��� �ؼ����� ������ ������
	// a_Radius �� �߰��� ��Ȳ���� size �� 3�� ���� ����
	// �ٸ� Stride �� ���� ������ ������ġ�� �˷��� sizeof(float) * 4 ��ŭ �̵��ؾ� �ϹǷ� ������ ����

	// Lecture 6 (09.22)
	int aRadiusLoc = glGetAttribLocation(m_TestShader, "a_Radius");
	glEnableVertexAttribArray(aRadiusLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestPos);	// Bind ���־�� �Ʒ� AttribPointer ���� �۾��� ����
	glVertexAttribPointer(
		aRadiusLoc, 1, GL_FLOAT,
		GL_FALSE, sizeof(float) * 4, 
		(GLvoid*)(sizeof(float) * 3)); // VBO �� ��� �ؼ����� ������ ������

	// [   x   ][   y   ][   z   ][ Radius ] | [   x   ][   y   ][   z   ][ Radius ] 
	// a_Radius �� ��쿡�� ���������� stride �� sizeof(float) * 4
	// �ٸ� ��� ���� �о�;� �ϴ���(������ param) �� �޶����µ� float 3���� �ǳʶٰ�(xyz) �о������ ������

	// Lecture 3 (09.09)
	int aColorLoc = glGetAttribLocation(m_TestShader, "a_Color");
	glEnableVertexAttribArray(aColorLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestColor);
	glVertexAttribPointer(
		aColorLoc, 4, GL_FLOAT,
		GL_FALSE, sizeof(float) * 4, 0);	// size �� stride �� �ٲ��� ��
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
		GL_FALSE, sizeof(float) * 12, 0);// x, y, z, value, r, g, b, a -> Stride �� sizeof(float) * 8

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
		size = ((float)rand() / (float)RAND_MAX) * 0.01;	// 0~1 �� �ʹ� ũ�Ƿ� ������ ����

		float sTime = ((float)rand() / (float)RAND_MAX) * 2.f;

		float vx, vy, vz;
		vx = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;	// -1.f ~ 1.f
		vy = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;	// -1.f ~ 1.f
		vz = 0.f;

		int index = i * floatCountPerParticle;
		pVertices[index] = x - size;	index++;	// v1 ���ϴ�
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
		
		pVertices[index] = x + size;	index++;	// v2 ����
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
		
		pVertices[index] = x - size;	index++;	// v3 �»��
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
		
		pVertices[index] = x - size;	index++;	// v4 ���ϴ�
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
		
		pVertices[index] = x + size;	index++;	// v5 ���ϴ�
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
		
		pVertices[index] = x + size;	index++;	// v6 ����
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
