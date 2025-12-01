#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	bool IsInitialized();
	void ReloadAllShaderPrograms();	// Lecture 6 (09.22)
	void DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a);

	// Lecture 2 (09.08)
	void DrawTest();

	// Lecture 5 (09.22)
	void DrawParticle();

	// 10.13
	void DrawGridMesh();

	// 10.14
	void DrawFullScreenColor(float r, float g, float b, float a);

	void DrawFS();

	// 11.25
	void DrawTexture(float x, float y, float sx, float sy, GLuint TexID);
	void DrawDebugTextures();
	void DrawFBOs();

private:
	void Initialize(int windowSizeX, int windowSizeY);
	void CompileAllShaderPrograms();	// Lecture 6 (09.23)
	void DeleteAllShaderPrograms();	// Lecture 6 (09.23)
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint Shader¤Ç¤¾Program, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects();
	void GetGLPosition(float x, float y, float *newX, float *newY);
	void GenerateParticles(int numParticles);	// Lecture 5 (09.22)
	
	void CreateGridMesh(int x, int y);	// 10.13

	// 11.17
	GLuint CreatePngTexture(char* filePath, GLuint samplingMethod);

	// 11.25
	void CreateFBOs();
	
	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_SolidRectShader = 0;

	// Lecture 2 (09.08)
	GLuint m_VBOTestPos = 0;

	// Lecture 3 (09.09)
	GLuint m_VBOTestColor = 0;

	// Lecture 4 (09.16)
	GLuint m_TestShader = 0;

	// Time (added 09.16)
	float m_Time = 0;

	// Particle (Lecture 5 (09.22))
	GLuint m_ParticleShader = 0;
	GLuint m_VBOParticle = 0;
	int m_VBOParticleVertexCount = 0;
	int m_floatCountPerVertex = 0;

	// Grid (10.13)
	GLuint m_GridMeshShader = 0;
	GLuint m_GridMeshVertexCount = 0;
	GLuint m_GridMeshVBO = 0;

	// FullScreen grid (10.14)
	GLuint m_VBOFullScreen = 0;
	GLuint m_FullScreenShader = 0;

	// RainDrop Position(10.21)
	float m_Points[100 * 4];

	GLuint m_FSShader = 0;
	GLuint m_VBOFS = 0;

	// 11.17
	GLuint m_RGBTexture = 0;

	// 11.17
	GLuint m_YTTexture = 0;

	// 11.24
	GLuint m_0Texture = 0;
	GLuint m_1Texture = 0;
	GLuint m_2Texture = 0;
	GLuint m_3Texture = 0;
	GLuint m_4Texture = 0;
	GLuint m_5Texture = 0;
	GLuint m_6Texture = 0;
	GLuint m_7Texture = 0;
	GLuint m_8Texture = 0;
	GLuint m_9Texture = 0;
	GLuint m_NumTexture = 0;


	// 11.25
	// Texture
	GLuint m_TexVBO = 0;
	GLuint m_TexShader = 0;

	// FBO Color Buffers
	GLuint m_RTs_0[5];	// ATTACHMENT0 (layout = 0)
	GLuint m_RTs_1[5];	// ATTACHMENT1 (layout = 1)

	// FBOs
	GLuint m_FBOs[5];

};

