#pragma once
#include "../../Core/glm.h"

#include <vector>

enum MeshSize {
	UBYTE = UCHAR_MAX,
	USHORT = USHRT_MAX,
	UINT = UINT_MAX
};
template <typename VertexType, MeshSize Size>
class GMesh {

};

class Mesh{

	Mesh();
};



struct VertexLayout {
	std::vector<unsigned short> Strife;
	std::vector<unsigned short> Count;
};
struct Vertex {
	static VertexLayout layout;
};
struct Vertex3D :Vertex{

	Vertex3D(glm::vec3 pos, glm::vec3 nor, glm::vec2 uv) {
		POSITION = pos;
		NORMAL = nor;
		TEXCOORD = uv;
	}
	glm::vec3 POSITION;
	glm::vec3 NORMAL;
	glm::vec2 TEXCOORD;
};
struct Vertex2D :public Vertex{


	glm::vec2 POSITION;
	glm::vec2 TEXCOORD;
};

											 //max verticies  - MemSize
typedef GMesh<Vertex2D,  UBYTE>GMesh2D_LOW;	 //256
typedef GMesh<Vertex2D, USHORT>GMesh2D;		 //25,565 - 
typedef GMesh<Vertex2D,   UINT>GMesh2D_HIGH; //4,294,967,295 - 128MB

typedef GMesh<Vertex3D,  UBYTE> GMesh3D_1B;
typedef GMesh<Vertex3D, USHORT> GMesh3D_2B;
typedef GMesh<Vertex3D,   UINT> GMesh3D_4B; //4,294,967,295 - 128MB