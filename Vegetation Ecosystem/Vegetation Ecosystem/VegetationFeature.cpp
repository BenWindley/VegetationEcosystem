#include "pch.h"
#include "VegetationFeature.h"
#include "VegetationNode.h"

VegetationFeature::VegetationFeature(Transform* parent)
{
	m_parent = parent;
}

void VegetationFeature::Update()
{

}

void VegetationFeature::Render()
{

}

bool VegetationFeature::GetFate()
{
	return m_fate;
}

bool VegetationFeature::GetRemove()
{
	return m_remove;
}
