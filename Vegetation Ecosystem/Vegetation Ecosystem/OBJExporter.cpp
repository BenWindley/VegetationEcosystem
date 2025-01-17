// Reused from AT Building Generation in DirectX 11 (2018)
// Made by myself - Ben Windley

#include "pch.h"
#include "OBJExporter.h"

#include <Windows.h>
#include <ostream>
#include <fstream>
#include <ppltasks.h>
#include <codecvt>
#include <locale>

using namespace concurrency;

using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace DirectX;
using namespace Windows::Foundation;

using namespace std;

bool Utility::ExportOBJ(std::vector<Renderable*> objects, std::string fileName)
{
	auto platformPath = ApplicationData::Current->LocalFolder->Path;
	std::wstring platformPathW(platformPath->Begin());
	std::string convertedPlatformPath(platformPathW.begin(), platformPathW.end());

	std::string path = convertedPlatformPath + "\\" + fileName + ".obj";

	ofstream file;
	char buff[256];
	file.open(path.c_str(), std::ios::trunc);
	
	if (file.fail())
		return false;

	OutputDebugString(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(
		std::string("Exporting to: ") + path + "\n").c_str()
	);

	file << "o ExportedBuilding" << std::endl << std::endl;

	std::vector<Vegetation_Ecosystem::VertexPositionColor> vertices;
	std::vector<USHORT> indices;

	int v_offset = 1;

	for (int object_i = 0; object_i < objects.size(); ++object_i)
	{
		file << "g object" << (int)objects.at(object_i)->GetTextureID() << std::endl;
		file << "usemtl " << (int)objects.at(object_i)->GetTextureID() << std::endl << std::endl;

		std::vector<std::string> vertex_position;
		std::vector<std::string> vertex_tex_coord;

		vertices = objects.at(object_i)->GetVertices();
		indices = objects.at(object_i)->GetIndices();

		// Fill strings

		for (int i = 0; i < vertices.size(); ++i)
		{
			vertex_position.push_back("v " + std::to_string(vertices.at(i).pos.x) + " "
				+ std::to_string(vertices.at(i).pos.y) + " "
				+ std::to_string(vertices.at(i).pos.z));

			vertex_tex_coord.push_back("vt " + std::to_string(vertices.at(i).uv.x) + " "
				+ std::to_string(vertices.at(i).uv.y));
		}

		// Add position

		for (int i = 0; i < vertex_position.size(); ++i)
		{
			file << vertex_position[i] << std::endl;
		}

		file << std::endl;

		// Add Normal

		for (int i = 0; i < vertex_tex_coord.size(); ++i)
		{
			file << vertex_tex_coord[i] << std::endl;
		}

		file << std::endl;

		// Add Face

		for (int i = indices.size() - 1; i > 0; )
		{
			file << "f ";

			file << v_offset + indices.at(i) << "/" << v_offset + indices.at(i) << "/" << v_offset + indices.at(i) << " ";
			--i;
			file << v_offset + indices.at(i) << "/" << v_offset + indices.at(i) << "/" << v_offset + indices.at(i) << " ";
			--i;
			file << v_offset + indices.at(i) << "/" << v_offset + indices.at(i) << "/" << v_offset + indices.at(i) << " ";
			--i;

			file << std::endl;
		}
		file << std::endl;

		v_offset += vertices.size();
	}

	file.close();

	return true;
}
