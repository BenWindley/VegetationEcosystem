#include "pch.h"
#include "VegetationFeature.h"
#include "VegetationNode.h"

VegetationFeature::VegetationFeature(Transform* parent)
{
	m_parent = parent;
}

void VegetationFeature::Start(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources)
{
	m_deviceResources = deviceResources;
	m_rendererResources = rendererResources;

	//m_cube = new Cube();
	//m_cube->Init(m_deviceResources, m_rendererResources);
	//m_cube->m_parent = this;
}

void VegetationFeature::Update()
{

}

void VegetationFeature::Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData)
{
	//m_cube->Render(constantBufferData);
}

bool VegetationFeature::GetFate()
{
	return m_fate;
}

bool VegetationFeature::GetRemove()
{
	return m_remove;
}
