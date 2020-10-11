#include "pch.h"
#include "Vegetation.h"

#define MAX_AGE 10.0f

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
	if (m_age > MAX_AGE) return;

	m_age += time;

	// Update features list
	std::vector<VegetationFeature*> allFeatures;
	m_vegetationNode->GetFeatures(&allFeatures);

	for (auto& vFeature : allFeatures)
	{
		vFeature->UpdateLighting(&allFeatures);
	}

	// Update growth rate
	m_growth += time * m_species.m_growthRate * m_vegetationNode->GetGrowthFactor();

	// Pass growth down list of nodes
	m_vegetationNode->Update(m_growth);

	// This can be changed if some growth is stored to prevent sudden changes of growth
	m_growth *= 0.1f;
}

void Vegetation::Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData)
{
	if (!this) return;

	m_vegetationNode->Render(constantBufferData);
}
