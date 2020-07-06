#include "pch.h"
#include "VegetationBud.h"
#include "VegetationNode.h"

void VegetationBud::Update()
{
	if (m_fate)
	{
		m_remove = true;
	}
}

void VegetationBud::Render()
{

}
