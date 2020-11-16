// Reused from AT Building Generation in DirectX 11 (2018)
// Made by myself - Ben Windley

#pragma once
#include "Renderable.h"

#include <vector>
#include <string>

namespace Utility
{
	bool ExportOBJ(std::vector<Renderable*> objects, std::string fileName);
}