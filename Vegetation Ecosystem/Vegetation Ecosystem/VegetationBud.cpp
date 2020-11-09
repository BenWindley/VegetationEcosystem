#include "pch.h"
#include "VegetationBud.h"
#include "VegetationNode.h"

VegetationBud::VegetationBud(Transform* parent, bool terminalBud) : VegetationFeature(parent), m_terminalBud(terminalBud)
{

}

void VegetationBud::Start(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources)
{
	m_deviceResources = deviceResources;
	m_rendererResources = rendererResources;
}

void VegetationBud::Update(float growth, float time)
{
	m_storedGrowth += growth;
	m_age += time;

	if (m_storedGrowth > (m_branchWidth * 0.5f + 0.5f))
	{
		m_fate = true;
	}

	if (m_light < (m_terminal ? 0.15f : 0.2f))
	{
		m_dormant = true;
	}
}

void VegetationBud::Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData)
{

}
