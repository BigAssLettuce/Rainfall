#pragma once
//#include "../../Core/include.h"
//#include "../../Core/Registry.h"
#include "../../Core/RenderAPI/UniformBindingManager/UBO_Binding_Manager.h"
#include <string>
#include "../../Core/RenderAPI/RenderAPI.h"
#include "../../Core/Debug/Console.h"

enum ShaderTypes {
	VERTEX= GL_VERTEX_SHADER,
	FRAGMENT = GL_FRAGMENT_SHADER,
	GEOMETRY = GL_GEOMETRY_SHADER
};

struct ShaderPart {
	ShaderPart(ShaderTypes _Type, std::string _Content) { Type = _Type; Content = _Content; };
	ShaderTypes Type;
	std::string Content;
};
class Debugger;
class Shader
{
	friend Debugger;

	std::string ParseUniformBuffers(std::string source) {
		std::string returnstring = source;
		//int pos = 0;
		std::map<std::string, BindingPoint> bindingpoints = UBO_Binding_Manager::GetInstance()->GetBindingPoints();
		for (std::pair<std::string, BindingPoint> bindingpoint : bindingpoints) {
			size_t loc = returnstring.find("uniform " + bindingpoint.first);
			if (loc != -1) {
				//Debug::Log(to_string(static_cast<int>(loc)));
				returnstring.insert(static_cast<int>(loc), "layout ("+UBO_Binding_Manager::ParseType(bindingpoint.second.type)+", binding = " + to_string(bindingpoint.second.point) + ")");
				Console::Log("Added uniform buffer " + bindingpoint.first + " at " + to_string(bindingpoint.second.point));
			}
			//pos++;
		}
		//Debug::Log(returnstring);
		return returnstring;
	}
	std::vector<ShaderPart> ShaderParts = std::vector<ShaderPart>();
	static std::vector<Shader*> SHADERS;
public:
	std::string Name = "No Name";
	GLint shaderProgramID;
	Shader();
	~Shader();

	void LoadShader(std::vector<ShaderPart> Parts);
	bool LoadBasicShader(std::string vertex, std::string fragment);
	int getUniformLocation(string Uniform);
	void Use();

};
