#pragma once

#include "Renderable.h"

#include <vector>
#include <string>

namespace Utility
{
	bool ExportOBJ(std::vector<Renderable*> objects, std::string fileName);
}