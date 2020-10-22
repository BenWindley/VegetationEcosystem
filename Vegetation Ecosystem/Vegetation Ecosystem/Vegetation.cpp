#include "pch.h"
#include "Vegetation.h"

#include <thread>

Vegetation::Vegetation(Species species)
{
	m_species = species;
}

Vegetation::~Vegetation()
{
	m_keepThreads = false;

	while (m_threadCount > 0)
	{
		Sleep(0.1);
	}

	delete m_vegetationNode;
}

void Vegetation::Start(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources)
{
	m_vegetationNode = new VegetationNode();
	m_vegetationNode->Start(this, m_species, deviceResources, rendererResources);

	for (int i = 0; i < MAX_THREADS; ++i)
	{
		m_doThreads.push_back(std::thread(&Vegetation::DoThread, this));
		m_doThreads.back().detach();
		m_threadCount++;
	}
}

void Vegetation::Update(float time)
{
	if (!this) return;
	if (m_age > MAX_AGE) return;

	// Update features list

	m_vegetationNode->GetFeatures(&m_allFeatures);
	m_threadIterator = 0;
	m_threadsComplete = 0;

	m_updateThreads = true;

	while (m_updateThreads)
	{
		Sleep(0.01);
	} 

	m_allFeatures.clear();
	m_age += time;

	// Update growth rate
	m_growth += time * m_species.m_growthRate * m_vegetationNode->GetGrowthFactor() * 0.9f;

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

bool Vegetation::IsComplete()
{
	return m_age > MAX_AGE;
}

void Vegetation::DoThread()
{
	VegetationFeature* currentFeature;
	int i;

	while (m_keepThreads)
	{
		if (m_updateThreads)
		{
			i = m_threadIterator++;

			if (i >= m_allFeatures.size())
			{
				while (m_threadsComplete < m_allFeatures.size())
				{
					Sleep(0.01);
				}

				m_updateThreads = false;

				continue;
			}
			else
			{
				currentFeature = m_allFeatures[i];
				currentFeature->UpdateTropisms(&m_allFeatures);
				m_threadsComplete++;
			}
		}
		else
		{
			Sleep(0.01);
		}
	}

	m_threadCount--;
}
