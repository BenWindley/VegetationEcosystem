#pragma once

#include "Renderable.h"

#include <vector>
#include <string>

namespace Utility
{
	static bool ExportOBJ(std::vector<Renderable> objects, std::string fileName);
}