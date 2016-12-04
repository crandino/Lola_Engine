#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__

#include "Assimp\include\mesh.h"

#include "MathGeoLib\MathGeoLib.h" 

typedef long unsigned int ID;
struct Mesh;
class ResourceMesh;

class MeshImporter
{

public:

	unsigned int static Save(char **data, const Mesh *mesh);

	unsigned int static Load(const std::string &imported_file, ResourceMesh *mesh);
	void static Load(const aiMesh *ai_mesh, Mesh *mesh);

	bool static Import(const aiMesh *ai_mesh, std::string &imported_file, ID &res_id);
};

#endif // !__MESHIMPORTER_H__
