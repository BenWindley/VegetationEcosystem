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

void VegetationBud::Update()
{
	if (m_fate)
	{
		m_remove = true;
	}
}

void VegetationBud::Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData)
{

}
