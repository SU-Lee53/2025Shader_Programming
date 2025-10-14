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

private:
	void Initialize(int windowSizeX, int windowSizeY);
	void CompileAllShaderPrograms();	// Lecture 6 (09.23)
	void DeleteAllShaderPrograms();	// Lecture 6 (09.23)
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects();
	void GetGLPosition(float x, float y, float *newX, float *newY);
	void GenerateParticles(int numParticles);	// Lecture 5 (09.22)
	
	void CreateGridMesh(int x, int y);	// 10.13

	
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
};

