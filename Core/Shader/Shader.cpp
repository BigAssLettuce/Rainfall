#include "Shader.h"
#include "../../Core/Resource/Resource.h"
#include "../Application/Application.h"
vector<Shader*> Shader::SHADERS = vector<Shader*>();
bool CompileSuccess(int Shader, string* Info) {
	int success;
	char infoLog[512];
	glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(Shader, 512, NULL, infoLog);
		*Info = infoLog;
		return false;
	}
	return true;
}
bool LinkSuccess(int Program, string* Info) {
	int success;
	char infoLog[512];
	glGetProgramiv(Program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(Program, 512, NULL, infoLog);
		*Info = infoLog;
		return false;
	}
	return true;
}

Shader::Shader()
{
	if (!Application::GetIsOpenGLInit()) return;
	shaderProgramID = glCreateProgram();
	Console::Log("Shader program ID: " + std::to_string(shaderProgramID));
	SHADERS.push_back(this);
}

Shader::~Shader()
{
	glDeleteProgram(shaderProgramID);
}



void Shader::LoadShader(vector<ShaderPart> Parts)
{
	//ShaderPart next = Parts;

	vector<int> shadersAttached = vector<int>();

	for (ShaderPart part : Parts) {

		int shader = glCreateShader((GLenum)part.Type);
		string ParsedSource = ParseUniformBuffers(part.Content);
		const char* fsSoutemprceTemp = ParsedSource.c_str();
		glShaderSource(shader, 1, &fsSoutemprceTemp, NULL);
		glCompileShader(shader);
		
		string CompileLog;
		if (!CompileSuccess(shader, &CompileLog)) Console::Error("ShaderProgram " + to_string(shaderProgramID) + "Failed to compile shaderpart" + to_string(part.Type) + "\n" + CompileLog);
		glAttachShader(shaderProgramID, shader);
		shadersAttached.push_back(shader);
	}
	
	glLinkProgram(shaderProgramID);
	string CompileLog;
	if (!LinkSuccess(shaderProgramID, &CompileLog)) Console::Error("ShaderProgram " + to_string(shaderProgramID) + " Link Failed\n" + CompileLog);
	glValidateProgram(shaderProgramID);
	for (int attachedShader : shadersAttached) {
		glDetachShader(shaderProgramID, attachedShader);
		glDeleteShader(attachedShader);
	}

}

bool Shader::LoadBasicShader(string vertexFile, string fragmentFile) //loads from Resources/Shaders/
{
	string vertexfilepath = vertexFile;
	string fragmentfilepath = fragmentFile;
	string vsSource;
	if (!Resource::ReadTextFile(vertexfilepath, &vsSource)) return false;
	ShaderPart vertexPart(VERTEX,vsSource);
	vertexPart.Type = ShaderTypes::VERTEX;
	vertexPart.Content = vsSource;
	ShaderParts.push_back(vertexPart);
	vsSource = ParseUniformBuffers(vsSource);
	const char* vsSourceTemp = vsSource.c_str();



	string fsSource;
	if (!Resource::ReadTextFile(fragmentfilepath, &fsSource)) return false;
	ShaderPart fragmentPart(FRAGMENT,fsSource);
	fragmentPart.Type = ShaderTypes::FRAGMENT;
	fragmentPart.Content = fsSource;
	ShaderParts.push_back(fragmentPart);
	fsSource = ParseUniformBuffers(fsSource);
	const char* fsSourceTemp = fsSource.c_str();



	int vsShader = glCreateShader(GL_VERTEX_SHADER);
	int fsShader = glCreateShader(GL_FRAGMENT_SHADER);

	string CompileLog;
	glShaderSource(vsShader, 1,&vsSourceTemp, NULL);
	glCompileShader(vsShader);
	if (!CompileSuccess(vsShader, &CompileLog)) Console::Error("Shader " + vertexFile + " Compile Failed\n" + CompileLog);

	glShaderSource(fsShader, 1, &fsSourceTemp, NULL);
	glCompileShader(fsShader);
	if (!CompileSuccess(fsShader, &CompileLog)) Console::Error("Shader " + fragmentFile + " Compile Failed\n" + CompileLog);

	glAttachShader(shaderProgramID, vsShader);
	glAttachShader(shaderProgramID, fsShader);
	glLinkProgram(shaderProgramID);
	if (!LinkSuccess(shaderProgramID, &CompileLog)) Console::Error("ShaderProgram " + to_string(shaderProgramID) + " Link Failed\n" + CompileLog);
	glValidateProgram(shaderProgramID);

	glDetachShader(shaderProgramID, vsShader);
	glDetachShader(shaderProgramID, fsShader);

	glDeleteShader(vsShader);
	glDeleteShader(fsShader);

	
	return true;
}

int Shader::getUniformLocation(string Uniform)
{
	return glGetUniformLocation(shaderProgramID, Uniform.c_str());
}

void Shader::Use(){ glUseProgram(shaderProgramID); }
