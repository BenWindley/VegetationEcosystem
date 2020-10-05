#include "pch.h"
#include "Vegetation.h"

Vegetation::Vegetation(Species species)
{
	m_species = species;
}

Vegetation::~Vegetation()
{
	delete m_vegetationNode;
}

void Vegetation::Start(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources)
{
	m_vegetationNode = new VegetationNode();
	m_vegetationNode->Start(this, deviceResources, rendererResources);
}

void Vegetation::Update(float time)
{
	if (!this) return;

	m_growth += time * m_species.m_growthRate;

	m_vegetationNode->Update(m_growth);

	m_growth = 0;
}

void Vegetation::Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData)
{
	if (!this) return;

	m_vegetationNode->Render(constantBufferData);
}
